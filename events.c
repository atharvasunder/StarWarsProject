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

// Define the global variable
event_queue game_queue;

/*Below are functions for performing operations on a circular queue.
The operations include: enque, deque, is_empty, is_full.

The queue is implemented such that the start index is the top element and the end index is the
bottom element. 
*/

void init_event_queue(void) {
    game_queue.start_index = -1;
    game_queue.end_index = -1;
}

uint16_t get_event_queue_length(void) 
{
    if (queue_is_empty()) {
        return 0;
    }

    if (game_queue.end_index >= game_queue.start_index) {
        // No wrap-around: [start ... end]
        return (uint16_t)(game_queue.end_index - game_queue.start_index + 1);
    } else {
        // Wrapped: [start ... MAX-1] and [0 ... end]
        return (uint16_t)((MAX_EVENT_QUEUE_SIZE - game_queue.start_index) +
                          (game_queue.end_index + 1));
    }
}


void enqueue_event(event_type t, uint16_t param1, double param2) {
    
    event e;
    e.type = t;
    e.param1 = param1;
    e.param2 = param2;
    
    if (queue_is_full() == true){   // do not populate queue if full
        debugprintFullQueue();
        return;
    }

    else if (queue_is_empty()){     // initialize start index to 0 if queue is initially empty
        game_queue.start_index = game_queue.start_index + 1;
    }

    game_queue.end_index = (game_queue.end_index + 1) % MAX_EVENT_QUEUE_SIZE;
    game_queue.queue[game_queue.end_index] = e;
}

event dequeue_event(void) {
    if (queue_is_empty()){
        // debugprintEmptyQueue();
        event e;
        e.type = NO_EVENT;

        return e;
    }

    // if the queue has only 1 element, reset indices to empty queue state
    if (game_queue.start_index == game_queue.end_index){
        event e = game_queue.queue[game_queue.start_index];
        game_queue.start_index = game_queue.end_index = -1;

        return e;
    }

    else{
        event e = game_queue.queue[game_queue.start_index];
        game_queue.start_index = (game_queue.start_index + 1) % MAX_EVENT_QUEUE_SIZE;

        return e;
    }

}

bool queue_is_full(void){
    if ((game_queue.end_index + 1) % MAX_EVENT_QUEUE_SIZE == game_queue.start_index){
        return true;
    }

    else{
        return false;
    }
}

bool queue_is_empty(void){
    if (game_queue.start_index == -1){
        return true;
    }

    else{
        return false;
    }
}

void service_event_queue(void){

    event current_event = dequeue_event();

    // pass event into state machine that reasons for actions based on the current state and event
    state_machine(current_event);

}