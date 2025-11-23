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

/* TIM4 Base Address (APB1) */
#define TIM4_BASE_ADDRESS   ((uint32_t)0x40000800)

/* TIM4 Register Addresses (Base + Offset) */
#define TIM4_CR1                                (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x00))
#define TIM4_DIER                               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x0C))
#define TIM4_SR                                 (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x10))
#define TIM4_CCMR1              (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x18))
#define TIM4_CCMR2              (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x1C))
#define TIM4_CCER               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x20))
#define TIM4_CNT                (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x24))
#define TIM4_PSC                (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x28))
#define TIM4_ARR                (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x2C))
#define TIM4_CCR1               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x34))
#define TIM4_CCR2               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x38))
#define TIM4_CCR3               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x3C))
#define TIM4_CCR4               (*(volatile uint32_t *)(TIM4_BASE_ADDRESS + 0x40))

/* NVIC Base Addresses */
#define SYSTEM_CONTROL_BASE_ADDRESS             ((uint32_t)0xE000E000)
#define NVIC_BASE_ADDRESS                       (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

/* NVIC Interrupt Set-Enable Registers */
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31     (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x00))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x04))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x08))

/* TIM4 IRQ: vector table position 37 → ISER[1] bit (37 - 32 = 5) */
#define TIM4_INTERRUPT_BIT_ISER1                    (1UL << 5)

/* TIM4_CR1 Bit Definitions */
#define TIM4_CR1_CEN        (1U << 0)    /* Counter Enable */
#define TIM4_CR1_ARPE       (1U << 7)    /* Auto-reload preload */

/* TIM4_DIER Bit Definitions (DMA/Interrupt Enable Register) */
#define TIM4_DIER_UIE       (1U << 0)    /* Update Interrupt Enable */
#define TIM4_DIER_CC1IE     (1U << 1)    /* Capture/Compare 1 Interrupt Enable */
#define TIM4_DIER_CC2IE     (1U << 2)    /* Capture/Compare 2 Interrupt Enable */
#define TIM4_DIER_CC3IE     (1U << 3)    /* Capture/Compare 3 Interrupt Enable */
#define TIM4_DIER_CC4IE     (1U << 4)    /* Capture/Compare 4 Interrupt Enable */

/* TIM4_SR Bit Definitions (Status Register) */
#define TIM4_SR_UIF         (1U << 0)    /* Update Interrupt Flag */
#define TIM4_SR_CC1IF       (1U << 1)    /* Capture/Compare 1 Interrupt Flag */
#define TIM4_SR_CC2IF       (1U << 2)    /* Capture/Compare 2 Interrupt Flag */
#define TIM4_SR_CC3IF       (1U << 3)    /* Capture/Compare 3 Interrupt Flag */
#define TIM4_SR_CC4IF       (1U << 4)    /* Capture/Compare 4 Interrupt Flag */

/* TIM4_CCMR1 Bit Definitions (for Channel 1/2) */
#define TIM4_CCMR1_CC1S_OUTPUT   (0x0U << 0)   /* CC1S bits [1:0] = 00 (output) */
#define TIM4_CCMR1_CC1S_MASK     (0x3U << 0)   /* mask to clear CC1S */
#define TIM4_CCMR1_OC1PE         (1U   << 3)   /* preload enable for OC1 */
#define TIM4_CCMR1_OC1M_PWM1     (0x6U << 4)   /* OC1M bits [6:4] = 110 (PWM1) */
#define TIM4_CCMR1_OC1M_MASK     (0x7U << 4)   /* mask to clear OC1M */

/* TIM4_CCMR2 Bit Definitions (for Channel 3/4) */
#define TIM4_CCMR2_CC3S_OUTPUT   (0x0U << 0)   /* CC3S bits [1:0] = 00 (output) */
#define TIM4_CCMR2_CC3S_MASK     (0x3U << 0)   /* mask to clear CC3S */
#define TIM4_CCMR2_OC3PE         (1U   << 3)   /* preload enable for OC3 */
#define TIM4_CCMR2_OC3M_TOGGLE   (0x3U << 4)   /* OC3M bits [6:4] = 011 (toggle) */
#define TIM4_CCMR2_OC3M_PWM1     (0x6U << 4)   /* OC3M bits [6:4] = 110 (PWM1) */
#define TIM4_CCMR2_OC3M_MASK     (0x7U << 4)   /* mask to clear OC3M */

/* TIM4_CCER Bit Definitions (Capture/Compare Enable Register) */
#define TIM4_CCER_CC1E           (1U << 0)     /* Capture/Compare 1 enable */
#define TIM4_CCER_CC2E           (1U << 4)     /* Capture/Compare 2 enable */
#define TIM4_CCER_CC3E           (1U << 8)     /* Capture/Compare 3 enable */
#define TIM4_CCER_CC4E           (1U << 12)    /* Capture/Compare 4 enable */

/* function definitions----------------------------------------------------------*/

void timer4_init(uint16_t prescale_value, uint16_t period)
{
    /* Enable APB1 clock for TIM4 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Stop counter while configuring */
    TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;

    /* Clear status register */
    TIM4_SR = 0;

    /* Upload prescaler and auto-reload */
    TIM4_PSC = prescale_value;
    TIM4_ARR = period;

    /* Start counter */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
}

void timer4_channel3_output_compare(uint16_t prescale_value, uint16_t period, uint16_t compare_value)
{
    /* Enable APB1 clock for TIM4 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Stop and clear */
    TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
    TIM4_SR  = 0;

    /* Timebase */
    TIM4_PSC = prescale_value;
    TIM4_ARR = period;

    /* Configure CH3 as output, toggle on match (CCMR2) */
    TIM4_CCMR2 = TIM4_CCMR2 & ~(TIM4_CCMR2_CC3S_MASK | TIM4_CCMR2_OC3M_MASK);
    TIM4_CCMR2 = TIM4_CCMR2 |  TIM4_CCMR2_CC3S_OUTPUT | TIM4_CCMR2_OC3M_TOGGLE;

    /* Compare value */
    TIM4_CCR3 = compare_value;

    /* Enable CH3 output */
    TIM4_CCER = TIM4_CCER | TIM4_CCER_CC3E;

    /* Start counter */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
}

void timer4_ch1_input_capture_init(uint16_t prescale_value, uint16_t period)
{
    /* Enable APB1 clock for TIM4 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Stop and clear */
    TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
    TIM4_SR  = 0;

    /* Timebase */
    TIM4_PSC = prescale_value;
    TIM4_ARR = period;

    /* CC1S = 01 (input on TI1); leave filter/prescaler at reset */
    TIM4_CCMR1 = (TIM4_CCMR1 & ~TIM4_CCMR1_CC1S_MASK) | (0x1U << 0);

    /* Enable capture on CH1 */
    TIM4_CCER = TIM4_CCER | TIM4_CCER_CC1E;

    /* Start counter */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
}

uint16_t getCaptureTimer4CH1(void)
{
    /* Check CC1 capture flag */
    if ((TIM4_SR & TIM4_SR_CC1IF) > 0U)
    {
        uint16_t value = (uint16_t)TIM4_CCR1;
        /* Clear CC1IF correctly (mask the bit off) */
        TIM4_SR = TIM4_SR & ~TIM4_SR_CC1IF;
        return value;
    }
    else
    {
        return 0;
    }
}

void enableNVIC_Timer4(void)
{
    /* Enable TIM4 global interrupt: ISER[1] bit 5 */
    NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63 = TIM4_INTERRUPT_BIT_ISER1;
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

void initTimer4PWM(uint16_t prescalervalue, uint16_t autoreloadvalue, float duty_cycle)
{
    /* APB1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Stop and clear */
    TIM4_CR1 = TIM4_CR1 & ~TIM4_CR1_CEN;
    TIM4_SR  = 0;

    /* Timebase */
    TIM4_PSC = prescalervalue;
    TIM4_ARR = autoreloadvalue;

    /* Clamp duty cycle */
    if (duty_cycle < 0.0f) { duty_cycle = 0.0f; }
    if (duty_cycle > 1.0f) { duty_cycle = 1.0f; }

    /* Duty on CH3 */
    TIM4_CCR3 = (uint16_t)((float)autoreloadvalue * duty_cycle);

    /* CH3 output + preload + PWM1 */
    TIM4_CCMR2 = (TIM4_CCMR2 & ~TIM4_CCMR2_CC3S_MASK) | TIM4_CCMR2_CC3S_OUTPUT;
    TIM4_CCMR2 = TIM4_CCMR2 | TIM4_CCMR2_OC3PE;
    TIM4_CCMR2 = (TIM4_CCMR2 & ~TIM4_CCMR2_OC3M_MASK) | TIM4_CCMR2_OC3M_PWM1;

    /* Enable CH3 output */
    TIM4_CCER = TIM4_CCER | TIM4_CCER_CC3E;

    /* Enable ARR preload */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_ARPE;

    /* Start counter */
    TIM4_CR1 = TIM4_CR1 | TIM4_CR1_CEN;
}
