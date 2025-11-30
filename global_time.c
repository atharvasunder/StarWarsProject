/**
  ***************************************************************************************************************************
  * @file    global_timer.c 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Contains global timer, a list to manage time delay requests, and a function to check for timeout events
  ***************************************************************************************************************************
*/

#include "stdint.h"
#include "debug_mort.h"
#include "global_time.h"
#include "events.h"
#include "hardware_stm_timer4.h"
#include <cstdint>

// define global variables
uint32_t global_time;   // stores timer count in mills seconds (ms)
DelayRequestList delay_list;    // stores time delay objects

// define functions for global time and game time
uint32_t current_time_ms(void){
    return global_time;
}

void increment_time(void){
    global_time ++;
}

void init_global_timer(void){
    initTimer4ToInterrupt(89,999);  // START THE GLOBAL TIMER (TIMER4) with PSC 89, ARR 999 to get a update every millisecond
}

// To add a delay object at the end of the list 
void insertDelayToList(uint16_t timernumber,
                           double timeouttime,
                           double t0_ms)
{
    if (delay_list.sizeoflist >= MAX_TIME_DELAY_REQUESTS) {
        // list full
        return;
    }

    uint16_t idx = delay_list.sizeoflist;   // index to write to (one after the last element)

    delay_list.list[idx].timernumber = timernumber;
    delay_list.list[idx].timeouttime = timeouttime;
    delay_list.list[idx].t0          = t0_ms;

    delay_list.sizeoflist++;
    delay_list.startoflist = 0;
}

// To delete delay object at given index (0 <= index < sizeoflist)
void deleteDelayTimerFromList(uint16_t index)
{
    if (index >= delay_list.sizeoflist) {
        // invalid index, nothing to delete
        return;
    }

    // Shift everything after 'index' left by one
    // if index = size-1, the loop does not even execute
    for (uint16_t i = index; i < delay_list.sizeoflist - 1; i++) {
        delay_list.list[i] = delay_list.list[i + 1];
    }

    if (delay_list.sizeoflist > 0) {
        delay_list.sizeoflist--;
    }

    delay_list.startoflist = 0; // this always stays zero
}

double absoluteVal_double(double x)
{
    if (x < 0.0)
        return -x;
    else
        return x;
}

void timeoutCheck(void)
{
    double timeouttime;
    double t0, t1;
    uint16_t index, timernumber;

    index = delay_list.startoflist; // (always zero for us)

    // loop through list of delay reqauest data structures
    // check all of them if they have timed out
    // if any of them timed out, create a timeout event 
    // marking the end of that request 
    for (int i = 0; i < delay_list.sizeoflist; i++)
    {

        timeouttime = delay_list.list[index].timeouttime;
        t0 = delay_list.list[index].t0;
        timernumber = delay_list.list[index].timernumber;

        t1 = current_time_ms();

        if (absoluteVal_double(t1 - t0) > timeouttime)
        {
            deleteDelayTimerFromList(i);

            // create a timeout event and insert it into the event queue
            enqueue_event(TIMEOUT, timernumber, timeouttime);

            return;  // only handle one timeout per call
        }

        index = index + 1;
    }
}

void cleartimeoutlist(void)
{

    for (int i = 0; i < delay_list.sizeoflist; i++)
    {
        deleteDelayTimerFromList(i);

    }
}