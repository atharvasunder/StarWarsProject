/**
  ******************************************************************************
  * @file    main.cpp 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Main file for star wars light saber gamep hello i m kor
  ******************************************************************************
*/

#include "main.h"
#include "debug_mort.h"
#include "events.h"
#include "gummy_led_utils.h"
#include "state_machine.h"
#include "global_time.h"
#include "hardware_stm_interruptcontroller.h"
#include <cstdint>

/* TEMPORARY */
// for debug printing
#include "mbed.h"
Serial pc(USBTX, USBRX);
/* TEMPORARY */

// #define buttontest
#define mama

#ifdef buttontest
int main (void)
{
    init_gummy_Input();
    while(1) 
    {
        int16_t reader = read_INPUT();
        pc.printf("what is the value, %d\n", reader);
    }
}

#endif

#ifdef mama
int main (void)
{
    /*initialize the event queue and state machine*/
    init_event_queue();
    init_global_timer();
    init_state_machine();

    /*Keep servicing event queue and checking for timeouts*/
    while(1){
        timeoutCheck();
        service_event_queue();
        pc.printf("%d1 \n", current_time_ms());
        
    }
}
#endif