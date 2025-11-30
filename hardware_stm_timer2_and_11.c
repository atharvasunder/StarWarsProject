// #include "mbed2/299/TARGET_NUCLEO_F446ZE/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F446ZE/device/stm32f4xx.h"
#include "stdint.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer2_and_11.h"



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

void initTimer11_CH1_PWM( void )
{
    uint32_t * reg_pointer;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);

    /* 1. Set Timer Frequency (Time Base) */
    // Note: TIM11 is on APB2. Check if APB2 is faster than APB1 (TIM2).
    // If APB2 is 2x faster, double this prescaler to keep the same sound pitch.
    reg_pointer = (uint32_t*)TIM11_PSC_REGISTER;
    *reg_pointer = 83; 
    
    reg_pointer = (uint32_t*)TIM11_ARR_REGISTER;
    *reg_pointer = 999;

    /* 2. Set Duty Cycle (Pulse Width) */
    // TIM11 only has Channel 1, so we use CCR1
    reg_pointer = (uint32_t*)TIM11_CCR1_REGISTER;
    *reg_pointer = 500; 

    /* 3. Configure PWM Mode for Channel 1 */
    
    // Get the address for the CCMR1 register
    reg_pointer = (uint32_t*)TIM11_CCMR1_REGISTER;
    
    // Clear the old mode bits for Channel 1 (Bits 6:4)
    *reg_pointer = *reg_pointer & (~(TIM_CCMR1_OC1M_MASK)); 
    
    // Set PWM Mode 1 and Preload Enable for Channel 1
    *reg_pointer = *reg_pointer | TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

    /* 4. Enable Channel 1 Output */
    // We use CCER bit 0 (CC1E) instead of bit 4 (CC2E)
    reg_pointer = (uint32_t*)TIM11_CCER_REGISTER;
    *reg_pointer = *reg_pointer | TIM_CCER_CC1E;

    /* 5. Enable the Timer */
    reg_pointer = (uint32_t*)TIM11_CR1_REGISTER;
    *reg_pointer = *reg_pointer | TIM_CR1_ARPE | TIM_CR1_CEN;
}