/**
  ******************************************************************************
  * @file    hardware_stm_dma_controller.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Nov-2025
  * @brief   DMA controller implementation for STM32F446ZE
  ******************************************************************************
  */

#include "hardware_stm_dma_controller.h"
#include "hardware_stm_adc.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"

// DMA Base Addresses
#define DMA1_BASE_ADDRESS     ((uint32_t)0x40026000)
#define DMA2_BASE_ADDRESS     ((uint32_t)0x40026400)

// DMA2 Registers
#define DMA2_LISR   (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x00))
#define DMA2_HISR   (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x04))
#define DMA2_LIFCR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x08))
#define DMA2_HIFCR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x0C))

// DMA2 Stream 0 Registers
#define DMA2_STREAM0_CR       (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x10))
#define DMA2_STREAM0_NDTR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x14))
#define DMA2_STREAM0_PAR      (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x18))
#define DMA2_STREAM0_M0AR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x1C))
#define DMA2_STREAM0_M1AR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x20))
#define DMA2_STREAM0_FCR      (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x24))

// DMA2 Stream 2 Registers
#define DMA2_STREAM2_CR   (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x50))
#define DMA2_STREAM2_NDTR (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x54))
#define DMA2_STREAM2_PAR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x58))
#define DMA2_STREAM2_M0AR (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x5C))
#define DMA2_STREAM2_FCR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x64))

// DMA Bitfield Masks
#define DMA_SxCR_EN           (1UL << 0)
#define DMA_SxCR_CIRC         (1UL << 8)
#define DMA_SxCR_DIR_P2M      (0UL << 6)
#define DMA_SxCR_MINC         (1UL << 10)
#define DMA_SxCR_PINC         (1UL << 9)
#define DMA_SxCR_PSIZE_16BIT  (1UL << 11)
#define DMA_SxCR_MSIZE_16BIT  (1UL << 13)
#define DMA_SxCR_TCIE         (1UL << 4)
// Channel selection (CHSEL bits [27:25]) 
#define DMA_CHANNEL(x)        ((uint32_t)(x) << 25)

// Global Interrupt Flags
#define DMA_TCIF0             (1UL << 5)
#define DMA_TCIF2             (1UL << 21)

typedef struct {
    uint8_t adc_num;                 // 1,2,3
    volatile uint32_t *ndtr_reg;     // address of DMA2_SxNDTR for that ADC
} ADC_DMA_Map;

// ADC1→DMA2 Stream0, ADC2→DMA2 Stream2, ADC3→DMA2 Stream0
static const ADC_DMA_Map adc_dma_map[] = {
    {1, &DMA2_STREAM0_NDTR},
    {2, &DMA2_STREAM2_NDTR},
    {3, &DMA2_STREAM0_NDTR},
};

// Callback pointer for DMA transfer complete
static void (*dma_transfer_complete_callback)(void) = 0;

// Internal helper
static void dma_configure_stream(volatile uint32_t *stream_cr,
                                 volatile uint32_t *stream_ndtr,
                                 volatile uint32_t *stream_par,
                                 volatile uint32_t *stream_m0ar,
                                 volatile uint32_t *stream_fcr,
                                 uint8_t channel,
                                 uint16_t *buffer,
                                 uint16_t length,
                                 uint32_t peripheral_address)
{
    /* Disable stream before config */
    *stream_cr &= ~DMA_SxCR_EN;
    while (*stream_cr & DMA_SxCR_EN);

    /* Reset */
    *stream_cr = 0;
    *stream_fcr = 0;

    /* Channel selection and configuration */
    *stream_cr |= DMA_CHANNEL(channel);
    *stream_cr |= DMA_SxCR_DIR_P2M;
    *stream_cr |= DMA_SxCR_MINC;
    *stream_cr &= ~DMA_SxCR_PINC;
    *stream_cr |= DMA_SxCR_PSIZE_16BIT | DMA_SxCR_MSIZE_16BIT;
    *stream_cr |= DMA_SxCR_CIRC;
    *stream_cr |= DMA_SxCR_TCIE;   // enable transfer complete interrupt

    *stream_ndtr = length;
    *stream_par  = peripheral_address;
    *stream_m0ar = (uint32_t)(uintptr_t)buffer;

    *stream_cr |= DMA_SxCR_EN;
}

// ADC1 → DMA2 Stream0 Channel0
void dma_init_for_adc1(uint16_t *buffer, uint16_t length)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    dma_configure_stream(
        &DMA2_STREAM0_CR, &DMA2_STREAM0_NDTR, &DMA2_STREAM0_PAR,
        &DMA2_STREAM0_M0AR, &DMA2_STREAM0_FCR,
        0, buffer, length, adc_get_data_register_address(1)
    );
}

// ADC2 → DMA2 Stream2 Channel1
void dma_init_for_adc2(uint16_t *buffer, uint16_t length)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    dma_configure_stream(
        &DMA2_STREAM2_CR, &DMA2_STREAM2_NDTR, &DMA2_STREAM2_PAR,
        &DMA2_STREAM2_M0AR, &DMA2_STREAM2_FCR,
        1, buffer, length, adc_get_data_register_address(2)
    );
}

// ADC3 → DMA2 Stream0 Channel2
void dma_init_for_adc3(uint16_t *buffer, uint16_t length)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    dma_configure_stream(
        &DMA2_STREAM0_CR, &DMA2_STREAM0_NDTR, &DMA2_STREAM0_PAR,
        &DMA2_STREAM0_M0AR, &DMA2_STREAM0_FCR,
        2, buffer, length, adc_get_data_register_address(3)
    );
}

// Interrupt Callback Handling
void dma_register_callback(void (*cb)(void))
{
    dma_transfer_complete_callback = cb;
}

void dma_enable_irq(void)
{
    /* NVIC IRQ channel for DMA2 Stream0 */
    volatile uint32_t *NVIC_ISER1 = (uint32_t *)0xE000E104;
    *NVIC_ISER1 = (1UL << (DMA2_Stream0_IRQn & 0x1F));
}

void dma_disable_irq(void)
{
    volatile uint32_t *NVIC_ICER1 = (uint32_t *)0xE000E184;
    *NVIC_ICER1 = (1UL << (DMA2_Stream0_IRQn & 0x1F));
}


// DMA2 Stream0 Interrupt Service Routine (for ADC1/3)
void DMA2_Stream0_IRQHandler(void)
{
    if (DMA2_LISR & DMA_TCIF0)
    {
        DMA2_LIFCR |= DMA_TCIF0; // Clear transfer complete flag

        if (dma_transfer_complete_callback)
            dma_transfer_complete_callback();
    }
}


// DMA2 Stream2 Interrupt Service Routine (for ADC2)
void DMA2_Stream2_IRQHandler(void)
{
    if (DMA2_LISR & DMA_TCIF2)
    {
        DMA2_LIFCR |= DMA_TCIF2; // Clear transfer complete flag

        if (dma_transfer_complete_callback)
            dma_transfer_complete_callback();
    }
}

/* ============================================================================
 * Utility
 * ==========================================================================*/
void dma_stop_all(void)
{
    DMA2_STREAM0_CR &= ~DMA_SxCR_EN;
    DMA2_STREAM2_CR &= ~DMA_SxCR_EN;
}

void dma_clear_flags(void)
{
    DMA2_LIFCR = 0xFFFFFFFF;
    DMA2_HIFCR = 0xFFFFFFFF;
}

static const ADC_DMA_Map* find_map(uint8_t adc_num)
{
    for (unsigned i = 0; i < (sizeof(adc_dma_map)/sizeof(adc_dma_map[0])); ++i)
        if (adc_dma_map[i].adc_num == adc_num) return &adc_dma_map[i];
    return 0;
}

uint16_t dma_get_write_index_for_adc(uint8_t adc_num, uint16_t buffer_size)
{
    const ADC_DMA_Map *m = find_map(adc_num);
    if (!m || buffer_size == 0) return 0;
    uint16_t remaining = (uint16_t)(*(m->ndtr_reg));          // NDTR counts down
    return (uint16_t)((buffer_size - remaining) % buffer_size);
}

uint16_t dma_get_latest_sample_for_adc(uint8_t adc_num, uint16_t *buffer, uint16_t buffer_size)
{
    uint16_t idx = dma_get_write_index_for_adc(adc_num, buffer_size);
    /* The latest completed sample is at idx-1 (wrap safe) */
    uint16_t last = (uint16_t)((idx + buffer_size - 1) % buffer_size);
    return buffer[last];
}

void enableDMAForTimer3Channel2(void)
{
    // TODO: implement real DMA setup for TIM3 CH2.
    // For now this is a stub so code links.
}