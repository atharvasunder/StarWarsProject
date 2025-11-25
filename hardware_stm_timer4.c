/**
  ******************************************************************************
  * @file    hardware_stm_timer4.c
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    October-2025
  * @brief   Controls STM32F446ze timer4
  ******************************************************************************
  */

#include "hardware_stm_timer4.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"
#include <stdint.h>

/* MACRO definitions----------------------------------------------------------*/



/* function definitions----------------------------------------------------------*/

void enableNVIC_Timer4(void)
{
    /* Enable TIM4 global interrupt: ISER[1] bit 5 */
    NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63 |= TIM4_INTERRUPT_BIT_ISER1;
}

void initTimer4ToInterrupt(uint16_t prescalervalue, uint16_t autoreloadvalue)
{
    /* APB1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* NVIC */
    enableNVIC_Timer4();

    /* Stop and clear */
    TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
    TIM4_SR  = 0;

    /* Timebase */
    TIM4_PSC = prescalervalue;
    TIM4_ARR = autoreloadvalue;

    /* Compare at 25% */
    // TIM4_CCR3 = (uint16_t)(autoreloadvalue / 4U);

    /* CH3 output + preload */
    TIM4_CCMR2 = (TIM4_CCMR2 & ~TIM4_CCMR2_CC3S_MASK) | TIM4_CCMR2_CC3S_OUTPUT;
    TIM4_CCMR2 = TIM4_CCMR2 | TIM4_CCMR2_OC3PE;

    /* Enable CH3 output */
    TIM4_CCER = TIM4_CCER | TIM4_CCER_CC3E;

    /* Enable Update and CC3 interrupts */
    TIM4_DIER = TIM4_DIER | (TIM4_DIER_UIE); // | TIM4_DIER_CC3IE);

    /* Start counter */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
}

void TIM4_ClearCC3Flag(void)
{
    TIM4_SR = TIM4_SR & ~TIM4_SR_CC3IF;
}

void TIM4_ClearUpdateFlag(void)
{
    TIM4_SR = TIM4_SR & ~TIM4_SR_UIF;
}

uint8_t TIM4_GetUpdateFlag(void)
{
    uint8_t set_in_SR = (TIM4_SR & TIM4_SR_UIF) ? 1U : 0U;
    uint8_t enabled   = (TIM4_DIER & TIM4_DIER_UIE) ? 1U : 0U;
    return (uint8_t)(set_in_SR && enabled);
}

uint8_t TIM4_GetCC3Flag(void)
{
    uint8_t set_in_SR = (TIM4_SR & TIM4_SR_CC3IF) ? 1U : 0U;
    uint8_t enabled   = (TIM4_DIER & TIM4_DIER_CC3IE) ? 1U : 0U;
    return (uint8_t)(set_in_SR && enabled);
}

// void timer4_init(uint16_t prescale_value, uint16_t period)
// {
//     /* Enable APB1 clock for TIM4 */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//     /* Stop counter while configuring */
//     TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;

//     /* Clear status register */
//     TIM4_SR = 0;

//     /* Upload prescaler and auto-reload */
//     TIM4_PSC = prescale_value;
//     TIM4_ARR = period;

//     /* Start counter */
//     TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
// }

// void timer4_channel3_output_compare(uint16_t prescale_value, uint16_t period, uint16_t compare_value)
// {
//     /* Enable APB1 clock for TIM4 */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//     /* Stop and clear */
//     TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
//     TIM4_SR  = 0;

//     /* Timebase */
//     TIM4_PSC = prescale_value;
//     TIM4_ARR = period;

//     /* Configure CH3 as output, toggle on match (CCMR2) */
//     TIM4_CCMR2 = TIM4_CCMR2 & ~(TIM4_CCMR2_CC3S_MASK | TIM4_CCMR2_OC3M_MASK);
//     TIM4_CCMR2 = TIM4_CCMR2 |  TIM4_CCMR2_CC3S_OUTPUT | TIM4_CCMR2_OC3M_TOGGLE;

//     /* Compare value */
//     TIM4_CCR3 = compare_value;

//     /* Enable CH3 output */
//     TIM4_CCER = TIM4_CCER | TIM4_CCER_CC3E;

//     /* Start counter */
//     TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
// }

// void timer4_ch1_input_capture_init(uint16_t prescale_value, uint16_t period)
// {
//     /* Enable APB1 clock for TIM4 */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//     /* Stop and clear */
//     TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
//     TIM4_SR  = 0;

//     /* Timebase */
//     TIM4_PSC = prescale_value;
//     TIM4_ARR = period;

//     /* CC1S = 01 (input on TI1); leave filter/prescaler at reset */
//     TIM4_CCMR1 = (TIM4_CCMR1 & ~TIM4_CCMR1_CC1S_MASK) | (0x1U << 0);

//     /* Enable capture on CH1 */
//     TIM4_CCER = TIM4_CCER | TIM4_CCER_CC1E;

//     /* Start counter */
//     TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
// }

// uint16_t getCaptureTimer4CH1(void)
// {
//     /* Check CC1 capture flag */
//     if ((TIM4_SR & TIM4_SR_CC1IF) > 0U)
//     {
//         uint16_t value = (uint16_t)TIM4_CCR1;
//         /* Clear CC1IF correctly (mask the bit off) */
//         TIM4_SR = TIM4_SR & ~TIM4_SR_CC1IF;
//         return value;
//     }
//     else
//     {
//         return 0;
//     }
// }



// void initTimer4PWM(uint16_t prescalervalue, uint16_t autoreloadvalue, float duty_cycle)
// {
//     /* APB1 clock */
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//     /* Stop and clear */
//     TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
//     TIM4_SR  = 0;

//     /* Timebase */
//     TIM4_PSC = prescalervalue;
//     TIM4_ARR = autoreloadvalue;

//     /* Clamp duty cycle */
//     if (duty_cycle < 0.0f) { duty_cycle = 0.0f; }
//     if (duty_cycle > 1.0f) { duty_cycle = 1.0f; }

//     /* Duty on CH3 */
//     TIM4_CCR3 = (uint16_t)((float)autoreloadvalue * duty_cycle);

//     /* CH3 output + preload + PWM1 */
//     TIM4_CCMR2 = (TIM4_CCMR2 & ~TIM4_CCMR2_CC3S_MASK) | TIM4_CCMR2_CC3S_OUTPUT;
//     TIM4_CCMR2 = TIM4_CCMR2 | TIM4_CCMR2_OC3PE;
//     TIM4_CCMR2 = (TIM4_CCMR2 & ~TIM4_CCMR2_OC3M_MASK) | TIM4_CCMR2_OC3M_PWM1;

//     /* Enable CH3 output */
//     TIM4_CCER = TIM4_CCER | TIM4_CCER_CC3E;

//     /* Enable ARR preload */
//     TIM4_CR1 = TIM4_CR1 | TIM4_CR1_ARPE;

//     /* Start counter */
//     TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
// }
