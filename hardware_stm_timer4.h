#ifndef HARDWARE_STM_TIMER4_H
#define HARDWARE_STM_TIMER4_H


#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

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
#define TIM4_INTERRUPT_BIT_ISER0                    (1UL << 30U)


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

/* Basic timebase */
void timer4_init(uint16_t prescale_value, uint16_t period);

/* Output-compare on CH3: toggle on match */
void timer4_channel3_output_compare(uint16_t prescale_value, uint16_t period, uint16_t compare_value);

/* Input-capture on CH1 */
void timer4_ch1_input_capture_init(uint16_t prescale_value, uint16_t period);
uint16_t getCaptureTimer4CH1(void);

/* NVIC enable for TIM4 global interrupt */
void enableNVIC_Timer4(void);

/* Periodic interrupt demo: Update + CC3 interrupt enabled */
void initTimer4ToInterrupt(uint16_t prescalervalue, uint16_t autoreloadvalue);

/* Flag helpers */
void TIM4_ClearCC3Flag(void);
void TIM4_ClearUpdateFlag(void);
uint8_t TIM4_GetUpdateFlag(void);
uint8_t TIM4_GetCC3Flag(void);

/* PWM on CH3 (PWM mode 1) */
void initTimer4PWM(uint16_t prescalervalue, uint16_t autoreloadvalue, float duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /*__TIMER4_H */
