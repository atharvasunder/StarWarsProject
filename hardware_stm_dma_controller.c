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
#include "led_strip_utils.h"
#include "hardware_stm_adc.h"
#include "hardware_stm_timer3.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"


// typedef struct {
//     uint8_t adc_num;                 // 1,2,3
//     volatile uint32_t *ndtr_reg;     // address of DMA2_SxNDTR for that ADC
// } ADC_DMA_Map;

// // ADC1→DMA2 Stream0, ADC2→DMA2 Stream2, ADC3→DMA2 Stream0
// static const ADC_DMA_Map adc_dma_map[] = {
//     {1, &DMA2_STREAM0_NDTR},
//     {2, &DMA2_STREAM2_NDTR},
//     {3, &DMA2_STREAM0_NDTR},
// };

// Callback pointer for DMA transfer complete
static void (*dma_transfer_complete_callback)(void) = 0;


void initDMAForTimer3Channel2(void){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


    volatile uint32_t *stream_cr = &DMA1_STREAM5_CR;
    volatile uint32_t *stream_ndtr = &DMA1_STREAM5_NDTR; 
    volatile uint32_t *stream_par = &DMA1_STREAM5_PAR;
    volatile uint32_t *stream_m0ar = &DMA1_STREAM5_M0AR;
    volatile uint32_t *stream_fcr = &DMA1_STREAM5_FCR;


    /* Disable stream before config */
    *stream_cr &= ~DMA_SxCR_EN;
    while (*stream_cr & DMA_SxCR_EN);

    /* Reset (reset value = 0, just as for safety) */ 
    *stream_cr = 0;
    *stream_fcr = 0;

    /* Channel selection and configuration */
    *stream_cr |= DMA_CHANNEL(5);
    *stream_cr |= DMA_SxCR_DIR_M2P;
    *stream_cr |= DMA_SxCR_MINC;
    *stream_cr &= ~DMA_SxCR_PINC;
    *stream_cr |= DMA_SxCR_PSIZE_16BIT | DMA_SxCR_MSIZE_16BIT;
    *stream_cr &= ~DMA_SxCR_CIRC;
    // *stream_cr |= DMA_SxCR_TCIE;   // enable transfer complete interrupt

    /* Each DMA transfer consists of a mesage length corresponding to total number 
    of LED color bits plus some bits of zero duty cycle for locking in the led color*/
    *stream_ndtr = MESSAGE_LENGTH;

    // peripheral address
    *stream_par  = (uint32_t)&TIM3_CCR2;          // should be timer3 CCR2

    // memory address
    *stream_m0ar = (uint32_t)get_led_message_address();    // insert address
}

void enableDMAForTimer3Channel2(void){
    volatile uint32_t *stream_cr = &DMA1_STREAM5_CR;
    volatile uint32_t *stream_ndtr = &DMA1_STREAM5_NDTR; 
    volatile uint32_t *stream_par = &DMA1_STREAM5_PAR;
    volatile uint32_t *stream_m0ar = &DMA1_STREAM5_M0AR;
    volatile uint32_t *stream_fcr = &DMA1_STREAM5_FCR;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* Disable stream before config */
    *stream_cr &= ~DMA_SxCR_EN;
    while (*stream_cr & DMA_SxCR_EN);

    /* clear all previously created flags (half transfer complete, transfer complete flags)*/
    dma_clear_flags();

    /* Reset (reset value = 0, just as for safety) */ 
    *stream_cr = 0;
    *stream_fcr = 0;

    /* Channel selection and configuration */
    *stream_cr |= DMA_CHANNEL(5);
    *stream_cr |= DMA_SxCR_DIR_M2P;
    *stream_cr |= DMA_SxCR_MINC;
    *stream_cr &= ~DMA_SxCR_PINC;
    *stream_cr |= DMA_SxCR_PSIZE_16BIT | DMA_SxCR_MSIZE_16BIT;
    *stream_cr &= ~DMA_SxCR_CIRC;
    // *stream_cr |= DMA_SxCR_TCIE;   // enable transfer complete interrupt

    /* Each DMA transfer consists of a mesage length corresponding to total number 
    of LED color bits plus some bits of zero duty cycle for locking in the led color*/
    *stream_ndtr = MESSAGE_LENGTH;

    // peripheral address
    *stream_par  = (uint32_t)&TIM3_CCR2;          // should be timer3 CCR2

    // memory address
    *stream_m0ar = (uint32_t)get_led_message_address();    // insert address

    // enable stream
    *stream_cr |= DMA_SxCR_EN;
}

/* ============================================================================
 * Utility
 * ==========================================================================*/
void dma_stop_all(void)
{
    DMA1_STREAM5_CR &= ~DMA_SxCR_EN;
    DMA2_STREAM0_CR &= ~DMA_SxCR_EN;
    DMA2_STREAM2_CR &= ~DMA_SxCR_EN;
}

void dma_clear_flags(void)
{
    DMA1_LIFCR = 0xFFFFFFFF;
    DMA1_HIFCR = 0xFFFFFFFF;
}
