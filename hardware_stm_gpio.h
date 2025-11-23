/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef __HARDWARE_STM_GPIO_H_
#define __HARDWARE_STM_GPIO_H_

/*used for enabling the header file to be compatible 
with C++ compilation along with C compilation*/
#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"


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
void initGpioB0AsOutput( void );
void initGpioB1AsOutput( void );
void initGpioB2AsOutput( void );
void initGpioB0AsAlternate2( void );
void toggleGPIOB0( void );
void setGPIOB0( void );
void clearGPIOB0( void );
void setGPIOB1( void );
void clearGPIOB1( void );
void setGPIOB2( void );
void clearGPIOB2( void );
void initGpioC6AsInput( void );
uint32_t checkGPIOC6(void);
uint32_t checkGPIOB0(void);


/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */