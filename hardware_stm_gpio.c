/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  mortamar@andrew.cmu.edu
  * @version 1.0
  * @date    Septembr-2021
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>  /*C++ version of the <stdint.h> C header*/


//led 1 is connected to PB0. 
// GPIO B addresses: 0x4002 0400 - 0x4002 07FF
// GPIO C addresses: 0x4002 0800 - 0x4002 0BFF


/* MACRO definitions----------------------------------------------------------*/
//Port B addresses:
#define PORTB_BASE_ADDRESS ((uint32_t)0x40020400)        //The first address in memory corresponding to Port B (this is in the user manual!)
// I gave you the first one, now you fill in the rest, check in the user manual what is the offset from the base address for each register!
#define PORTB_MODER_REGISTER (PORTB_BASE_ADDRESS + 0x00) //replace the question mark with the correct offset!
#define PORTB_OTYPER_REGISTER (PORTB_BASE_ADDRESS + 0x04)
//#define PORTB_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)
#define PORTB_PUPDR_REGISTER (PORTB_BASE_ADDRESS + 0x0C)
#define PORTB_IDR_REGISTER (PORTB_BASE_ADDRESS + 0x10)
#define PORTB_ODR_REGISTER (PORTB_BASE_ADDRESS + 0x14)
//#define PORTB_BSRRL_REGISTER (PORTB_BASE_ADDRESS + 0x18)
//#define PORTB_BSRR_REGISTER (PORTB_BASE_ADDRESS + 0x18)
//#define PORTB_BSRRH_REGISTER (PORTB_BASE_ADDRESS + 0x1A)
//#define PORTB_LCKR_REGISTER (PORTB_BASE_ADDRESS + 0x1C)
#define PORTB_AFR1_REGISTER (PORTB_BASE_ADDRESS + 0x20)
//#define PORTB_AFR2_REGISTER (PORTB_BASE_ADDRESS + 0x24)
//#define PORTB_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)

//Port C addresses:
#define PORTC_BASE_ADDRESS ((uint32_t)0x40020800)
#define PORTC_MODER_REGISTER (PORTC_BASE_ADDRESS + 0x00)
// #define PORTC_OTYPER_REGISTER (PORTC_BASE_ADDRESS + 0x04)
#define PORTC_OSPEEDR_REGISTER (PORTC_BASE_ADDRESS + 0x08)
#define PORTC_PUPDR_REGISTER (PORTC_BASE_ADDRESS + 0x0C)
#define PORTC_IDR_REGISTER (PORTC_BASE_ADDRESS + 0x10)
// #define PORTC_ODR_REGISTER (PORTC_BASE_ADDRESS + 0x14)

// flags = bit masks (we have defined them as signed integers! need to cast them to unsigned
// before performing any bitwise operation, otherwise apparently there can be errors as
// inverting a signed positive integer gives a negative integer)
// we dont need to convert it for the or operation because there is no flipping bits involved 

//flags MODER Register:
#define GPIO_MODER_MODE0_ALTERNATE_Mask 0x02    // for pin 0 to be set to alternate mode
#define GPIO_MODER_MODE0_OUTPUT_Mask 0x01    // for pin 0 to be set to output mode
#define GPIO_MODER_MODE1_OUTPUT_Mask (uint32_t)(0x01 << 2)    // for pin 1 to be set to output mode
#define GPIO_MODER_MODE2_OUTPUT_Mask (uint32_t)(0x01 << 4)    // for pin 2 to be set to output mode
#define GPIO_MODER_MODE6_Mask (uint32_t)(0x03 << 12)    // for pin 6 (2 bits per pin)
// we convert to unsigned here as we dont want to accidently shift the sign bit, this
// can lead to errors for comparing a negative signed int with an unsigned it.
  
//flags OTYPER Register: (pins 0,1,2 configured as push-pull)
#define GPIO_OTYPER_OTYPE0_Mask 0x00000001
#define GPIO_OTYPER_OTYPE1_Mask (uint32_t)(0x00000001 << 1)
#define GPIO_OTYPER_OTYPE2_Mask (uint32_t)(0x00000001 << 2)

//flags OSPEEDR Register:
// #define OSPEEDR_OSPEED6_Mask 

//flags PUPDR Register:
#define GPIO_PUPDR_PUPD0_Mask 0x03   // not pulled up or down
#define GPIO_PUPDR_PUPD1_Mask (uint32_t)(0x03 << 2)   // not pulled up or down
#define GPIO_PUPDR_PUPD2_Mask (uint32_t)(0x03 << 4)   // not pulled up or down
#define GPIO_PUPDR_PUPD6_Mask (uint32_t)(0x02 << 12)  // to pull it down

// flags for data (to obtain the value at 1 bit while masking every other bit)
#define GPIO_IDR_ID6_DATA_Mask (uint32_t)(0x01 << 6)
#define GIO_IDR_ID0_DATA_Mask 0x01
#define GIO_ODR_OD0_DATA_Mask 0x01

//flags AFR1 Register:
#define GPIO_AFR1_AF01_Mask 0x02    // 0: pin 0, 1: AFR low register

//flags ODR Register:
#define GPIO_ODR_OD0_Mask 0x00000001
#define GPIO_ODR_OD1_Mask (uint32_t)(0x00000001 << 1)
#define GPIO_ODR_OD2_Mask (uint32_t)(0x00000001 << 2)


/* function definitions----------------------------------------------------------*/

void initGpioC6AsInput(void)
{
    uint32_t  * reg_pointer;

    /* GPIOC Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* GPIOC Pin 6 as input*/
    reg_pointer = (uint32_t *) PORTC_MODER_REGISTER;
    *reg_pointer = *reg_pointer & (~GPIO_MODER_MODE6_Mask);
    
    /*PUSH-PULL Pin*/ // required? because the port is an input!

    
    /*GPIOC pin 6 high speed */ // optional here
    

    /*Configure pulled-down*/ // (ensures port is at GND level when left floating externally)
    reg_pointer = (uint32_t *) PORTC_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_PUPDR_PUPD6_Mask;
}


void initGpioB0AsOutput( void )
{
    uint32_t  * reg_pointer;

    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOB0 configured as output */
    reg_pointer = (uint32_t *) PORTB_MODER_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_MODER_MODE0_OUTPUT_Mask;
    
    /*GPIOB0 configured as push-pull */
    reg_pointer = (uint32_t *) PORTB_OTYPER_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_OTYPER_OTYPE0_Mask);
    
    /*GPIOB0 configured floating (no pull-up resistor connected; may because the board has a pullup connected to the LED) */
    reg_pointer = (uint32_t *) PORTB_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_PUPDR_PUPD0_Mask);

}

void initGpioB1AsOutput( void )
{
    uint32_t  * reg_pointer;

    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOB0 configured as output */
    reg_pointer = (uint32_t *) PORTB_MODER_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_MODER_MODE1_OUTPUT_Mask;
    
    /*GPIOB0 configured as push-pull */
    reg_pointer = (uint32_t *) PORTB_OTYPER_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_OTYPER_OTYPE1_Mask);
    
    /*GPIOB0 configured floating (no pull-up resistor connected; may because the board has a pullup connected to the LED) */
    reg_pointer = (uint32_t *) PORTB_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_PUPDR_PUPD1_Mask);

}

void initGpioB2AsOutput( void )
{
    uint32_t  * reg_pointer;

    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOB0 configured as output */
    reg_pointer = (uint32_t *) PORTB_MODER_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_MODER_MODE2_OUTPUT_Mask;
    
    /*GPIOB0 configured as push-pull */
    reg_pointer = (uint32_t *) PORTB_OTYPER_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_OTYPER_OTYPE2_Mask);
    
    /*GPIOB0 configured floating (no pull-up resistor connected; may because the board has a pullup connected to the LED) */
    reg_pointer = (uint32_t *) PORTB_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_PUPDR_PUPD2_Mask);

}

void initGpioB0AsAlternate2( void )
{
    uint32_t  * reg_pointer;

    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOB0 configured as alternate */
    reg_pointer = (uint32_t *) PORTB_MODER_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_MODER_MODE0_ALTERNATE_Mask;
    
    /*GPIOB0 configured as push-pull */
    reg_pointer = (uint32_t *) PORTB_OTYPER_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_OTYPER_OTYPE0_Mask);
    
    /*GPIOB0 configured floating (no pull-up resistor connected; may because the board has a pullup connected to the LED) */
    reg_pointer = (uint32_t *) PORTB_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_PUPDR_PUPD0_Mask);

    /*GPIO0 mapped to timer3 channel3 using the GPIO alternate function low register*/
    reg_pointer = (uint32_t *) PORTB_AFR1_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_AFR1_AF01_Mask;

}

void toggleGPIOB0( void )
{
    uint32_t value;
    uint32_t  * reg_pointer;

    //get the current value of the pin 
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    value = *reg_pointer & (uint32_t)GIO_ODR_OD0_DATA_Mask;
    // now every bit in value is 0 except the relevant output which could be 0 or 1

    if (value > 0)
    {
        //if high, clear the bit
        *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_ODR_OD0_Mask);
    }
    else
    {
        //if low, set the bit
        *reg_pointer = *reg_pointer | GPIO_ODR_OD0_Mask;
    } 
}

void setGPIOB0( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_ODR_OD0_Mask;
}

void clearGPIOB0( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_ODR_OD0_Mask);

}

void setGPIOB1( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_ODR_OD1_Mask;
}

void clearGPIOB1( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_ODR_OD1_Mask);

}

void setGPIOB2( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer | GPIO_ODR_OD2_Mask;
}

void clearGPIOB2( void )
{
    uint32_t  * reg_pointer;
    reg_pointer = (uint32_t *) PORTB_ODR_REGISTER;
    *reg_pointer = *reg_pointer & (~(uint32_t)GPIO_ODR_OD2_Mask);

}

uint32_t checkGPIOC6(void)
{
    uint32_t valueC6;
    uint32_t * reg_pointer;

    reg_pointer = (uint32_t *) PORTC_IDR_REGISTER;

    valueC6 = *reg_pointer; // copy value at register to another variable because we 
                            // dont want to modify all pins of the register
    valueC6 = valueC6 & GPIO_IDR_ID6_DATA_Mask; // apply mask to make all values zero except 
                                                // the value at PC6 which can be 0 or 1
    valueC6 = valueC6 >> 6; // bitshift right by 6 bits to get the output as 1 or 0

    return valueC6;   
}  

uint32_t checkGPIOB0(void)
{
    uint32_t valueB0;
    uint32_t * reg_pointer;

    reg_pointer = (uint32_t *) PORTB_IDR_REGISTER;

    valueB0 = *reg_pointer; // copy value at register to another variable because we 
                            // dont want to modify all pins of the register
    valueB0 = valueB0 & GIO_IDR_ID0_DATA_Mask; // apply mask to make all values zero except 
                                                // the value at PB0 which can be 0 or 1

    return valueB0;   
}  