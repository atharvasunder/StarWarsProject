// /**
//   ******************************************************************************
//   * @file    hardware_stm_timer5.h
//   * @author  paussava
//   * @version 1.0
//   * @date    Nov-2025
//   * @brief   Header for STM32F446ZE Timer5 driver
//   ******************************************************************************
//   */

// #ifndef __HARDWARE_STM_TIMER5_H
// #define __HARDWARE_STM_TIMER5_H

// #ifdef __cplusplus
// extern "C" {
// #endif

// #include <stdint.h>
// #define TIM5_BASE_ADDRESS   ((uint32_t)0x40000C00) /* APB1 */
// #define TIM5_ARR    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x2C))
// #define TIM5_CCR2   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x38))
// #define TIM5_CCR4   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x40))
// #define TIM5_EGR    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x14))
// #define TIM5_CCMR1  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x18))
// #define TIM5_CCMR2  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x1C))
// #define TIM5_CCER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x20))
// #define TIM5_CR1    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x00))

// /* =========================
//  *       TIM5 REGISTERS
//  * ========================= */
// #define TIM5_BASE_ADDRESS   ((uint32_t)0x40000C00) /* APB1 */

// /* 16-bit control / status registers */
// #define TIM5_CR1    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x00))
// #define TIM5_CR2    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x04))
// #define TIM5_SMCR   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x08))
// #define TIM5_DIER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x0C))
// #define TIM5_SR     (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x10))
// #define TIM5_EGR    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x14))
// #define TIM5_CCMR1  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x18))
// #define TIM5_CCMR2  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x1C))
// #define TIM5_CCER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x20))
// #define TIM5_PSC    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x28))

// /* 32-bit counter / data registers */
// #define TIM5_CNT    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x24))
// #define TIM5_ARR    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x2C))
// #define TIM5_CCR1   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x34))
// #define TIM5_CCR2   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x38))
// #define TIM5_CCR3   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x3C))
// #define TIM5_CCR4   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x40))

// /* --- Update --- */
// #define TIM5_SR_UIF        (1U << 0)   /* Update interrupt flag */

// /* --- Capture/Compare --- */
// #define TIM5_SR_CC1IF      (1U << 1)   /* Capture/Compare 1 interrupt flag */
// #define TIM5_SR_CC2IF      (1U << 2)   /* Capture/Compare 2 interrupt flag */
// #define TIM5_SR_CC3IF      (1U << 3)   /* Capture/Compare 3 interrupt flag */
// #define TIM5_SR_CC4IF      (1U << 4)   /* Capture/Compare 4 interrupt flag */
// #define TIM5_SR_CCxIF      (TIM5_SR_CC1IF | TIM5_SR_CC2IF | TIM5_SR_CC3IF | TIM5_SR_CC4IF)

// /* --- COM / Trigger / Break --- */
// #define TIM5_SR_COMIF      (1U << 5)   /* COM interrupt flag (not used in TIM5) */
// #define TIM5_SR_TIF        (1U << 6)   /* Trigger interrupt flag */
// #define TIM5_SR_BIF        (1U << 7)   /* Break interrupt flag (not used in TIM5) */

// /* --- Capture/Compare Overcapture --- */
// #define TIM5_SR_CC1OF      (1U << 9)   /* Capture/Compare 1 overcapture flag */
// #define TIM5_SR_CC2OF      (1U << 10)  /* Capture/Compare 2 overcapture flag */
// #define TIM5_SR_CC3OF      (1U << 11)  /* Capture/Compare 3 overcapture flag */
// #define TIM5_SR_CC4OF      (1U << 12)  /* Capture/Compare 4 overcapture flag */

// #define TIM5_SR_CCxOF      (TIM5_SR_CC1OF | TIM5_SR_CC2OF | TIM5_SR_CC3OF | TIM5_SR_CC4OF)

// #define TIM5_SR_ALL_IRQ    (TIM5_SR_UIF | TIM5_SR_CCxIF | TIM5_SR_TIF)
// #define TIM5_SR_ALL_FLAGS  (TIM5_SR_ALL_IRQ | TIM5_SR_CCxOF)

// /* =========================
//  *       Bit Definitions
//  * ========================= */
// /* CR1 */
// #define TIM5_CR1_CEN        (1U << 0)
// #define TIM5_CR1_URS        (1U << 2)
// #define TIM5_CR1_ARPE       (1U << 7)

// /* DIER */
// #define TIM5_DIER_UIE       (1U << 0)

// /* =========================
//  *          NVIC
//  * ========================= */
// #define SYSTEM_CONTROL_BASE_ADDRESS             ((uint32_t)0xE000E000)
// #define NVIC_BASE_ADDRESS                       (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

// #define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31     (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x00))
// #define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x04))
// #define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x08))

// #define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x180))
// #define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_32_63 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x184))
// #define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_64_95 (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x188))

// /* TIM5 global interrupt on STM32F446: IRQn = 50 → ISER[1] bit 18 */
// #define TIM5_INTERRUPT_BIT_ISER1                    (1UL << 18)

// uint32_t tim5_arr(void);
// void tim5_set_ch2_duty(uint32_t duty);
// void tim5_set_ch4_duty(uint32_t duty);
// void timer5_pwm_dual_init_A03_A01(void);

// #ifdef __cplusplus
// }
// #endif

// #endif /* __HARDWARE_STM_TIMER5_H */
