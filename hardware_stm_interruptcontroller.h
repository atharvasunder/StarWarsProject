/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef __HARDWARE_STM_INTERRUPTCONTROLLER_H_
#define __HARDWARE_STM_INTERRUPTCONTROLLER_H_

/*used for enabling the header file to be compatible 
with C++ compilation along with C compilation*/
#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"


/*Function definitions---------------------------------------------------------*/
void enableNVIC_Timer3(void);

/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */