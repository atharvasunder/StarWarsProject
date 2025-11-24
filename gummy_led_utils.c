/**
  ******************************************************************************
  * @file    nucleo_led.c 
  * @author  mortamar@andrew.cmu.edu
  * @version 1.0
  * @date    Septembr-2021
  * @brief   Controls the LED's on the nucleo board
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "gummy_led_utils.h"
#include "stdint.h"
#include "hardware_stm_interruptcontroller.h"

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
    initGpioBxAsOutput(3);

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
    setGPIOBx(3);
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
    clearGPIOBx(3);
}


/* Initialize PF4 as anlog - Phototransistor */
void init_phototransistor() {
    initGpioFxAsAnalog(4);
}

/* Read Photo transistor value */
uint16_t read_phototransistor() {
    return 0;
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