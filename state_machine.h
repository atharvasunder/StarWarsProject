/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

/*used for enabling the header file to be compatible 
with C++ compilation along with C compilation*/
#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"
#include <stdbool.h>
#include "events.h"

/* Macros/declerations related to states--------------------------------------------------------*/
typedef enum
{
    IDLE,
    SABER_INITIALIZE,
    SABER_READY,
    IN_GAME_WAITING
    // IN_GAME_PARRY,
    // END_OF_GAME
} state_type;


typedef struct state{
    state_type type;
} state;


/*Function definitions---------------------------------------------------------*/
void init_state_machine(void);
void state_machine(event currentevent);
void get_gummy_colour(float led_response[3]);

/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */
