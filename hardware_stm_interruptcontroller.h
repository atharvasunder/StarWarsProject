/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_INTERRUPTCONTROLLER_H
#define __HARDWARE_STM_INTERRUPTCONTROLLER_H
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
void enableEXTI6OnPortC(void);
void EXTI9_5_IRQHandler(void);
void TIM3_IRQHandler(void);

extern volatile int16_t    buttonPressFlag;
extern volatile int32_t    pressedTime;

#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTCONTROLLER_H */
