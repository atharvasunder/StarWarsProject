/**
  ******************************************************************************
  * @file    led_events.c 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Contains functions to manipulate and service the events queue
  ******************************************************************************
*/

#include "events.h"
#include "state_machine.h"
#include "stdint.h"
#include "debug_mort.h"
#include <stdbool.h>

void debugprintFullQueue(void)
{
    // TEMP: simple stub. You can make this actually print the queue later.
    // For example, if you have a debug printf:
    // debug_printf("Event queue is full!\n");
}

void debugprintHelloWorld( void )
{
    printf("I'm aliiiiiivveeee!!!\n");
} 