/**
  ******************************************************************************
  * @file    led_state_machine.c 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Contains functions for reasoning about state transitions
  ******************************************************************************
*/

#include "debug_mort.h"
#include "events.h"
#include "gummy_led_utils.h"
#include "led_strip_utils.h"
#include "hardware_stm_interruptcontroller.h"
#include "state_machine.h"
#include "stdint.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer3.h"
#include "global_time.h"
#include "hardware_stm_timer2.h"
#include "audio.h"
#include <stdbool.h>

// Define the global variables
state current_state;
float led_response[3]; // for storing inputs from light detection circuit.

// define flags for start of states
uint8_t idle_start_flag = 0;
uint8_t in_game_flag = 0;

// for gummy bear LEDs (can turn them on after detecting light from them)
uint8_t led1_flag = 0; // red
uint8_t led2_flag = 0; // green
uint8_t led3_flag = 0; // yellow 
uint8_t led4_flag = 0; // blue

// for gummy color classification
uint16_t gummy_responses[4];
uint16_t gummy_color;

// for turning on saber blade
uint8_t  saber_start_flag = 0; // for initializing saber turning on
uint8_t saber_stop_flag = 0;    // for initializing saber turning off
uint16_t led_on_count = 0;  // for counting how many leds have turned on
uint16_t led_off_count = NUM_OF_LEDS;
uint8_t  saber_init_flag  = 0;  // whether initialized or not

// initialize strip leds array
neopixel_led leds[NUM_OF_LEDS];

// led strip idle colour
rgb_color strip_color;

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
        init_speaker();

        // initialize vibration motor 
        init_vibration_motor();

    /* HILT */

    /* BLADE */
        // initialize pins for LED strip
        initGpioBxAsAF2(5); // PB5 as alternative function 2 for PWM
        initTimer3PWM(8, 13);   // 8: PSC, 13: ARR

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

                saber_init_flag = 0;    // for led cycling to happen everytime from idle state
                in_game_flag = 0;
                
                strip_color.r = 0;
                strip_color.g = 0;
                strip_color.b = 255;

                // turn on LED strip bottom light
                set_n_leds(&strip_color, leds, 2);

                // start timeout for LED strip
                // START_TIMEOUT: event name
                // 1: device that requires the delay, 1: led_strip, 2: speaker, 3: gummy LED
                // 1000: delay duration in milli seconds     
                // enqueue_event(START_TIMEOUT, 1, 1000);   // enque timeout request

                // turn on speaker (start the music)
                resetMusicCounter();
                uint16_t duration_to_wait = playMainTheme();

                // start timeout for speaker
                enqueue_event(START_TIMEOUT, 2, duration_to_wait);   // enque timeout request, fill up delay duration 
                // @ korell need to choose a delay duration based on the music

                // set the flag
                idle_start_flag = 1;
            }

            if (newevent.type == BUTTON_PRESSED){

                current_state.type = SABER_INITIALIZE;

                // turn off led strip (don't want led strip to affect phototransistor readings)
                reset_all_leds(leds);

                //turn off chill intro music
                stopAudio();
                resetMusicCounter();

                // clear the timeout list before entering new state to avoid 
                // unfinished timeouts when a button is pressed
                cleartimeoutlist();

                enqueue_event(START_TIMEOUT, 3, 500);
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
                    uint16_t duration_to_wait = playMainTheme();

                    // debugprintHelloWorld();

                    // start timeout for speaker
                    enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                }  
            }   

            break;


        case SABER_INITIALIZE:  // for cycling through leds and determining gummy bear colour
            
            if (saber_init_flag == 0) {
                // turn on LED1
                set_LED_Red();

                // play speaker sound (beep indicating one led is on)
                resetMusicCounter();
                uint16_t duration_to_wait = playLED_Scan();
                enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                // keep LED on for 0.5s
                enqueue_event(START_TIMEOUT, 3, 1000);

                saber_init_flag = 1;
                led1_flag = 1;

            } 

            // delay for .5 s
            if (newevent.type == START_TIMEOUT) {
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == COLOUR_DETECTED){
                current_state.type = SABER_TURN_ON;
            }

            else if (newevent.type == NO_COLOUR_DETECTED){
                current_state.type = IDLE;
                idle_start_flag = 0;
                // clear the event queue when entering idle state
            }
            
            else if (newevent.type == TIMEOUT) {

                if (newevent.param1 == 3) { 
                    if (saber_init_flag == 1) { // started
                        // LED1 done 
                        gummy_responses[0] = read_phototransistor();
                        clear_LED_Red();

                        set_LED_Blue();
                        enqueue_event(START_TIMEOUT, 3, 1000);

                        uint16_t duration_to_wait = playLED_Scan();
                        enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                        saber_init_flag = 2;
                    }

                    else if (saber_init_flag == 2) {
                        // LED2 done
                        gummy_responses[1] = read_phototransistor();
                        clear_LED_Blue();

                        set_LED_Green();
                        enqueue_event(START_TIMEOUT, 3, 1000);

                        uint16_t duration_to_wait = playLED_Scan();
                        enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                        saber_init_flag = 3;
                    }

                    else if (saber_init_flag == 3) {
                        gummy_responses[2] = read_phototransistor();
                        clear_LED_Green();

                        set_LED_Yellow();
                        enqueue_event(START_TIMEOUT, 3, 1000);

                        uint16_t duration_to_wait = playLED_Scan();
                        enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                        saber_init_flag = 4;
                    }

                    else if (saber_init_flag == 4) {
                        gummy_responses[3] = read_phototransistor();
                        clear_LED_Yellow();

                        saber_init_flag = 5;    // ensures this condition does not get visited again
                        led_on_count = 0;

                        for (uint8_t i = 0; i < 4; i++) {
                            printf("\n%d", gummy_responses[i]);
                            }

                        gummy_color = gummy_to_saber(gummy_responses, 4);

                        if (gummy_color != 0) {
                            // gummy detected
                            enqueue_event(COLOUR_DETECTED, gummy_color, 0);
                        }
                        
                        else {
                            // no color detected
                            enqueue_event(NO_COLOUR_DETECTED, gummy_color, 0);
                        }
                    }

                }

                if (newevent.param1 == 2){
                    stopAudio();
                }

            }

            break;

        case SABER_TURN_ON:

            if (saber_start_flag == 0){
                
                get_strip_colour(gummy_color);  // updates the global variable strip_color with the required gummy color
                // turn on first led in strip, update led_count variable by 1
                set_n_leds(&strip_color, leds, led_on_count);
                
                // start led_strip timeout request
                enqueue_event(START_TIMEOUT, 1, 5);

                // start playing sound
                resetMusicCounter(); 
                uint16_t duration_to_wait = playLightsaberEffect();

                // start speaker timeout request
                enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                saber_start_flag = 1;
                led_on_count ++;

            }

            if (newevent.type == SABER_ON){
                current_state.type = SABER_READY;

                saber_start_flag = 0;

                // clear the timeout list before entering new state to avoid 
                // unfinished timeouts when a new state is entered
                cleartimeoutlist();
            }

            else if (newevent.type == START_TIMEOUT){
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == TIMEOUT){
                if (newevent.param1 ==  1){ // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    
                    // if number of leds on < 60, turn on the required number of leds
                    // else, dont do anything
                    if (led_on_count <= NUM_OF_LEDS){
                        led_on_count++;
                        set_n_leds(&strip_color, leds, led_on_count);
                        // start a new timeout - keep blade on
                        enqueue_event(START_TIMEOUT, 1, 5);
                    }

                }

                else if (newevent.param1 ==  2){   // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                
                    // enqueue_event(START_TIMEOUT, 2, 1000);

                    if (led_on_count <= NUM_OF_LEDS){
                        // play speaker 
                        uint16_t duration_to_wait = playLightsaberEffect();

                        // start speaker timeout request
                        enqueue_event(START_TIMEOUT, 2, duration_to_wait);
                    }

                    else{
                        stopAudio(); //turn off lightsaber audio after
                        resetMusicCounter();
                        
                        enqueue_event(SABER_ON, 0, 0);  // params do not matter here
                    }

                }  
            }   
            break;

        case SABER_TURN_OFF: // for light saber retracting

            if (saber_stop_flag == 0){
            
                // turn on all leds in strip, decreasae led_count variable by 1
                set_n_leds(&strip_color, leds, led_off_count);
                
                // start led_strip timeout request
                enqueue_event(START_TIMEOUT, 1, 5);

                // start playing sound
                resetMusicCounter(); 
                uint16_t duration_to_wait = playSABER_off();

                // start speaker timeout request
                enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                saber_stop_flag = 1;
                led_off_count --;

            }

            if (newevent.type == SABER_OFF){
                current_state.type = IDLE;

                saber_stop_flag = 0;

                // clear the timeout list before entering new state to avoid 
                // unfinished timeouts when a new state is entered
                cleartimeoutlist();

            }

            else if (newevent.type == START_TIMEOUT){
                insertDelayToList(newevent.param1, newevent.param2, current_time_ms());
            }

            else if (newevent.type == TIMEOUT){
                if (newevent.param1 ==  1){ // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                    
                    // if number of leds on >=1, turn on the required number of leds
                    // else, dont do anything
                    if (led_off_count >= 1){
                        led_off_count--;
                        set_n_leds(&strip_color, leds, led_off_count);
                        // start a new timeout - keep blade on
                        enqueue_event(START_TIMEOUT, 1, 5);
                    }

                }

                else if (newevent.param1 ==  2){   // param1 = 1 denotes the timeout is for the led strip, param1 = 2: for speaker
                
                    // enqueue_event(START_TIMEOUT, 2, 1000);

                    if (led_off_count >= 1){
                        // play speaker 
                        uint16_t duration_to_wait = playSABER_off();

                        // start speaker timeout request
                        enqueue_event(START_TIMEOUT, 2, duration_to_wait);
                    }

                    else{
                        stopAudio(); //turn off lightsaber audio after
                        resetMusicCounter();
                        enqueue_event(SABER_OFF, 0, 0);  // params do not matter here
                    }

                }  
            }   
            break;

        case SABER_READY:  
            if (newevent.type == BUTTON_PRESSED){
                current_state.type = IN_GAME_WAITING;

                // clear the timeout list before entering new state to avoid 
                // unfinished timeouts when a new state is entered
                cleartimeoutlist();
            }

            break;

        case IN_GAME_WAITING:  
            if (newevent.type == BUTTON_PRESSED){
                current_state.type = SABER_TURN_OFF;

                saber_start_flag = 0;
                led_on_count = 0;
                led_off_count = 144;
                idle_start_flag = 0;

                // clear the timeout list before entering new state to avoid 
                // unfinished timeouts when a new state is entered
                cleartimeoutlist();
            }

            if (in_game_flag == 0){

                // turn on speaker (start imperial march)
                resetMusicCounter();
                uint16_t duration_to_wait = playImperialMarch();

                // start timeout for speaker
                enqueue_event(START_TIMEOUT, 2, duration_to_wait);   // enque timeout request, fill up delay duration 

                // set the flag
                in_game_flag = 1;
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
                    uint16_t duration_to_wait = playImperialMarch();

                    // debugprintHelloWorld();

                    // start timeout for speaker
                    enqueue_event(START_TIMEOUT, 2, duration_to_wait);

                }  
            }
            break;
    }

}

void get_strip_colour(uint16_t gummy_color){
    if (gummy_color == 0){
        strip_color.r = 255;
        strip_color.g = 0;
        strip_color.b = 0;
    }

    else if (gummy_color == 1){
        strip_color.r = 0;
        strip_color.g = 255;
        strip_color.b = 0;
    }

    else if (gummy_color == 2){
        strip_color.r = 0;
        strip_color.g = 0;
        strip_color.b = 255;
    }

    else if (gummy_color == 3){
        strip_color.r = 100;
        strip_color.g = 100;
        strip_color.b = 0;
    }
}
