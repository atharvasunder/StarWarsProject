/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef GLOBAL_TIME_H
#define GLOBAL_TIME_H

/*used for enabling the header file to be compatible 
with C++ compilation along with C compilation*/
#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"
#include <stdbool.h>

/* Macros related to software timer and timer queues--------------------------------------------------------*/
typedef struct {
    double timeouttime; // duration of time delay
    double t0;  // start time of timeout
    uint16_t timernumber;   // one for each device that calls the timer (as different devices require different timeouts)
} Delay;

#define MAX_TIME_DELAY_REQUESTS 100   // define max no of time delay requests that can be on the list at a time

// data structure for storing delay requests
typedef struct {
    Delay list[MAX_TIME_DELAY_REQUESTS];  // array of active delay requests
    uint16_t sizeoflist;       // how many timers are active
    uint16_t startoflist;      // index of first timer
} DelayRequestList;

/*Function definitions---------------------------------------------------------*/
uint32_t current_time_ms(void);
void increment_time(void);
void init_global_timer(void);
void insertDelayToList(uint16_t timernumber, double timeouttime, double t0_ms);
void deleteDelayTimerFromList(uint16_t index);
double absoluteVal_double(double x);
void timeoutCheck(void);

/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif