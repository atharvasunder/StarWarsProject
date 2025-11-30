/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef EVENTS_H_
#define EVENTS_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"
#include <stdbool.h>

/* Macros related to events and event queues--------------------------------------------------------*/
typedef enum
{
    BUTTON_PRESSED,
    COLOUR_DETECTED,
    NO_COLOUR_DETECTED,
    SABER_ON,
    SABER_OFF,
    START_TIMEOUT,
    TIMEOUT,
    GO_TO_PARRYING,
    GO_TO_WAITING,
    END_GAME,
    CORRECT_SWING_DIRECTION,
    WRONG_SWING_DIRECTION,
    GAME_OVER,
    NO_EVENT
} event_type;

// data structure to represent an event
typedef struct event{    // param1 ansd param2 are only relevent for start_timeout and timeout events
    event_type type;    
    uint16_t param1;    // to denote the device that requires the timeout
    double param2;      // to denote the duration of the timeout
} event;

#define MAX_EVENT_QUEUE_SIZE 100   // define max number of events that can be queued for service

typedef struct event_queue {
    event queue[MAX_EVENT_QUEUE_SIZE]; // array of event objects, this array is called queue here
    int16_t start_index; // where to read next
    int16_t end_index;   // where to write next
} event_queue;

/*Function definitions---------------------------------------------------------*/
void init_event_queue(void);
void service_event_queue(void);
bool queue_is_full(void);
bool queue_is_empty(void);
event dequeue_event(void);
void enqueue_event(event_type t, uint16_t param1, double param2);
uint16_t get_event_queue_length(void);

/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif 
