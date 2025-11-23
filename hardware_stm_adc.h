/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_ADC_H_
#define __HARDWARE_STM_ADC_H_

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
void initADC1(void);
void initADC3(void);
void initADC3_DMA(uint16_t*, uint8_t );
void initADC_DMA(uint8_t , const uint8_t *, uint8_t );
int initADCxINy(uint8_t , uint8_t);
uint16_t readADC1Channel(uint8_t );
uint16_t readADC2Channel(uint8_t );
uint16_t readADC3Channel(uint8_t );
uint32_t adc_get_data_register_address(uint8_t);
void startADC_DMA(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /*__ADC_H */
