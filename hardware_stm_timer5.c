/**
  ******************************************************************************
  * @file    hardware_stm_timer5.c
  * @author  paussava
  * @version 1.0
  * @date    Nov-2025
  * @brief   Minimal TIM5 update-IRQ test: blink PB0 in ISR
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "hardware_stm_gpio.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"
#include <stdint.h>
#include <stdio.h>

/* =========================
 *       TIM5 REGISTERS
 * ========================= */
#define TIM5_BASE_ADDRESS   ((uint32_t)0x40000C00) /* APB1 */

/* 16-bit control / status registers */
#define TIM5_CR1    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x00))
#define TIM5_CR2    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x04))
#define TIM5_SMCR   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x08))
#define TIM5_DIER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x0C))
#define TIM5_SR     (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x10))
#define TIM5_EGR    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x14))
#define TIM5_CCMR1  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x18))
#define TIM5_CCMR2  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x1C))
#define TIM5_CCER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x20))
#define TIM5_PSC    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x28))

/* 32-bit counter / data registers */
#define TIM5_CNT    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x24))
#define TIM5_ARR    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x2C))
#define TIM5_CCR1   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x34))
#define TIM5_CCR2   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x38))
#define TIM5_CCR3   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x3C))
#define TIM5_CCR4   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x40))

/* --- Update --- */
#define TIM5_SR_UIF        (1U << 0)   /* Update interrupt flag */

/* --- Capture/Compare --- */
#define TIM5_SR_CC1IF      (1U << 1)   /* Capture/Compare 1 interrupt flag */
#define TIM5_SR_CC2IF      (1U << 2)   /* Capture/Compare 2 interrupt flag */
#define TIM5_SR_CC3IF      (1U << 3)   /* Capture/Compare 3 interrupt flag */
#define TIM5_SR_CC4IF      (1U << 4)   /* Capture/Compare 4 interrupt flag */
#define TIM5_SR_CCxIF      (TIM5_SR_CC1IF | TIM5_SR_CC2IF | TIM5_SR_CC3IF | TIM5_SR_CC4IF)

/* --- COM / Trigger / Break --- */
#define TIM5_SR_COMIF      (1U << 5)   /* COM interrupt flag (not used in TIM5) */
#define TIM5_SR_TIF        (1U << 6)   /* Trigger interrupt flag */
#define TIM5_SR_BIF        (1U << 7)   /* Break interrupt flag (not used in TIM5) */

/* --- Capture/Compare Overcapture --- */
#define TIM5_SR_CC1OF      (1U << 9)   /* Capture/Compare 1 overcapture flag */
#define TIM5_SR_CC2OF      (1U << 10)  /* Capture/Compare 2 overcapture flag */
#define TIM5_SR_CC3OF      (1U << 11)  /* Capture/Compare 3 overcapture flag */
#define TIM5_SR_CC4OF      (1U << 12)  /* Capture/Compare 4 overcapture flag */

#define TIM5_SR_CCxOF      (TIM5_SR_CC1OF | TIM5_SR_CC2OF | TIM5_SR_CC3OF | TIM5_SR_CC4OF)

#define TIM5_SR_ALL_IRQ    (TIM5_SR_UIF | TIM5_SR_CCxIF | TIM5_SR_TIF)
#define TIM5_SR_ALL_FLAGS  (TIM5_SR_ALL_IRQ | TIM5_SR_CCxOF)

/* =========================
 *       Bit Definitions
 * ========================= */
/* CR1 */
#define TIM5_CR1_CEN        (1U << 0)
#define TIM5_CR1_URS        (1U << 2)
#define TIM5_CR1_ARPE       (1U << 7)

/* DIER */
#define TIM5_DIER_UIE       (1U << 0)

/* =========================
 *          NVIC
 * ========================= */
#define SYSTEM_CONTROL_BASE_ADDRESS             ((uint32_t)0xE000E000)
#define NVIC_BASE_ADDRESS                       (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31     (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x00))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x04))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x08))

#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x180))
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_32_63 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x184))
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_64_95 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x188))

/* TIM5 global interrupt on STM32F446: IRQn = 50 → ISER[1] bit 18 */
#define TIM5_INTERRUPT_BIT_ISER1                    (1UL << 18)

uint32_t tim5_arr(void) { return TIM5_ARR; }

void tim5_set_ch2_duty(uint32_t duty)
{
    uint32_t arr = tim5_arr();
    if (duty > arr) duty = arr;
    TIM5_CCR2 = duty;
}

void tim5_set_ch4_duty(uint32_t duty)
{
    uint32_t arr = tim5_arr();
    if (duty > arr) duty = arr;
    TIM5_CCR4 = duty;
}

void timer5_pwm_dual_init_A03_A01(void)
{
    /* 1) Enable APB1 clock for TIM5 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    initGpioAxAsAF2(1);   // PA1 → TIM5_CH2 (PWM A)
    initGpioAxAsAF2(3);   // PA3 → TIM5_CH4 (PWM B)

    /* 3) Time base: 30 kHz (for example)
       90 MHz / (PSC+1) / (ARR+1) = 30 kHz → ARR = 2999, PSC = 0
    */
    TIM5_PSC = 0;
    TIM5_ARR = 2799;

    /* 4) Configure Channel 2 (CCMR1 high half) → PWM1 mode + preload */
    TIM5_CCMR1 &= ~(0xFF00U);               // clear CC2S + OC2M + OC2PE bits
    TIM5_CCMR1 |= (6U << 12) | (1U << 11);  // OC2M=110 (PWM1), OC2PE=1

    /* 5) Configure Channel 4 (CCMR2 high half) → PWM1 mode + preload */
    TIM5_CCMR2 &= ~(0xFF00U);               // clear CC4S + OC4M + OC4PE bits
    TIM5_CCMR2 |= (6U << 12) | (1U << 11);  // OC4M=110 (PWM1), OC4PE=1

    /* 6) Enable outputs (active high) */
    TIM5_CCER &= ~((0xF << 4) | (0xF << 12));  // clear both channel groups
    TIM5_CCER |= (1U << 4) | (1U << 12);       // enable CH2, CH4 active-high


    /* 7) Start with 0% duty cycle */
    TIM5_CCR2 = 0;
    TIM5_CCR4 = 0;

    /* 8) Enable auto-reload preload and generate update event */
    TIM5_CR1 |= TIM5_CR1_ARPE;
    TIM5_EGR  = 1U;   // UG to latch PSC/ARR/CCR

    /* 9) Counter enable */
    TIM5_CR1 |= TIM5_CR1_CEN;
}