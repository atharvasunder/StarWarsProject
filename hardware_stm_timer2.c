// #include "mbed2/299/TARGET_NUCLEO_F446ZE/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F446ZE/device/stm32f4xx.h"
#include "stdint.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer2.h"


void initTimer2_CH2_PWM( void )
{
    /* 2. TIM2 Peripheral clock enable (Timer is on APB1 bus) */
    // Uses the function from your header
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    uint32_t * reg_pointer;

    /* 1. Set Timer Frequency (Time Base) */
    reg_pointer = (uint32_t*)TIM2_PRESCALER_REGISTER;
    *reg_pointer = 89;
    reg_pointer = (uint32_t*)TIM2_AUTORELOAD_REGISTER;
    *reg_pointer = 999;

    /* 2. Set Duty Cycle (Pulse Width) */
    reg_pointer = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    *reg_pointer = 500; 

    /* 3. Configure PWM Mode for Channel 2 */
    
    // Get the address for the CCMR1 register (controls CH1 & CH2)
    reg_pointer = (uint32_t*)TIM2_CAPTURE_COMPARE_MODE1_REGISTER;
    
    // *** THE FIX ***
    // Clear the old mode bits for Channel 2
    *reg_pointer = *reg_pointer & (~(TIM_CCMR1_OC2M_MASK)); 
    
    // Set PWM Mode 1 and Preload Enable **using the new Channel 2 flags**
    *reg_pointer = *reg_pointer | TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

    /* 4. Enable Channel 2 Output */
    reg_pointer = (uint32_t*)TIM2_CAPTURE_COMPARE_ENABLE_REGISTER;
    *reg_pointer = *reg_pointer | TIM_CCER_CC2E;

    /* 5. Enable the Timer */
    reg_pointer = (uint32_t*)TIM2_CR1_REGISTER_1;
    *reg_pointer = *reg_pointer | TIM_CR1_ARPE | TIM_CR1_CEN;
}

// void initTimer3_CH2_PWM( void )
// {
//     uint32_t * reg_pointer;

//     /* 1. Set Timer Frequency (Time Base) */
    
//     // Set Prescaler (PSC)
//     // Example: 45MHz clock / (44 + 1) = 1MHz counter clock
//     reg_pointer = (uint32_t*)TIM2_PRESCALER_REGISTER;
//     *reg_pointer = 44;
    
//     // Set Auto-Reload (ARR)
//     // Example: 1MHz clock / (999 + 1) = 1kHz PWM frequency
//     reg_pointer = (uint32_t*)TIM2_AUTORELOAD_REGISTER;
//     *reg_pointer = 999;

//     /* 2. Set Duty Cycle (Pulse Width) */
    
//     // Set Capture/Compare Register 2 (CCR2)
//     // Example: 500 = 50% duty cycle (500 / 1000)
//     reg_pointer = (uint32_t*)TIM2_COMPARE_2_REGISTER;
//     *reg_pointer = 500; // 50% duty cycle for a test tone

//     /* 3. Configure PWM Mode for Channel 2 */
    
//     // Set to PWM Mode 1 (110) and enable Preload
//     reg_pointer = (uint32_t*)TIM2_CAPTURE_COMPARE_MODE1_REGISTER;
//     *reg_pointer = *reg_pointer & (~(TIM_CCMR1_OC2M_MASK)); // Clear mode bits
//     *reg_pointer = *reg_pointer | TIM2_CCMR13_PWM1| TIM_CCMR1_OC2PE;

//     /* 4. Enable Channel 2 Output */
    
//     // Set the CC2E bit in the CCER register
//     reg_pointer = (uint32_t*)TIM2_CAPTURE_COMPARE_ENABLE_REGISTER;
//     *reg_pointer = *reg_pointer | TIM_CCER_CC2E;

//     /* 5. Enable the Timer */
    
//     // Enable Auto-Reload Preload (ARPE) and Counter (CEN)
//     reg_pointer = (uint32_t*)TIM2_CR1_REGISTER_1;
//     *reg_pointer = *reg_pointer | TIM_CR1_ARPE | TIM_CR1_CEN;
// }

