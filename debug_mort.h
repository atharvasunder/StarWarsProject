#ifndef __DEBUG_MORT_H_
#define __DEBUG_MORT_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/*Function definitions---------------------------------------------------------*/
void debugprint(uint16_t number);
void debugprint_2(uint32_t number);
void debugprintHelloWorld( void );
void debugprintFullQueue( void );
void debugprintEmptyQueue( void );
void printgummycolour(uint16_t gummy);
void printoneinput(void);
void printtwoinputs(void);
void printthreeinputs(void);
void printtgetoutputs(void);


#ifdef __cplusplus
}
#endif

#endif /*__DEBUG_MORT_H */
