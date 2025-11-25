/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_DMA_CTRL_H_
#define __HARDWARE_STM_DMA_CTRL_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"


/* Macros for Everyone--------------------------------------------------------*/
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10

// Register addresses

// DMA Base Addresses
#define DMA1_BASE_ADDRESS     ((uint32_t)0x40026000)
#define DMA2_BASE_ADDRESS     ((uint32_t)0x40026400)

// DMA2 Registers
#define DMA2_LISR   (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x00))
#define DMA2_HISR   (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x04))
#define DMA2_LIFCR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x08))
#define DMA2_HIFCR  (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x0C))

// DMA1 Registers
#define DMA1_LISR   (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x00))
#define DMA1_HISR   (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x04))
#define DMA1_LIFCR  (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x08))
#define DMA1_HIFCR  (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x0C))

// DMA2 Stream 0 Registers
#define DMA2_STREAM0_CR       (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x10))
#define DMA2_STREAM0_NDTR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x14))
#define DMA2_STREAM0_PAR      (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x18))
#define DMA2_STREAM0_M0AR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x1C))
#define DMA2_STREAM0_M1AR     (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x20))
#define DMA2_STREAM0_FCR      (*(volatile uint32_t *)(DMA2_BASE_ADDRESS + 0x24))

// DMA1 Stream 5 Registers
#define DMA1_STREAM5_CR       (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x10 + 0x78))
#define DMA1_STREAM5_NDTR     (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x14 + 0x78))
#define DMA1_STREAM5_PAR      (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x18 + 0x78))
#define DMA1_STREAM5_M0AR     (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x1C + 0x78))
#define DMA1_STREAM5_M1AR     (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x20 + 0x78))
#define DMA1_STREAM5_FCR      (*(volatile uint32_t *)(DMA1_BASE_ADDRESS + 0x24 + + 0x78))

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
#define DMA_SxCR_DIR_M2P      (1UL << 6)
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


/*Function definitions---------------------------------------------------------*/
// void dma_init_for_adc1(uint16_t *buffer, uint16_t length);
// void dma_init_for_adc2(uint16_t *buffer, uint16_t length);
// void dma_init_for_adc3(uint16_t *buffer, uint16_t length);
// uint16_t dma_get_write_index_for_adc(uint8_t adc_num, uint16_t buffer_size);
// uint16_t dma_get_latest_sample_for_adc(uint8_t adc_num, uint16_t *buffer, uint16_t buffer_size);

/* Interrupt handling */
// void dma_register_callback(void (*cb)(void));
// void dma_enable_irq(void);
// void dma_disable_irq(void);

/* Utility functions */
void dma_stop_all(void);
void dma_clear_flags(void);

void enableDMAForTimer3Channel2(void);


#ifdef __cplusplus
}
#endif

#endif /*__DMA_CTRL_H */
