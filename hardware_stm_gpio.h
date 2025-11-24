/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_GPIO_H_
#define __HARDWARE_STM_GPIO_H_

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
void initGpioBxAsInput(uint16_t );
void initGpioBxAsOutput(uint16_t );
void toggleGPIOBx( uint16_t );
void setGPIOBx( uint16_t );
void clearGPIOBx( uint16_t );
void initGpioCxAsInput( uint16_t );
void initGpioCxAsInput_PU( uint16_t);
void initGpioCxAsInputNoPull(uint16_t);
uint32_t checkGPIOBx(uint16_t);
uint32_t checkGPIOCx(uint16_t);
void initGpioBxAsAF2(uint16_t);
void initGpioCxAsAF2(uint16_t);
void initGpioBxForPWM(uint16_t);
void initGpioFxAsOutput(uint16_t);
void setGPIOFx(uint16_t);
void clearGPIOFx(uint16_t);
uint32_t checkGPIOFx(uint16_t );
void initGpioFxAsAnalog(uint16_t);
void initGpioAxAsOutput(uint16_t);
void setGPIOAx(uint16_t);
void clearGPIOAx(uint16_t);
uint32_t checkGPIOAx(uint16_t );
void initGpioAxAsAnalog(uint16_t);
void initGpioAxAsAF2(uint16_t);
void initGpioBxAsAF4(uint8_t);
void initGpioAxAsAF4(uint8_t);
void initGpioCxAsAF4(uint8_t);
void initGpioFxAsAF4(uint8_t);
void initGpioHxAsAF4(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */
