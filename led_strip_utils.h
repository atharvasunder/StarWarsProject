/* Define to prevent recursive inclusion 
this defines a name for the header file once it is included
so that multiple includes are prevented. (the preprocessor would
check if this name is defined, and if yes, the headers contents
would not be pasted into the file to be compiled for a second time.)

Overall, this is called a header guard and it is present at the start of
every header file. It is basically an if condition for the preprocessor
that ends with #endif
-------------------------------------*/
#ifndef __LED_UTILS_H_
#define __LED_UTILS_H_

/*used for enabling the header file to be compatible 
with C++ compilation along with C compilation*/
#ifdef __cplusplus
 extern "C" {
#endif

/*  ------------------------------------------------------------------
Contains microcontroller specific macros and function definitions for use in this header. */
#include "main.h"

/*----------------------------Structure decelerations--------------------------*/

// structure to store an RGB color (e.g. r = 0, g = 255, b = 0)
typedef struct
{
    uint8_t g;
    uint8_t r;
    uint8_t b;
} rgb_color;

// structure for one NeoPixel LED (stores pwm duty cycle values (logical zero and ones))
typedef struct
{
    uint16_t g[8];
    uint16_t r[8];
    uint16_t b[8];
} neopixel_led;

/* MACRO definitions----------------------------------------------------------*/

// 1.25 us - period
// logical 1 - 0.85 us high
// logical 0 - 0.4 us high
// system frequency = 90 MHz, prescaler = 8 -> timer frequency (counts/s) = 10 MHz
// Auto-reload value = 13 -> pwm time period = 1.3 us 
// (0.05 micro second error from datasheet which within safe error range of +-0.6 us)
// CCR = 9 -> 0.8 us high
// CCR = 4  -> 0.4 us high

#define LED_LOGICAL_ONE  9
#define LED_LOGICAL_ZERO 4

#define NUM_OF_LEDS 144
#define COLOR_SET_CYCLES 60        // conservative number of pwm cycles that make up 75 micro seconds (= 75/1.3)

#define MESSAGE_LENGTH (NUM_OF_LEDS*24 + COLOR_SET_CYCLES)


/*Function definitions---------------------------------------------------------*/
void reset_all_leds(neopixel_led* leds);
void set_all_leds(neopixel_led* leds, rgb_color* strip_color);
void get_led_message(neopixel_led* leds);
uint16_t* get_led_message_address(void);
void delay(void);
void encode_byte(uint8_t byte, uint16_t out[8]);
void convert_rgb_to_neopixel(rgb_color *in, neopixel_led *out);
void set_n_leds(rgb_color* strip_color, neopixel_led* leds, uint16_t n);
void delay_long(void);
void init_led_strip(void);

/*end c++ compilation compatibility */
#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */