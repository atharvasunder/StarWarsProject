#include "main.h"
#include "debug_mort.h"
#include "led_utils.h"
#include "hardware_stm_timer3.h"
#include "hardware_stm_gpio.h"
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