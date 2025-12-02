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
    

    // red gummy
    // red: 6   |  blue: 94 | green: 99 | yellow: 76

    // green gummy
    // red: 80  |  blue: 95 | green: 76 | yellow: 89

    // blue gummy
    // red: 86  |  blue: 30 | green: 66 | yellow: 90

    // purple
    // red: 79  |  blue: 67 | green: 91 | yellow: 83

    // array [red, blue, green, yellow]
    if (array[0] < 30 && array[1] > 80) {
        printf("color is %d\n", 1);
        return 1; // this is red
    } else if (array[1] > 80 && array[2] < 90) {
        printf("color is %d\n", 2);
        return 2; // this is green
    } else if (array[1] < 60 && array[0] > 65) {
        printf("color is %d\n", 3);
        return 3; // this is blue
    // } else if (array[1] < 80 && array[2] > 80) {
    //     // extremely similar to blue
    //     printf("color is %d\n - Welcome home, Master Windu", 3);
    //     return 4; // this is purple
    // }
    } else if (array[0] < 50 && array[1] < 50) {
        return 0; // empty
    }

    else {
        printf("no gummy\n");
        return 0; // no color detected - return to idle
    }

}

