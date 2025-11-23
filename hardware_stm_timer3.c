/**
  ******************************************************************************
  * @file    hardware_stm_timer3.c 
  * @author  asramdas@andrew.cmu.edu
  * @version 1.0
  * @date    October 2025
  * @brief   Controls STM32F446ze Timer3
  ******************************************************************************
  */

#include "hardware_stm_timer3.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_interruptcontroller.h"
#include <cstdint>  /*C++ version of the <stdint.h> C header*/
#include "debug_mort.h"

/* MACRO definitions----------------------------------------------------------*/
//Timer 3 addresses:
#define TIMER3_BASE_ADDRESS ((uint32_t)0x40000400)

#define TIMER3_CONTROL_REGISTER_1 (TIMER3_BASE_ADDRESS + 0x00)
// #define TIMER3_CONTROL_REGISTER_2 (TIMER3_BASE_ADDRESS + 0x04)
#define TIMER3_STATUS_REGISTER (TIMER3_BASE_ADDRESS + 0x10)
#define TIMER3_CC_MODE_REGISTER_2 (TIMER3_BASE_ADDRESS + 0x1C)
#define TIMER3_CC_MODE_REGISTER_1 (TIMER3_BASE_ADDRESS + 0x18)
#define TIMER3_CC_ENABLE_REGISTER (TIMER3_BASE_ADDRESS + 0x20)  
#define TIMER3_COUNTER_REGISTER (TIMER3_BASE_ADDRESS + 0x24)
#define TIMER3_PRESCALER_REGISTER (TIMER3_BASE_ADDRESS + 0x28)
#define TIMER3_AUTO_RELOAD_REGISTER (TIMER3_BASE_ADDRESS + 0x2C)
#define TIMER3_CC_REGISTER_1 (TIMER3_BASE_ADDRESS + 0x34)
#define TIMER3_CC_REGISTER_2 (TIMER3_BASE_ADDRESS + 0x38)
#define TIMER3_CC_REGISTER_3 (TIMER3_BASE_ADDRESS + 0x3C)
#define TIMER3_CC_REGISTER_4 (TIMER3_BASE_ADDRESS + 0x40)
#define TIMER3_INTERRUPT_ENABLE_REGISTER (TIMER3_BASE_ADDRESS + 0x0C)


// flags = bit masks (we have defined them as signed integers! need to cast them to unsigned
// before performing any bitwise operation, otherwise apparently there can be errors as
// inverting a signed positive integer gives a negative integer)
// we dont need to convert it for the or operation because there is no flipping bits involved 

// flag for control register enable/disable timer3
#define TIMER3_CR1_Mask 0x01

// prescaler value (based on clock frequency = 3000Hz)
#define TIMER3_PSC_Value (uint16_t)29999   // would an int/hexadecimal prescaler value work? as the register stores binary values   

// auto reload value
#define TIMER3_ARR_Value (uint16_t)23999    //(0 to 24000 in counts at 3000 counts/sec)

// flag for capture compare mode setting for the timer channels
#define TIMER3_CC_CHANNEL4_DIRECTION_Mask (uint16_t)(0X03 << 8)   // channel to be set as output, so i want to clear those bits
#define TIMER3_CHANNEL4_CC_OUTPUT_COMPARE_MODE_Mask (uint16_t)(0X03 << 12)

#define TIMER3_CC_CHANNEL3_DIRECTION_Mask 0X03   // channel to be set as output, so i want to clear those bits
#define TIMER3_CHANNEL3_CC_OUTPUT_COMPARE_MODE_Mask (uint16_t)(0X03 << 4)

#define TIMER3_CC_CHANNEL2_DIRECTION_Mask (uint16_t)(0X03 << 8)   // channel to be set as output, so i want to clear those bits
#define TIMER3_CHANNEL2_CC_OUTPUT_COMPARE_MODE_Mask (uint16_t)(0X03 << 12)

#define TIMER3_CC_CHANNEL1_DIRECTION_Mask 0X03   // channel to be set as output, so i want to clear those bits
#define TIMER3_CHANNEL1_CC_OUTPUT_COMPARE_MODE_Mask (uint16_t)(0X03 << 4)

// output compare value for timer3
#define TIMER3_CHANNEL1_COMPARE_VALUE (uint16_t)TIMER3_ARR_Value/4
#define TIMER3_CHANNEL2_COMPARE_VALUE (uint16_t)TIMER3_ARR_Value/2
#define TIMER3_CHANNEL3_COMPARE_VALUE (uint16_t)3*TIMER3_ARR_Value/4
#define TIMER3_CHANNEL4_COMPARE_VALUE (uint16_t)(TIMER3_ARR_Value - 1)

// flag for capture compare channel enable register
#define TIMER3_ENABLE_CHANNEL1_Mask (uint16_t)(0x01)
#define TIMER3_ENABLE_CHANNEL2_Mask (uint16_t)(0x01 << 4)
#define TIMER3_ENABLE_CHANNEL3_Mask (uint16_t)(0x01 << 8)
#define TIMER3_ENABLE_CHANNEL4_Mask (uint16_t)(0x01 << 12)

// preload flag (to be able to change how the output compare value gets updated with time)
#define TIMER3_CHANNEL1_PRELOAD_Mask (uint16_t)(0x01 << 3)
#define TIMER3_CHANNEL2_PRELOAD_Mask (uint16_t)(0x01 << 11)
#define TIMER3_CHANNEL3_PRELOAD_Mask (uint16_t)(0x01 << 3)
#define TIMER3_CHANNEL4_PRELOAD_Mask (uint16_t)(0x01 << 11)


//flags for interrupt enable register:
#define TIMER3_UPDATE_INTERRUPT_ENABLE_Mask 0x1 //timer overflow or update event interrupt
#define TIMER3_CH1_CC_INTERRUPT_ENABLE_Mask (uint16_t)(0x1 << 1) //timer channel 3 capture/compare interrupt
#define TIMER3_CH2_CC_INTERRUPT_ENABLE_Mask (uint16_t)(0x1 << 2) //timer channel 3 capture/compare interrupt
#define TIMER3_CH3_CC_INTERRUPT_ENABLE_Mask (uint16_t)(0x1 << 3) //timer channel 3 capture/compare interrupt
#define TIMER3_CH4_CC_INTERRUPT_ENABLE_Mask (uint16_t)(0x1 << 4) //timer channel 3 capture/compare interrupt

//flags for Status register:
#define TIMER3_UIF 0x01 //timer 3 overflow flag
#define TIMER3_CH1_CC1IF 0x02 //timer channel 1 capture/compare event
#define TIMER3_CH3_CC3IF 0x8 //timer channel 3 capture/compare event

/* function definitions----------------------------------------------------------*/
void initTimer3ChannelsAsOutput(void)
{
    uint16_t  * reg_pointer;

    /* Timer3 Peripheral clock enable so that we can talk to registers */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /*Tell the interrupt controller to look for interrupts from timer3*/
    enableNVIC_Timer3();

    /* Start by turning off the timer3, just as a safety measure*/
    reg_pointer = (uint16_t *) TIMER3_CONTROL_REGISTER_1;
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CR1_Mask);

    // // /*Clear any flags from status register just in case*/
    // reg_pointer = (uint16_t *) TIMER3_STATUS_REGISTER;
    // *reg_pointer = ~((uint16_t)TIMER3_CH3_CC3IF);
    // *reg_pointer = ~((uint16_t)TIMER3_UIF);

    /*Set pre-scale value*/
    reg_pointer = (uint16_t *) TIMER3_PRESCALER_REGISTER;
    *reg_pointer = TIMER3_PSC_Value;

    /*Set auto-reload value*/
    reg_pointer = (uint16_t *) TIMER3_AUTO_RELOAD_REGISTER;
    *reg_pointer = TIMER3_ARR_Value;

    /*Set timer3 channels 1,2,3,4 as output, and leave output mode as frozen*/
    reg_pointer = (uint16_t *) TIMER3_CC_MODE_REGISTER_2;
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CC_CHANNEL1_DIRECTION_Mask);
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CC_CHANNEL2_DIRECTION_Mask);
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CC_CHANNEL3_DIRECTION_Mask);
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CC_CHANNEL4_DIRECTION_Mask);
    // *reg_pointer = *reg_pointer | TIMER3_CC_OUTPUT_COMPARE_MODE_Mask);

    /*Set compare value for all channels*/
    reg_pointer = (uint16_t *) TIMER3_CC_REGISTER_1;
    *reg_pointer = TIMER3_CHANNEL1_COMPARE_VALUE;

    reg_pointer = (uint16_t *) TIMER3_CC_REGISTER_2;
    *reg_pointer = TIMER3_CHANNEL2_COMPARE_VALUE;
    
    reg_pointer = (uint16_t *) TIMER3_CC_REGISTER_3;
    *reg_pointer = TIMER3_CHANNEL3_COMPARE_VALUE;

    reg_pointer = (uint16_t *) TIMER3_CC_REGISTER_4;
    *reg_pointer = TIMER3_CHANNEL4_COMPARE_VALUE;

    /*Enable preload for all channels*/
    reg_pointer = (uint16_t *) TIMER3_CC_MODE_REGISTER_2;
    *reg_pointer = *reg_pointer | TIMER3_CHANNEL1_PRELOAD_Mask | 
                    TIMER3_CHANNEL2_PRELOAD_Mask | TIMER3_CHANNEL3_PRELOAD_Mask | TIMER3_CHANNEL4_PRELOAD_Mask;
    
    /*Enable channels*/
    reg_pointer = (uint16_t *) TIMER3_CC_ENABLE_REGISTER;
    *reg_pointer = *reg_pointer | TIMER3_ENABLE_CHANNEL1_Mask | TIMER3_ENABLE_CHANNEL2_Mask 
                    | TIMER3_ENABLE_CHANNEL3_Mask | TIMER3_ENABLE_CHANNEL4_Mask;

    /*enable interrupt detection on capture compare channels 1,2,3,4*/
    reg_pointer = (uint16_t *)TIMER3_INTERRUPT_ENABLE_REGISTER;
    *reg_pointer = *reg_pointer | TIMER3_CH1_CC_INTERRUPT_ENABLE_Mask | TIMER3_CH2_CC_INTERRUPT_ENABLE_Mask
                    | TIMER3_CH3_CC_INTERRUPT_ENABLE_Mask | TIMER3_CH4_CC_INTERRUPT_ENABLE_Mask;

    /*Now start timer3*/
    reg_pointer = (uint16_t *) TIMER3_CONTROL_REGISTER_1;
    *reg_pointer = *reg_pointer | TIMER3_CR1_Mask;

}


void initTimer3Channel1AsInput( void )
{
    uint16_t  * reg_pointer;

    /* Timer3 Peripheral clock enable so that we can talk to registers */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Start by turning off the timer3, just as a safety measure*/
    reg_pointer = (uint16_t *) TIMER3_CONTROL_REGISTER_1;
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CR1_Mask);

    /*Set pre-scale value*/
    reg_pointer = (uint16_t *) TIMER3_PRESCALER_REGISTER;
    *reg_pointer = TIMER3_PSC_Value;

    /*Set auto-reload value*/
    reg_pointer = (uint16_t *) TIMER3_AUTO_RELOAD_REGISTER;
    *reg_pointer = TIMER3_ARR_Value;

    /*Set timer3 channel1 as input, and then set input mode to toggle*/
    reg_pointer = (uint16_t *) TIMER3_CC_MODE_REGISTER_1;
    *reg_pointer = *reg_pointer & (~(uint16_t)TIMER3_CC_CHANNEL1_DIRECTION_Mask);
    *reg_pointer = *reg_pointer | TIMER3_CHANNEL1_CC_OUTPUT_COMPARE_MODE_Mask;

    /*Set compare value*/
    reg_pointer = (uint16_t *) TIMER3_CC_REGISTER_3;
    *reg_pointer = TIMER3_CHANNEL1_COMPARE_VALUE;
    
    /*Enable channel3*/
    reg_pointer = (uint16_t *) TIMER3_CC_ENABLE_REGISTER;
    *reg_pointer = *reg_pointer | TIMER3_ENABLE_CHANNEL3_Mask;

    /*Now start timer3*/
    reg_pointer = (uint16_t *) TIMER3_CONTROL_REGISTER_1;
    *reg_pointer = *reg_pointer | TIMER3_CR1_Mask;

}

uint16_t get_timer3_count(void)
{
    uint16_t  * reg_pointer;

    reg_pointer = (uint16_t *) TIMER3_COUNTER_REGISTER;
    return *reg_pointer;
}

