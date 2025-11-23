/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED1_H_
#define __LED1_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Macros for Everyone--------------------------------------------------------*/




/*Function definitions---------------------------------------------------------*/

void init_LED_Red(void);
void init_LED_Green(void);
void init_LED_Blue(void);

void toggle_LED1(void);
void init_INPUT(void);
uint32_t read_INPUT(void);
void LED_state_set(uint32_t input);

void set_LED_Red(void);
void clear_LED_Red(void);
void set_LED_Green(void);
void clear_LED_Green(void);
void set_LED_Blue(void);
void clear_LED_Blue(void);


#ifdef __cplusplus
}
#endif

#endif /*__LED1_H */