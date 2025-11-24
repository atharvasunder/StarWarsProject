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




/*Function definitions---------------------------------------------------------*/
void dma_init_for_adc1(uint16_t *buffer, uint16_t length);
void dma_init_for_adc2(uint16_t *buffer, uint16_t length);
void dma_init_for_adc3(uint16_t *buffer, uint16_t length);
uint16_t dma_get_write_index_for_adc(uint8_t adc_num, uint16_t buffer_size);
uint16_t dma_get_latest_sample_for_adc(uint8_t adc_num, uint16_t *buffer, uint16_t buffer_size);

/* Interrupt handling */
void dma_register_callback(void (*cb)(void));
void dma_enable_irq(void);
void dma_disable_irq(void);

/* Utility functions */
void dma_stop_all(void);
void dma_clear_flags(void);

/* for function testing */
void enableDMAForTimer3Channel2(void);


#ifdef __cplusplus
}
#endif

#endif /*__DMA_CTRL_H */
