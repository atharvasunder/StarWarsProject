/**
  ******************************************************************************
  * @file    led_state_machine.c 
  * @author  Group 3
  * @version 1.0
  * @date    November-2025
  * @brief   Contains functions for reasoning about state transitions
  ******************************************************************************
*/

#include "events.h"
#include "state_machine.h"
#include "led_utils.h"
#include "hardware_stm_gpio.h"
#include "stdint.h"
#include "debug_mort.h"
#include "global_time.h"
#include <stdbool.h>

// Define the global variables
state current_state;
float led_response[3]; // for storing inputs from light detection circuit.

// define flags for start of states
uint8_t idle_start_flag = 0;

// for gummy bear LEDs (can turn them on after detecting light from them)
uint8_t led1_flag = 0;
uint8_t led2_flag = 0;
uint8_t led3_flag = 0;
uint8_t led4_flag = 0;

// for gummy color classification
uint16_t gummy_responses[4];
uint16_t gummy_color;

// for turning on saber blade
uint8_t saber_start_flag = 0;
uint16_t led_on_count = 0;  // for counting how many leds have turned on

void init_state_machine(void) {

    /* PLEASE COMMENT EXACTLY WHAT PIN, TIMER, ADC ETC YOU ARE INITIALIZING */

    // initialize pins for gummy LEDs and phototransistor circuit output as input to nucleo
    // maybe an ADC here if we are not going to use a schmitt trigger?

    // initialize pins for button (including the external interrupt)

    // initialize pins, ADCs for accelerometer (don't need to start the ADC yet?)

    // initialize pins for LED strip

    // initialize pins for speaker

    // initialize and start timers

    current_state.type = IDLE;
}

void state_machine(event newevent){

    switch (current_state.type){
        case IDLE:
            
            if (idle_start_flag == 0){
                // turn on LED strip bottom light

                // start timeout for LED strip
                // START_TIMEOUT: event name
                // 1: device that requires the delay, 1: led_strip, 2: speaker
                // 1000: delay duration in milli seconds     
                enqueue_event(START_TIMEOUT, 1, 1000);   // enque timeout request

                // turn on speaker (start the music)

                // start timeout for speaker
                enqueue_event(START_TIMEOUT, 2, 1000);   // enque timeout request
                // @ korell need to choose a delay duration based on the music

                // set the flag
                idle_start_flag = 1;
            }
            
            if (newevent.type == BUTTON_PRESSED){
                current_state.type = SABER_INITIALIZE;

                // turn off led strip (don't want led strip to affect phototransistor readings)

                //turn off chill intro music

                // reset the idle flag to zero
                idle_start_flag = 0;

            }

            else if (newevent.type == START_TIMEOUT){
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == TIMEOUT){
                if (newevent.param1 ==  1){ // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    // toggle strip (for blinking effect)

                    // start a new timeout
                    enqueue_event(START_TIMEOUT, 1, 1000);

                }

                else if (newevent.param1 ==  2){   // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    // play speaker (send 1 set of bits before the next timeout)

                    // start a new timeout
                    enqueue_event(START_TIMEOUT, 2, 1000);
                }  
            }   


        case SABER_INITIALIZE:  // for cycling through leds and determining gummy bear colour
            
            if (led1_flag == 0){
                // turn on LED1

                // play speaker sound (beep indicating one led is on)

                // start a delay (see idle state for how to do it, can keep the param1 = 3 to denote gummy led related timeout
                // but does not really matter here)

                led1_flag = 1;
            }
            
            if (newevent.type == COLOUR_DETECTED){  // can divide these events into individual colour events
                current_state.type = SABER_READY;
            }

            else if (newevent.type == START_TIMEOUT){
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == TIMEOUT){
                
                // here we use a common timeout event for the led strip and the speaker
                // because we want the speaker to blick as the individual gummy leds light up. 
                // That is why param1 does not really matter here

                if (led1_flag == 1 &&
                    led2_flag == 0 &&
                    led3_flag == 0 &&
                    led4_flag == 0){

                    // read input from phototransistor circuit, store in an array

                    // turn off led1

                    // turn on led2

                    // play speaker beep

                    // start time pause

                    led2_flag = 1;
                }

                else if(led1_flag == 1 &&
                        led2_flag == 1 &&
                        led3_flag == 0 &&
                        led4_flag == 0){
                    
                    // read input from phototransistor circuit, store in an array

                    // turn off led2

                    // turn on led3

                    // play speaker beep

                    // start time pause

                    led3_flag = 1;
                }

                else if(led1_flag == 1 &&
                        led2_flag == 1 &&
                        led3_flag == 1 &&
                        led4_flag == 0){
                    
                    // read input from phototransistor circuit, store in an array

                    // turn off led3

                    // turn on led4

                    // play speaker beep

                    // start time pause

                    led4_flag = 1;
                }

                else if (led1_flag == 1 &&
                        led2_flag == 1 &&
                        led3_flag == 1 &&
                        led4_flag == 1){
                        
                    // read input from phototransistor circuit, store in an array

                    // turn off led4

                    // reset flags
                    led1_flag = led2_flag = led3_flag = led4_flag = 0;

                    // compute gummy color using responses in the array
                    // store gummy color in the gummy_color variable (declared above)

                    // if gummy color is valid: enqueue color detected event
                    // else enqueue no color detected event
                    
                    }

            }

            else if (newevent.type == NO_COLOUR_DETECTED){
                current_state.type = IDLE;
            }

            // turn on each led successively followed by a manual delay

            // read each leds response, store it in an array or sth

            // pass array into a logic function to get the colour/no colour

            // enqueue resulting event into event queue

        case SABER_READY:

            if (saber_start_flag == 0){
                // turn on first led in strip, update led_count variable by 1

                // start led_strip timeout request
                enqueue_event(START_TIMEOUT, 1, 200);

                // start playing sound

                // start speaker timeout request

                saber_start_flag = 1;

            }

            if (newevent.type == BUTTON_PRESSED){
                current_state.type = IN_GAME_WAITING;

                saber_start_flag = 0;
            }

            else if (newevent.type == START_TIMEOUT){
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == TIMEOUT){
                if (newevent.param1 ==  1){ // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    // if number of leds on < 60, turn on the required number of leds
                    // else, dont do anything

                    // start a new timeout
                    enqueue_event(START_TIMEOUT, 1, 1000);

                }

                else if (newevent.param1 ==  2){   // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    // play speaker (send 1 set of bits before the next timeout)
                    // if number of leds >= 60, dont do anything, dont start another delay too

                    // start a new timeout
                    enqueue_event(START_TIMEOUT, 2, 1000);
                }  
            }   

        case IN_GAME_WAITING:  
            if (newevent.type == BUTTON_PRESSED){
                current_state.type = IDLE;

            // start reading accelerometer

            }
    }
}


// THIS IS MY LAB4 FUNCTION, YOU CAN DELETE THIS @DAVID
void get_gummy_colour(float led_response[3]){

    float red_LED_response = led_response[0];
    float green_led_response = led_response[1];
    float blue_led_response = led_response[2];

    uint16_t gummy = 5; // will stay 5 if none of the conditions are met

    if (red_LED_response == 1 && green_led_response == 1 && blue_led_response == 1){
        gummy = 3;  // colourless allows all colours
    }

    else if (red_LED_response == 1 && green_led_response == 1 && blue_led_response == 0){
        gummy = 2;  // orange does not alow blue colour
    }

    else if (red_LED_response == 0 && green_led_response == 1 && blue_led_response == 0){
        gummy = 1;  // green does not allow red and blue colour
    }

    else if (red_LED_response == 1 && green_led_response == 0 && blue_led_response == 0){
        gummy = 0;  // red does not allow green and blue colour
    }

    printgummycolour(gummy);

}