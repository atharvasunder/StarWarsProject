/**
  ******************************************************************************
  * @file    hardware_stm_timer3.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Oct-2025
  * @brief   Controls STM32F446ze timer3
  ******************************************************************************
  */

#include "hardware_stm_timer3.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_dma_controller.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>
/* MACRO definitions----------------------------------------------------------*/

void initTimer3PWM(uint16_t prescalervalue, uint16_t autoreloadvalue)
{
    /*Function to start timer3 channel 2 (used for LED strip)*/

    /* Enable Timer 3 APB1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /*initialize DMA controller to sync with the timer (DMA is not on yet)*/
    initDMAForTimer3Channel2();
    
    /* Clear status register */
    TIM3_SR = 0;
    
    /* Set Prescaler */
    TIM3_PSC = prescalervalue;
    
    /* Set Auto-Reload */
    TIM3_ARR = autoreloadvalue;

    //  /* Clamp duty cycle */
    // if (duty_cycle < 0.0f) { duty_cycle = 0.0f; }
    // if (duty_cycle > 1.0f) { duty_cycle = 1.0f; }
    
    /* Set Compare value for  duty cycle */
    TIM3_CCR2 = (uint16_t)((float)autoreloadvalue);  // 
    
    /* Configure Channel 2 as Output */
    TIM3_CCMR1 = TIM3_CCMR1 & ~TIM3_CCMR1_CC2S_MASK;        // Clear CC2S bits
    TIM3_CCMR1 = TIM3_CCMR1 | TIM3_CCMR1_CC2S_OUTPUT;       // CC2 as output
    
    /* Enable Output Compare 2 Preload */
    TIM3_CCMR1 = TIM3_CCMR1 | TIM3_CCMR1_OC2PE;
    
    /* Set PWM Mode 1 on Channel 2 */
    TIM3_CCMR1 = TIM3_CCMR1 & ~TIM3_CCMR1_OC2M_MASK;        // Clear OC2M bits
    TIM3_CCMR1 = TIM3_CCMR1 | TIM3_CCMR1_OC2M_PWM1;         // Set PWM mode 1
    
    /* Enable Channel 2 output with default polarity (active high) */
    TIM3_CCER = TIM3_CCER | TIM3_CCER_CC2E;
    
    /* Enable Auto-Reload Preload (optional but recommended) */
    TIM3_CR1 = TIM3_CR1 | (1 << 7);  // ARPE bit

    /*Enable channel 2 sending DMA requests*/
    TIM3_DIER = TIM3_DIER | TIM3_DIER_CC2DE;    // send dma request when there is a output compare event
    
    /* Start the timer */
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
}

void timer3_set_duty(uint16_t arr_value){
    /* Set Compare value for  duty cycle for channel 2 pwm*/
    TIM3_CCR2 = (uint16_t)((float)arr_value);
}

// void timer3_init(uint16_t prescale_value, uint16_t period){
//     // Step 1: Enable the APB1 clock for TIM3
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
//     // stop timer if running
//     TIM3_CR1 = TIM3_CR1 & ~TIM3_CR1;

//     // Step 2: Clear the update event flag in TIM3_SR
//     TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;

//     // Make sure this start from 0
//     TIM3_CNT = TIM3_CNT & ~TIM3_CNT;
    
//     // Step 3: Upload the prescale value to TIM3_PSC
//     TIM3_PSC = prescale_value;
    
//     // Step 4: Set the period value to TIM3_ARR
//     TIM3_ARR = period;
    
//     // Step 5: Enable the timer by setting CEN bit in TIM3_CR1
//     TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
// }

// void timer3_channel3_output_compare(uint16_t prescale_value,uint16_t period, uint16_t compare_value){
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//     TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
//     TIM3_PSC = prescale_value;
//     TIM3_ARR = period;
    
//     // Configure channel 3 as output and set to Toggle mode
//     TIM3_CCMR2 = TIM3_CCMR2 & ~(0xFF << 0);  // Clear channel 3 bits (bits 7:0)
//     TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_CC3S_OUTPUT /* CC3 as output */
//                             | TIM3_CCMR2_OC3M_TOGGLE;/* OC3M = 011 (toggle)*/

//     // Step 6: Set the compare value in CCR3
//     TIM3_CCR3 = compare_value;
    
//     // Step 7: Enable channel 3 output by setting CC3E bit in CCER
//     TIM3_CCER = TIM3_CCER | TIM3_CCER_CC3E;
    
//     // Step 8: Enable the timer by setting CEN bit in TIM3_CR1
//     TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;

// }

// void timer3_ch1_input_capture_init(uint16_t prescale_value, uint16_t period)
// {
//     // Step 1: Enable the APB1 clock for TIM3
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//     // Step 2: Clear the update event flag in TIM3_SR
//     TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
//     // Step 3: Upload the prescale value to TIM3_PSC
//     TIM3_PSC = prescale_value;
//     // Step 4: Set the period value to TIM3_ARR
//     TIM3_ARR = period;
//     // Step 5: Set the input capture mode in CCMR1 register for channel 1
//     // Configure CC1S bits to 01 (IC1 mapped to TI1)
//     TIM3_CCMR1 = TIM3_CCMR1 & ~(0xFF << 0);  // Clear channel 1 bits (bits 7:0)
//     TIM3_CCMR1 = TIM3_CCMR1 | TIM3_CCMR1_CC1S_TI1;  // Set CC1S = 01 (input, mapped to TI1)
//     // Step 6: Enable channel 1 by setting CC1E bit in CCER
//     TIM3_CCER = TIM3_CCER | TIM3_CCER_CC1E;
//     // Step 7: Enable the timer by setting CEN bit in TIM3_CR1
//     TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
// }

// uint16_t getCaptureTimer3CH1(void)
// {
//     // Check if capture event occurred (CC1IF flag is bit 1)
//     if(TIM3_SR & TIM3_SR_CC1IF)
//     {
//         TIM3_SR = TIM3_SR & ~TIM3_SR_CC1IF;  // Clear CC1IF flag
//         return TIM3_CCR1;
//     }
//     else
//     {
//         return 0;  // No new capture event
//     }
// }

// void enableNVIC_Timer3(void)
// {
//     NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 = TIM3_INTERRUPT_BIT;
// }

// void initTimer3ToInterrupt(uint16_t prescalervalue, uint16_t autoreloadvalue)
// {
    
//     /* Timer 3 APB1 clock enable */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
//     /* Enable the NVIC interrupt for Timer 3 */
//     enableNVIC_Timer3();
    
//     /* Clear any pending flags in the status register */
//     TIM3_SR = 0;
    
//     /* Set Prescaler value */
//     TIM3_PSC = prescalervalue;
    
//     /* Set Auto-reload value (defines the period) */
//     TIM3_ARR = autoreloadvalue;
    
//     /* Set Compare value for Channel 3 (25% of period) */
//     TIM3_CCR3 = autoreloadvalue / 4;
    
//     /* Enable Preload register for Channel 3 */
//     TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_OC3PE;
    
//     /* Configure Channel 3 as output (CC3S = 00) */
//     TIM3_CCMR2 = TIM3_CCMR2 & ~TIM3_CCMR2_CC3S_MASK;  // Clear CC3S bits
//     TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_CC3S_OUTPUT;
    
//     /* Enable Channel 3 output */
//     TIM3_CCER = TIM3_CCER | TIM3_CCER_CC3E;
    
//     /* Enable interrupts for:
//      * - Update event (counter overflow)
//      * - Channel 3 Compare match
//      */
//     TIM3_DIER = TIM3_DIER | (TIM3_DIER_UIE | TIM3_DIER_CC3IE);
    
//     /* Enable Timer 3 counter */
//     TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
// }

// void TIM3_ClearCC3Flag(void)
// {
//     TIM3_SR = TIM3_SR & ~TIM3_SR_CC3IF;
// }

// void TIM3_ClearUpdateFlag(void)
// {
//     TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
// }

// uint8_t TIM3_GetUpdateFlag(void)
// {
//     return (TIM3_SR & TIM3_SR_UIF) && (TIM3_DIER & TIM3_DIER_UIE);
// }

// uint8_t TIM3_GetCC3Flag(void){
//     return (TIM3_SR & TIM3_SR_CC3IF) && (TIM3_DIER & TIM3_DIER_CC3IE);
// }



// void Timer3_ComputePrescalerAndARR(uint32_t timer_clock_hz,
//                                    uint32_t delay_ms,
//                                    uint16_t *prescaler_out,
//                                    uint16_t *autoreload_out)
// {
//     /*
//      * Formula:
//      *   period_seconds = (PSC + 1) * (ARR + 1) / timer_clock
//      *   delay_ms / 1000 = (PSC + 1) * (ARR + 1) / timer_clock
//      *   → (PSC + 1) * (ARR + 1) = timer_clock * delay_ms / 1000
//      */

//     uint32_t target_counts = (timer_clock_hz / 1000U) * delay_ms;
//     uint32_t prescaler = 0U;
//     uint32_t autoreload = 0U;

//     /* we constrain ARR to 16-bit range (max 65535) for simplicity */
//     if (target_counts <= 65535U)
//     {
//         prescaler = 0U;
//         autoreload = target_counts - 1U;
//     }
//     else
//     {
//         /* find prescaler so ARR fits into 16 bits */
//         prescaler = (target_counts / 65535U);
//         if (prescaler > 0U)
//         {
//             prescaler = prescaler - 1U;
//         }

//         autoreload = (target_counts / (prescaler + 1U)) - 1U;
//     }

//     *prescaler_out = (uint16_t)prescaler;
//     *autoreload_out = (uint16_t)autoreload;
// }

// void timer3_clearSR(void)
// {
//     TIM3_SR  = TIM3_SR & ~TIM3_SR;
// }
// void timer3_stopC1(void)
// {
//     TIM3_CR1 = TIM3_CR1 & ~TIM3_CR1;
// }
// uint8_t timer3_checkUIF(void)
// {
//     return TIM3_SR & TIM3_SR_UIF;
// }

// void timer3_pwm_dual_init_B01(void)
// {
//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

//     // PB0 -> TIM3_CH3, PB1 -> TIM3_CH4
//     initGpioBxAsAF2(0);
//     initGpioBxAsAF2(1);

//     // 30 kHz: ARR = 2799, PSC = 0  (84 MHz / (0+1) / (2799+1) = 30 kHz)
//     TIM3_PSC = 0;
//     TIM3_ARR = 2799;

//     // Channel 3: PWM1 + preload
//     TIM3_CCMR2 = (TIM3_CCMR2 & ~(TIM3_CCMR2_OC3M_MASK | TIM3_CCMR2_CC3S_MASK))
//                | TIM3_CCMR2_OC3M_PWM1 | TIM3_CCMR2_OC3PE;

//     // Channel 4: PWM1 + preload (bits [14:12] are OC4M, [8] is OC4PE)
//     // Reuse CCMR2: CH4 shares CCMR2 high bits
//     TIM3_CCMR2 |= (0x6U << 12) | (1U << 11);  // OC4M=110 (PWM1), OC4PE=1

//     // Enable CH3 and CH4 outputs (active high)
//     TIM3_CCER |= TIM3_CCER_CC3E | TIM3_CCER_CC4E;

//     // start with 0% duty
//     TIM3_CCR3 = 0;
//     TIM3_CCR4 = 0;

//     // ARPE (auto-reload preload) so period updates are buffered (optional)
//     TIM3_CR1 |= (1U << 7);

//     // Counter enable
//     TIM3_CR1 |= TIM3_CR1_CEN;
// }