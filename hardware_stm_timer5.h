/**
  ******************************************************************************
  * @file    hardware_stm_timer5.h
  * @author  paussava
  * @version 1.0
  * @date    Nov-2025
  * @brief   Header for STM32F446ZE Timer5 driver
  ******************************************************************************
  */

#ifndef __HARDWARE_STM_TIMER5_H
#define __HARDWARE_STM_TIMER5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define TIM5_BASE_ADDRESS   ((uint32_t)0x40000C00) /* APB1 */
#define TIM5_ARR    (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x2C))
#define TIM5_CCR2   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x38))
#define TIM5_CCR4   (*(volatile uint32_t *)(TIM5_BASE_ADDRESS + 0x40))
#define TIM5_EGR    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x14))
#define TIM5_CCMR1  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x18))
#define TIM5_CCMR2  (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x1C))
#define TIM5_CCER   (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x20))
#define TIM5_CR1    (*(volatile uint16_t *)(TIM5_BASE_ADDRESS + 0x00))

uint32_t tim5_arr(void);
void tim5_set_ch2_duty(uint32_t duty);
void tim5_set_ch4_duty(uint32_t duty);
void timer5_pwm_dual_init_A03_A01(void);

#ifdef __cplusplus
}
#endif

#endif /* __HARDWARE_STM_TIMER5_H */
