/**
  ******************************************************************************
  * @file    hardware_stm_interruptcontroller.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Oct-2025
  * @brief   Controls STM32F446ze interrupt
  ******************************************************************************
  */

#include "hardware_stm_interruptcontroller.h"
#include "debug_mort.h"
#include "events.h"
#include "gummy_led_utils.h"
#include "state_machine.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer3.h"
#include "hardware_stm_timer4.h"
#include "global_time.h"



/* Button Interrupt flags */
volatile int32_t pressedTime           = 0;
int8_t           butFlag =0;
#define BUTTON_DEBOUNCE_MS = 50;


void enableEXTI6OnPortC(void)
{
    uint32_t * reg_pointer_32;
    
    /* Initialize GPIO C6 as input */
    initGpioCxAsInput_PU(6);
    
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Map EXTI6 to Port C pin 6 */
    SYSCFG_EXTERNAL_INTERRUPT_REGISTER_2 &= ~SYSCFG_EXTERNAL_INTERRUPT_6_BITS;
    SYSCFG_EXTERNAL_INTERRUPT_REGISTER_2 |= SYSCFG_EXTERNAL_INTERRUPT_6_PORTC;
    
    /* Unmask EXTI6 in the interrupt mask register */
    EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER |= EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER_EXTI6;
    
    /* Configure to trigger on rising edge */
    EXTERNAL_INTERRUPT_CONTROLLER_RTSR |= EXTERNAL_INTERRUPT_CONTROLLER_RTSR_EXTI6;
    
    /* Enable EXTI9_5 interrupt in NVIC */
    NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 = EXTI9_5_INTERRUPT_BIT;
}

void EXTI9_5_IRQHandler(void)
{    
    /* Check if EXTI6 triggered the interrupt */
    if ((EXTERNAL_INTERRUPT_CONTROLLER_PENDING_REGISTER & EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6) > 0)
    {
        /* Clear the EXTI6 pending bit by writing 1 */
        EXTERNAL_INTERRUPT_CONTROLLER_PENDING_REGISTER = EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6;

        int32_t currentTime = current_time_ms();

        // debouncing 
        if ((currentTime - pressedTime) > 500)
        {
            pressedTime = currentTime;
            enqueue_event(BUTTON_PRESSED, 1, 1);
            printf("what the heck\n");
        } 
    }
    
    /* Additional handlers for EXTI5, 7, 8, 9 can be added bekiw*/
}

void TIM3_IRQHandler(void)
{
    /* Check if Channel 3 Compare match triggered the interrupt */
    if (TIM3_GetCC3Flag())
    {
        /* Clear the CC3 interrupt flag */
        TIM3_ClearCC3Flag();
        /* Perform action: Clear GPIO B0 (LED off or dim) */
        clearGPIOBx(0);
    }
    
    /* Check if Update event (overflow) triggered the interrupt */
    if (TIM3_GetUpdateFlag())
    {
        /* Clear the Update interrupt flag */
        TIM3_ClearUpdateFlag();
        
        /* Perform action: Set GPIO B0 (LED on or bright) */
        setGPIOBx(0);
    }
}

void TIM4_IRQHandler(void)
{
    // /* Check if Channel 3 Compare match triggered the interrupt */
    // if (TIM4_GetCC3Flag())
    // {
    //     /* Clear the CC3 interrupt flag */
    //     TIM4_ClearCC3Flag();
    //     /* Perform action: Clear GPIO B0 (LED off or dim) */
    //     clearGPIOBx(0);
    // }
    
    /* Check if Update event (overflow) triggered the interrupt */
    if (TIM4_GetUpdateFlag())
    {
        /* Clear the Update interrupt flag */
        TIM4_ClearUpdateFlag();
        
        /* Perform action: update global time */
        increment_time();
    }
}