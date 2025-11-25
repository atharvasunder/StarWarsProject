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
#include "gummy_led_utils.h"
#include "hardware_stm_interruptcontroller.h"
#include "state_machine.h"
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
uint8_t led1_flag = 0; // red
uint8_t led2_flag = 0; // green
uint8_t led3_flag = 0; // yellow 
uint8_t led4_flag = 0; // blue

// for gummy color classification
uint16_t gummy_responses[4];
uint16_t gummy_color;

// for turning on saber blade
uint8_t  saber_start_flag = 0; //
uint16_t led_on_count     = 0;  // for counting how many leds have turned on
uint8_t  saber_init_flag  = 0;  // whether initialized or not

void init_state_machine(void) {

    /* PLEASE COMMENT EXACTLY WHAT PIN, TIMER, ADC ETC YOU ARE INITIALIZING */

    /* GENERAL */

        // initialize and start timers

    /* GENERAL */


    /* HILT */

    // initialize pins for gummy LEDs and phototransistor circuit output as input to nucleo
    // maybe an ADC here if we are not going to use a schmitt trigger?
        // initialize LEDs
        init_LED_Blue();
        init_LED_Red();
        init_LED_Green();
        init_LED_Yellow();

        // init phototransistor
        init_phototransistor();

        // initialize pins for button (including the external interrupt)
        init_gummy_Input();
        

        // initialize pins, ADCs for accelerometer (don't need to start the ADC yet?)

        // initialize hilt speaker

    /* HILT */

    /* BLADE */
        // initialize pins for LED strip


    /* BLADE */


    /* TARGET BOARD */

        // initialize pins for target soundtrack speaker

        // initialize target motor                           Darth Vader

        // initialize target motor encoder                   Darth Vader

        // initialize target timer motor                     XX

        // initialize target timer encoder                   XX

        // initialize target timer motor                     X

        // initialize target timer encoder                   X

    /* TARGET BOARD */

    current_state.type = IDLE;
}

void state_machine(event newevent){

    switch (current_state.type){
        case IDLE:
            
            if (idle_start_flag == 0){
                // turn on LED strip bottom light

                // start timeout for LED strip
                // START_TIMEOUT: event name
                // 1: device that requires the delay, 1: led_strip, 2: speaker, 3: gummy LED
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

                init_gummy_flags();

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
            
            if (saber_init_flag == 0) {
                // turn on LED1
                set_LED_Red();

                // play speaker sound (beep indicating one led is on)

                // keep LED on for 0.5s
                enqueue_event(START_TIMEOUT, 3, 500);

                saber_init_flag = 1;
                led1_flag       = 1;

            } 

            // delay for .5 s
            if (newevent.type == START_TIMEOUT) {
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }
            

            else if (newevent.type == TIMEOUT) {
                if (saber_init_flag == 1) { // started
                    // LED1 done 
                    gummy_responses[0] = read_phototransistor();
                    clear_LED_Red();

                    set_LED_Blue();
                    enqueue_event(START_TIMEOUT, 3, 500);

                    saber_init_flag = 2;
                    led2_flag       = 1;

                }

            }
            
            (saber_init_flag == 1 && led1_flag == 1) {
                // read red phototransistor
                gummy_responses[0] = read_phototransistor();

                // clear red LED
                clear_LED_Red();

                // turn on LED2
                set_LED_Blue();

                // play speaker sound

                // keep blue LED on for 0.5s
                enqueue_event(START_TIMEOUT, 3, 500);

                led2_flag = 1;

            } else if (saber_init_flag == 1 && led2_flag == 1) {
                // read blue phototransistor
                gummy_responses[1] = read_phototransistor();

                // clear blue LED
                clear_LED_Blue();

                // turn on green LED
                set_LED_Blue();
                
                // play speaker sound

                // keep green LED on for 0.5s
                enqueue_event(START_TIMEOUT, 3, 500);

                led3_flag = 1;

            } else if (saber_init_flag == 1 && led3_flag == 1) {
                // read green phototransistor
                gummy_responses[2] = read_phototransistor();

                // clear green LED
                clear_LED_Green();

                // turn on yellow LED
                set_LED_Yellow();

                // play speaker sound

                // keep yellow LED on for 0.5s
                enqueue_event(START_TIMEOUT, 3, 500);


                led1_flag          = 0;
                led2_flag          = 0;
                led3_flag          = 0;
                saber_init_flag    = 0;
                // led4_flag       = 1;

                enqueue_event(COLOUR_DETECTED, 1, 1);
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

                if (gummy_responses == {0, 0, 1, 0}){

                    // read input from phototransistor circuit, store in an array

                    // turn off led1

                    // turn on led2

                    // play speaker beep

                    // start time pause

                    led2_flag = 1;
                }

                else if(gummy_responses == {} ){
                    
                    // read input from phototransistor circuit, store in an array

                    // turn off led2

                    // turn on led3

                    // play speaker beep

                    // start time pause

                    led3_flag = 1;
                }

                else if(gummy_responses == ){
                    
                    // read input from phototransistor circuit, store in an array

                    // turn off led3

                    // turn on led4

                    // play speaker beep

                    // start time pause

                    led4_flag = 1;
                }

                else if (gummy_responses){
                        
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