/**
  ******************************************************************************
  * @file    main.cpp 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet) hello
  * @version 1.0
  * @date    November-2025
  * @brief   Main file for star wars light saber gamep
  ******************************************************************************
*/

#include "main.h"
#include "debug_mort.h"
#include "events.h"
#include "state_machine.h"
#include "global_time.h"
#include <cstdint>

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
    }
}