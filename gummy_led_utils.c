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
#include "nucleo_led.h"
#include "stdint.h"

/************************************
* Initializes LED1 on the nucleo Board which is connected to Port B Pin 0
*************************************/
void init_LED_Red(void)
{
    // Call from hardware_stm_gpio
    initGpioB0AsOutput();

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

void init_LED_Green(void)
{
    // Call from hardware_stm_gpio
    initGpioB1AsOutput();

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

void init_LED_Blue(void)
{
    // Call from hardware_stm_gpio
    initGpioB2AsOutput();

    // initialize to be connected to timer3 channel3
    // initGpioB0AsAlternate2();
}

/************************************
* Toggles LED1 
*************************************/
void toggle_LED1( void )
{
    // Call from hardware_stm_gpio
    toggleGPIOB0();
}

/************************************
* SET LED1 
*************************************/
void set_LED_Red( void )
{
    // Call from hardware_stm_gpio
    setGPIOB0();
}

void set_LED_Green( void )
{
    // Call from hardware_stm_gpio
    setGPIOB1();
}

void set_LED_Blue( void )
{
    // Call from hardware_stm_gpio
    setGPIOB2();
}

/************************************
* CLEAR LED1 
*************************************/
void clear_LED_Red( void )
{
    // Call from hardware_stm_gpio
    clearGPIOB0();
}

void clear_LED_Green( void )
{
    // Call from hardware_stm_gpio
    clearGPIOB1();
}

void clear_LED_Blue( void )
{
    // Call from hardware_stm_gpio
    clearGPIOB2();
}



/*
INITIALIZES PIN C6 AS INPUT
*/
void init_INPUT(void)
{
    initGpioC6AsInput();
}

/*
CHECK STATE OF PIN 6
*/
uint32_t read_INPUT(void)
{
    return checkGPIOC6();
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