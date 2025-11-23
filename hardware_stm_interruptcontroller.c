/**
  ******************************************************************************
  * @file    hardware_stm_interruptcontroller.c 
  * @author  asramdas@andrew.cmu.edu
  * @version 1.0
  * @date    October-2025
  * @brief   Enables interrupt handling, and contains interrupt service functions.
  ******************************************************************************
*/

#include "hardware_stm_interruptcontroller.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer3.h"
#include "stm32f4xx_rcc_mort.h"
#include "debug_mort.h"   // for print statements
#include "events.h" // for populating event queue
#include <cstdint>  /*C++ version of the <stdint.h> C header*/

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
#define TIMER3_CC_REGISTER_3 (TIMER3_BASE_ADDRESS + 0x3C)
#define TIMER3_INTERRUPT_ENABLE_REGISTER (TIMER3_BASE_ADDRESS + 0x0C)

/* MACRO definitions for NVIC----------------------------------------------------------*/
#define SYSTEM_CONTROL_BASE_ADDRESS (0xE000E000)
#define NVIC_BASE_ADDRESS (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 (NVIC_BASE_ADDRESS)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63 (NVIC_BASE_ADDRESS+0x4)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95 (NVIC_BASE_ADDRESS+0x8)
#define TIMER3_INTERRUPT_BIT (0x20000000)

// flags = bit masks (we have defined them as signed integers! need to cast them to unsigned
// before performing any bitwise operation, otherwise apparently there can be errors as
// inverting a signed positive integer gives a negative integer)
// we dont need to convert it for the or operation because there is no flipping bits involved 

// flag for control register enable/disable timer3
#define TIMER3_CR1_Mask 0x01

// prescaler value (based on clock max count = 30_000)
#define TIMER3_PSC_Value (uint16_t)2999   // would an int/hexadecimal prescaler value work? as the register stores binary values   

// auto reload value
#define TIMER3_ARR_Value (uint16_t)29999    //(0 = 30000 in counts)

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

//flags for Status register (note that it is read clear by writing 0):
#define TIMER3_UIF 0x01 //timer 3 overflow flag
#define TIMER3_CH1_CC1IF (uint16_t)(0x1 << 1) //timer channel 1 capture/compare event
#define TIMER3_CH2_CC2IF (uint16_t)(0x1 << 2)
#define TIMER3_CH3_CC3IF (uint16_t)(0x1 << 3) //timer channel 3 capture/compare event
#define TIMER3_CH4_CC4IF (uint16_t)(0x1 << 4)

// Enabling interrupt detection
void enableNVIC_Timer3(void)
{
    uint32_t * reg_pointer_32;
    reg_pointer_32 = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
    *reg_pointer_32 = TIMER3_INTERRUPT_BIT;
    // debugprint_2(*reg_pointer_32);    
}

// Interrupt handlers
void TIM3_IRQHandler(void)    // stands for interrupt request handler?
{
    uint16_t * reg_pointer_sr;  // for status reg
    uint16_t * reg_pointer_ier; //for interrupt enable reg
    reg_pointer_sr = (uint16_t *)TIMER3_STATUS_REGISTER;
    reg_pointer_ier = (uint16_t *)TIMER3_INTERRUPT_ENABLE_REGISTER;

    // debugprint(*reg_pointer_sr);

    //check which interrupts fired and if they were supposed to fire, then clear the flags so they don’t keep firing,
    // then perform actions according to these interrupts

    //check which Output Compare 3 triggered the interrupt:
    if (( (*reg_pointer_sr & TIMER3_CH1_CC1IF) >0) && ( (*reg_pointer_ier & TIMER3_CH1_CC_INTERRUPT_ENABLE_Mask) >0))
    {
        //clear interrupt
        *reg_pointer_sr = ~((uint16_t)TIMER3_CH1_CC1IF);
        //perform action
        setGPIOB0();
        // update event queue with event
        LED_event e;
        e.type = LED1_ON;
        enqueue_event(e);
    }

    if (( (*reg_pointer_sr & TIMER3_CH2_CC2IF) >0) && ( (*reg_pointer_ier & TIMER3_CH2_CC_INTERRUPT_ENABLE_Mask) >0))
    {
        //clear interrupt
        *reg_pointer_sr = ~((uint16_t)TIMER3_CH2_CC2IF);
        //perform action
        clearGPIOB0();
        setGPIOB1();

        // update event queue with event
        LED_event e;
        e.type = LED2_ON;
        enqueue_event(e);
    }

    if (( (*reg_pointer_sr & TIMER3_CH3_CC3IF) >0) && ( (*reg_pointer_ier & TIMER3_CH3_CC_INTERRUPT_ENABLE_Mask) >0))
    {
        //clear interrupt
        *reg_pointer_sr = ~((uint16_t)TIMER3_CH3_CC3IF);
        //perform action
        clearGPIOB1();
        setGPIOB2();
        
        // update event queue with event
        LED_event e;
        e.type = LED3_ON;
        enqueue_event(e);
    }

    if (( (*reg_pointer_sr & TIMER3_CH4_CC4IF) >0) && ( (*reg_pointer_ier & TIMER3_CH4_CC_INTERRUPT_ENABLE_Mask) >0))
    {
        //clear interrupt
        *reg_pointer_sr = ~((uint16_t)TIMER3_CH4_CC4IF);
        //perform action
        clearGPIOB2();

        // update event queue with event
        LED_event e;
        e.type = CLEAR_ALL;
        enqueue_event(e);
    }

    //check if Overflow triggered the interrupt: I.e. Timer Counter 3 >= Autorreload value
    // if (( (*reg_pointer_sr & TIMER3_UIF) >0) && ( (*reg_pointer_ier & TIMER3_UPDATE_INTERRUPT_ENABLE_Mask) >0))
    // {
    //     //clear interrupt
    //     *reg_pointer_sr = ~((uint16_t)TIMER3_UIF);
    //     //perform action
    //     setGPIOB0();
    // }
}