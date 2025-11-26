// /**
//   ******************************************************************************
//   * @file    hardware_stm_timer5.c
//   * @author  paussava
//   * @version 1.0
//   * @date    Nov-2025
//   * @brief   Minimal TIM5 update-IRQ test: blink PB0 in ISR
//   ******************************************************************************
//   */

// #include "hardware_stm_gpio.h"
// #include "hardware_stm_gpio.h"
// #include "stm32f4xx_mort2.h"
// #include "stm32f4xx_rcc_mort.h"
// #include <stdint.h>
// #include <stdio.h>



// uint32_t tim5_arr(void) { return TIM5_ARR; }

// void tim5_set_ch2_duty(uint32_t duty)
// {
//     uint32_t arr = tim5_arr();
//     if (duty > arr) duty = arr;
//     TIM5_CCR2 = duty;
// }

// void tim5_set_ch4_duty(uint32_t duty)
// {
//     uint32_t arr = tim5_arr();
//     if (duty > arr) duty = arr;
//     TIM5_CCR4 = duty;
// }

// void timer5_pwm_dual_init_A03_A01(void)
// {
//     /* 1) Enable APB1 clock for TIM5 */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//     initGpioAxAsAF2(1);   // PA1 → TIM5_CH2 (PWM A)
//     initGpioAxAsAF2(3);   // PA3 → TIM5_CH4 (PWM B)

//     /* 3) Time base: 30 kHz (for example)
//        90 MHz / (PSC+1) / (ARR+1) = 30 kHz → ARR = 2999, PSC = 0
//     */
//     TIM5_PSC = 0;
//     TIM5_ARR = 2799;

//     /* 4) Configure Channel 2 (CCMR1 high half) → PWM1 mode + preload */
//     TIM5_CCMR1 &= ~(0xFF00U);               // clear CC2S + OC2M + OC2PE bits
//     TIM5_CCMR1 |= (6U << 12) | (1U << 11);  // OC2M=110 (PWM1), OC2PE=1

//     /* 5) Configure Channel 4 (CCMR2 high half) → PWM1 mode + preload */
//     TIM5_CCMR2 &= ~(0xFF00U);               // clear CC4S + OC4M + OC4PE bits
//     TIM5_CCMR2 |= (6U << 12) | (1U << 11);  // OC4M=110 (PWM1), OC4PE=1

//     /* 6) Enable outputs (active high) */
//     TIM5_CCER &= ~((0xF << 4) | (0xF << 12));  // clear both channel groups
//     TIM5_CCER |= (1U << 4) | (1U << 12);       // enable CH2, CH4 active-high


//     /* 7) Start with 0% duty cycle */
//     TIM5_CCR2 = 0;
//     TIM5_CCR4 = 0;

//     /* 8) Enable auto-reload preload and generate update event */
//     TIM5_CR1 |= TIM5_CR1_ARPE;
//     TIM5_EGR  = 1U;   // UG to latch PSC/ARR/CCR

//     /* 9) Counter enable */
//     TIM5_CR1 |= TIM5_CR1_CEN;
// }