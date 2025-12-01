/**
  ******************************************************************************
  * @file    led_utils.c 
  * @author  asramdas@andrew.cmu.edu
  * @version 1.0
  * @date    November 2025
  * @brief   Contains functions to control WS2812B Neopixel LED strip
  ******************************************************************************
*/

#include "led_strip_utils.h"
#include "debug_mort.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_dma_controller.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer3.h"
#include <cstdint>  /*C++ version of the <stdint.h> C header*/

/*------------------------------variable definitions-----------------------------*/
/*variable declerations*/
uint16_t led_message[MESSAGE_LENGTH];    // from here messages are sent through pwm
uint8_t strip_is_on = 0;
/* function definitions----------------------------------------------------------*/

uint16_t* get_led_message_address(void){
    return led_message;     // return a pointer to the first element of this array
}

// function to reset all LEDs
void reset_all_leds(neopixel_led* leds)
{
    for (int i = 0; i < NUM_OF_LEDS; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            (leds + i)->r[j] = LED_LOGICAL_ZERO;
            (leds + i)->g[j] = LED_LOGICAL_ZERO;
            (leds + i)->b[j] = LED_LOGICAL_ZERO;
        }
    }

    // convert message to an array
    get_led_message(leds);

    // begin varying duty cycle in such a way that the led color is set (to off here)
    enableDMAForTimer3Channel2();
    strip_is_on = 0;
}

// Convert one byte (0–255) into 8 logical zero/one entries
void encode_byte(uint8_t byte, uint16_t out[8])
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = (byte >> (7 - i)) & 0x01; // whatever bit is at the (7 - i)th place comes to the zeroth place
        
        if (bit)
            out[i] = LED_LOGICAL_ONE;
        else
            out[i] = LED_LOGICAL_ZERO;
        }
}

// Convert rgb_color to neopixel_led format
void convert_rgb_to_neopixel(rgb_color *in, neopixel_led *out)
{
    // WS2812 uses GRB order!
    encode_byte(in->g, out->g);
    encode_byte(in->r, out->r);
    encode_byte(in->b, out->b);
}

// function to set the same color to all LEDs
void set_all_leds(neopixel_led* leds, rgb_color* strip_color)
{
    for (int i = 0; i < NUM_OF_LEDS; i++)
    {
        convert_rgb_to_neopixel(strip_color, leds + i); // updates led structs in the array with desired color
    }

    // convert message to an array
    get_led_message(leds);

    // begin varying duty cycle in such a way that the led color is set
    enableDMAForTimer3Channel2();

    strip_is_on = 1;

}

void set_n_leds(rgb_color* strip_color, neopixel_led* leds, uint16_t n){
    
    for (int i = 0; i < n; i++)
    {
        convert_rgb_to_neopixel(strip_color, leds + i); // updates led structs in the array with desired color
    }

    for (int i = n; i < NUM_OF_LEDS; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            (leds + i)->r[j] = LED_LOGICAL_ZERO;
            (leds + i)->g[j] = LED_LOGICAL_ZERO;
            (leds + i)->b[j] = LED_LOGICAL_ZERO;
        }
    }

    // convert message to an array
    get_led_message(leds);

    // begin varying duty cycle in such a way that the led color is set
    enableDMAForTimer3Channel2();

    if (n > 0){
        strip_is_on = 1;
    }
    else{
        strip_is_on = 0;
    }
    
}

void get_led_message(neopixel_led* leds)
{
    int msg_idx = 0;

    // Flatten per-LED data into the message array
    for (int i = 0; i < NUM_OF_LEDS; i++)
    {
        // Order: G, R, B (each 8 bits)
        for (int bit = 0; bit < 8; bit++) {
            led_message[msg_idx++] = leds[i].g[bit];
        }
        for (int bit = 0; bit < 8; bit++) {
            led_message[msg_idx++] = leds[i].r[bit];
        }
        for (int bit = 0; bit < 8; bit++) {
            led_message[msg_idx++] = leds[i].b[bit];
        }
    }

    // Append reset period: line held low for COLOR_SET_CYCLES PWM periods
    for (int i = 0; i < COLOR_SET_CYCLES; i++) {
        led_message[msg_idx++] = 0;   // output compare value (in pwm mode) = 1 implies almost zero duty cycle
    }

    // debugprint(msg_idx);
}

void delay(void){
    // variables used to create time pauses
    uint32_t i, j, k;

    for (i = 0; i < 1000000; i++)
    {
        for (k = 0; k < 1000000; k++)
            j = j + 1;   // tiny delay
    }
}

void delay_long(void){
    // variables used to create time pauses
    uint32_t i, j, k;

    for (i = 0; i < 80000000; i++)
    {
        for (k = 0; k < 80000000; k++)
            j = j + 1;   // tiny delay
    }
}

void init_led_strip(void){
    initGpioBxAsAF2(5); // PB5 as alternative function 2 for PWM
    initTimer3PWM(8, 13);   // 8: PSC, 13: ARR
}

void toggle_led_strip(rgb_color* strip_color, neopixel_led* leds){
    if (strip_is_on == 1){
        reset_all_leds(leds);
    }

    else if (strip_is_on == 0){
        set_all_leds(leds, strip_color);
    }
}