/**
  ******************************************************************************
  * @file    led_events.c 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Contains functions to manipulate and service the events queue
  ******************************************************************************
*/

#include "hardware_stm_gpio.h"
#include "gummy_led_utils.h"
#include "stdint.h"
#include "hardware_stm_interruptcontroller.h"
#include "hardware_stm_adc.h"

/************************************
* Initializes LED1 on the nucleo Board which is connected to Port B Pin 0
*************************************/
void init_LED_Red(void)
{
    // Call from hardware_stm_gpio
    initGpioBxAsOutput(0);

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

void init_LED_Green(void)
{
    // Call from hardware_stm_gpio
    initGpioBxAsOutput(1);

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

void init_LED_Blue(void)
{
    // Call from hardware_stm_gpio
    initGpioBxAsOutput(2);

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

void init_LED_Yellow(void)
{
    initGpioBxAsOutput(12);

}

/************************************
* Toggles LED1 
*************************************/
void toggle_LED1( void )
{
    // Call from hardware_stm_gpio
    toggleGPIOBx(0);
}

/************************************
* SET LED1 
*************************************/
void set_LED_Red( void )
{
    // Call from hardware_stm_gpio
    setGPIOBx(0);
}

void set_LED_Green( void )
{
    // Call from hardware_stm_gpio
    setGPIOBx(1);
}

void set_LED_Blue( void )
{
    // Call from hardware_stm_gpio
    setGPIOBx(2);
}

void set_LED_Yellow( void )
{
    // Call from hardware_stm_gpio
    setGPIOBx(12);
}
/************************************
* CLEAR LED1 
*************************************/
void clear_LED_Red( void )
{
    // Call from hardware_stm_gpio
    clearGPIOBx(0);
}

void clear_LED_Green( void )
{
    // Call from hardware_stm_gpio
    clearGPIOBx(1);
}

void clear_LED_Blue( void )
{
    // Call from hardware_stm_gpio
    clearGPIOBx(2);
}

void clear_LED_Yellow( void )
{
    // Call from hardware_stm_gpio
    clearGPIOBx(12);
}

/* Vibration Motor Stuff */
void init_vibration_motor( void ) {
    initGpioBxAsOutput_PD(11);
}

void set_vibration_motor (void) {
    setGPIOBx(11);
}

void clear_vibration_motor (void) {
    clearGPIOBx(11);
}


/* Initialize PF4 as anlog - Phototransistor */
void init_phototransistor() {
    initGpioFxAsAnalog(4);
    initADC3();
}

/* Read Phototransistor value */
uint16_t read_phototransistor() {
    return converted_phototransistor(readADC3Channel(14));
}

/* convert phototransistor to 0-100 scale */
uint8_t converted_phototransistor(uint16_t photo) {
    uint8_t scaled_gummy = (photo/4095.0f) * 100.0f;
    return scaled_gummy;
}


/*
INITIALIZES PIN C6 AS INPUT
*/
void init_gummy_Input(void)
{
    // initGpioCxAsInput_PU(6);
    enableEXTI6OnPortC();
}

/*
CHECK STATE OF PIN 6
*/
uint32_t read_INPUT(void)
{
    return checkGPIOCx(6);
}

/*
turn on or off the LED based on input at pin C6
*/
void LED_state_set(uint32_t input)
{
    if (input == 1){
        set_LED_Red();
    }   

    else if (input == 0){
        clear_LED_Red();
    }

}

// takes converted gummy phototransistor reading then returns corresponding lightsaber color
uint16_t gummy_to_saber(uint16_t *array, uint8_t len) {
    
    // array [red, blue, green, yellow
    if (array[0] < 10 && array[1] > 90) {
        printf("color is %d\n", 1);
        return 1; // this is red
    } else if (array[0] < 60 && array[0] > 40) {
        printf("color is %d\n", 2);
        return 2; // this is green
    } else if (array[0] < 35 && array[1] < 80) {
        printf("color is %d\n", 4);
        return 4; // this is white

    }
    /* write else if for blue */ 

    else {
        printf("no gummy\n");
        return 0; // no color detected - return to idle
    }

    // red gummy
    // red: 5  |  blue: 95 | green: 98 | yellow: 89

    // green gummy
    // red: 58 |  blue: 79 | green: 96 | yellow: 91

    // yellow gummy
    // red: 6  |  blue: 89 | green: 81 | yellow: 80

    // blue gummy
    // red:    |  blue:  | green:  |  yellow:  

    // white gummy
    // red: 14 |  blue: 63 | green: 83 | yellow: 81

}

