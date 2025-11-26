/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_TIMER3_H_
#define __HARDWARE_STM_TIMER3_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"

/* Macros for Everyone--------------------------------------------------------*/
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10
#define TIM3_BASE_ADDRESS   ((uint32_t)0x40000400)
#define TIM3_ARR                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x2C))
#define TIM3_CCR3               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x3C))
#define TIM3_CCR4               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x40))


/*Register addresses*/


// TIM3 Base Address
#define TIM3_BASE_ADDRESS   ((uint32_t)0x40000400)

// TIM3 Register Addresses (Base + Offset) - directly accessible as registers
#define TIM3_CR1                                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x00))
// this one is TIM3_INTERRUPT_ENABLE_REGISTER 
#define TIM3_DIER               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x0C))
#define TIM3_SR                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x10))
#define TIM3_CCMR1              (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x18))
#define TIM3_CCMR2              (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x1C))
#define TIM3_CCER               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x20))
#define TIM3_CNT                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x24))
#define TIM3_PSC                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x28))
#define TIM3_ARR                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x2C))
#define TIM3_CCR1               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x34))
#define TIM3_CCR2               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x38))
#define TIM3_CCR3               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x3C))
#define TIM3_CCR4               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x40))

// TIM3 Register Addresses - directly accessible as registers
#define TIM3_CR1_REGISTER_1                     (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x00))
#define TIMER3_DMA_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x0C))
#define TIM3_STATUS_REGISTER                    (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x10))
#define TIM3_CAPTURE_COMPARE_MODE_1_REGISTER    (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x18))
#define TIM3_CAPTURE_COMPARE_MODE_2_REGISTER    (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x1C))
#define TIM3_CAPTURE_COMPARE_ENABLE_REGISTER    (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x20))
#define TIM3_PRESCALER_REGISTER                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x28))
#define TIM3_AUTORELOAD_REGISTER                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x2C))
#define TIM3_COMPARE_1_REGISTER                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x34))
#define TIM3_COMPARE_2_REGISTER                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x38))
#define TIM3_COMPARE_3_REGISTER                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x3C))
#define TIM3_COMPARE_4_REGISTER                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x40))

/* NVIC Base Addresses */
#define SYSTEM_CONTROL_BASE_ADDRESS             ((uint32_t)0xE000E000)
#define NVIC_BASE_ADDRESS                       (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

/* NVIC Interrupt Set-Enable Registers */
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31     (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x00))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x04))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95    (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x08))

/* Timer 3 Interrupt Bit (position 29 in NVIC) */
#define TIM3_INTERRUPT_BIT                      (1 << 29)   // Or (0x20000000)

// Flags for CR1 register
#define COUNTER_ENABLE_BIT                      (0x01)

// Flags for Interrupt Enable register
#define TIM_UPDATE_INTERRUPT_ENABLE             (0x01)  // Timer overflow or event interrupt
#define TIM_CH3_CC_INTERRUPT_ENABLE             (0x08)  // Timer channel 3 capture/compare interrupt

#define TIM_CCMR13_OC1M_0 (0b00010000)
#define TIM_CCMR13_OC1M_1 (0b00100000)
#define TIM_CCMR13_OC1M_2 (0b01000000)
#define TIM_CCMR13_OCPE (0b00001000)
#define TIM_CCMR13_OUTPUT 0x00

// TIM3_CR1 Bit Definitions
#define TIM3_CR1_CEN        (1 << 0)    // Counter Enable bit

// TIM3_DIER Bit Definitions (DMA/Interrupt Enable Register)
#define TIM3_DIER_UIE       (1 << 0)    // Update Interrupt Enable
#define TIM3_DIER_CC1IE     (1 << 1)    // Capture/Compare 1 Interrupt Enable
#define TIM3_DIER_CC2IE     (1 << 2)    // Capture/Compare 2 Interrupt Enable
#define TIM3_DIER_CC3IE     (1 << 3)    // Capture/Compare 3 Interrupt Enable
#define TIM3_DIER_CC4IE     (1 << 4)    // Capture/Compare 4 Interrupt Enable

#define TIM3_DIER_UDE       (1 << 8)     // Update DMA request Enable
#define TIM3_DIER_CC1DE     (1 << 9)     // Capture/Compare 1 DMA request Enable
#define TIM3_DIER_CC2DE     (1 << 10)    // Capture/Compare 2 DMA request Enable
#define TIM3_DIER_CC3DE     (1 << 11)    // Capture/Compare 3 DMA request Enable
#define TIM3_DIER_CC4DE     (1 << 12)    // Capture/Compare 4 DMA request Enable


// TIM3_SR Bit Definitions (Status Register)
#define TIM3_SR_UIF         (1 << 0)    // Update Interrupt Flag
#define TIM3_SR_CC1IF       (1 << 1)    // Capture/Compare 1 Interrupt Flag
#define TIM3_SR_CC2IF       (1 << 2)    // Capture/Compare 2 Interrupt Flag
#define TIM3_SR_CC3IF       (1 << 3)    // Capture/Compare 3 Interrupt Flag
#define TIM3_SR_CC4IF       (1 << 4)    // Capture/Compare 4 Interrupt Flag

// TIM3_CCMR1 Bit Definitions (for Channel 1,2)
#define TIM3_CCMR1_CC1S_OUTPUT  (0x0 << 0)  // CC1 channel configured as output
#define TIM3_CCMR1_CC2S_OUTPUT  (0x0 << 8)  // CC2 channel configured as output
#define TIM3_CCMR1_CC1S_TI1     (0x1 << 0)  // CC1 channel configured as input, mapped to TI1
#define TIM3_CCMR1_CC1S_TI2     (0x2 << 0)  // CC1 channel configured as input, mapped to TI2
#define TIM3_CCMR1_IC1PSC       (0x3 << 2)  // Input Capture 1 Prescaler
#define TIM3_CCMR1_IC1F         (0xF << 4)  // Input Capture 1 Filter
#define TIM3_CCMR1_OC1FE        (1 << 2)    // Output Compare 1 Fast enable
#define TIM3_CCMR1_OC1PE        (1 << 3)    // Output Compare 1 Preload enable
#define TIM3_CCMR1_OC2PE        (1 << 11)    // Output Compare 2 Preload enable
#define TIM3_CCMR1_OC1M_TOGGLE  (0x3 << 4)  // OC1M = 011 (Toggle mode)
#define TIM3_CCMR1_OC1M_PWM1    (0x6 << 4)  // OC1M = 110 (PWM mode 1)
#define TIM3_CCMR1_OC2M_PWM1    (0x6 << 12)  // OC2M = 110 (PWM mode 1)
#define TIM3_CCMR1_OC1M_PWM2    (0x7 << 4)  // OC1M = 111 (PWM mode 2)

// TIM3_CCMR1 Bit Clear Masks
#define TIM3_CCMR1_CC2S_MASK    (0x3 << 8)   /* CC2S bits [9:8] */
#define TIM3_CCMR1_OC2M_MASK    (0x7 << 12)   /* OC2M bits [14:12] */

// TIM3_CCMR2 Bit Definitions (for Channel 3,4)
#define TIM3_CCMR2_CC3S_OUTPUT  (0x0 << 0)  // CC3 channel configured as output
#define TIM3_CCMR2_OC3FE        (1 << 2)    // Output Compare 3 Fast enable
#define TIM3_CCMR2_OC3PE        (1 << 3)    // Output Compare 3 Preload enable
#define TIM3_CCMR2_OC3M_TOGGLE  (0x3 << 4)  // OC3M = 011 (Toggle mode)
#define TIM3_CCMR2_OC3M_PWM1    (0x6 << 4)  // OC3M = 110 (PWM mode 1)
#define TIM3_CCMR2_OC3M_PWM2    (0x7 << 4)  // OC3M = 111 (PWM mode 2)
// TIM3_CCMR2 Bit Clear Masks
#define TIM3_CCMR2_CC3S_MASK    (0x3 << 0)   /* CC3S bits [1:0] */
#define TIM3_CCMR2_OC3M_MASK    (0x7 << 4)   /* OC3M bits [6:4] */

// TIM3_CCER Bit Definitions (Capture/Compare Enable Register)
#define TIM3_CCER_CC1E          (1 << 0)    // Capture/Compare 1 enable
#define TIM3_CCER_CC1P          (1 << 1)    // Capture/Compare 1 polarity
#define TIM3_CCER_CC2E          (1 << 4)    // Capture/Compare 2 enable
#define TIM3_CCER_CC2P          (1 << 5)    // Capture/Compare 2 polarity
#define TIM3_CCER_CC3E          (1 << 8)    // Capture/Compare 3 enable
#define TIM3_CCER_CC3P          (1 << 9)    // Capture/Compare 3 polarity
#define TIM3_CCER_CC4E          (1 << 12)   // Capture/Compare 4 enable
#define TIM3_CCER_CC4P          (1 << 13)   // Capture/Compare 4 polarity

/*Function definitions---------------------------------------------------------*/
void timer3_init(uint16_t,uint16_t);
void timer3_channel3_output_compare(uint16_t,uint16_t,uint16_t );
void timer3_ch1_input_capture_init(uint16_t,uint16_t);
uint16_t getCaptureTimer3CH1(void);
void enableNVIC_Timer3(void);
void initTimer3ToInterrupt(uint16_t,uint16_t);
void TIM3_ClearUpdateFlag(void);
void TIM3_ClearCC3Flag(void);
uint8_t TIM3_GetUpdateFlag(void);
uint8_t TIM3_GetCC3Flag(void);
void TIM3_IRQHandler(void);
void initTimer3PWM(uint16_t,uint16_t);
void timer3_set_duty(uint16_t arr_value);
void Timer3_ComputePrescalerAndARR(uint32_t ,
                                   uint32_t ,
                                   uint16_t *,
                                   uint16_t *);
void timer3_clearSR(void);
void timer3_stopC1(void);
uint8_t timer3_checkUIF(void);
void timer3_pwm_dual_init_B01(void);
static inline uint16_t tim3_arr(void) { return (uint16_t)TIM3_ARR; }
/*
static inline void tim3_set_ch3_duty(uint16_t duty)
{
    if (duty > tim3_arr()) duty = tim3_arr();
    TIM3_CCR3 = duty;
}

static inline void tim3_set_ch4_duty(uint16_t duty)
{
    if (duty > tim3_arr()) duty = tim3_arr();
    TIM3_CCR4 = duty;
}

static inline void pwm3_low(void)  { tim3_set_ch3_duty(0); }
static inline void pwm3_high(void) { tim3_set_ch3_duty(tim3_arr()); }
static inline void pwm4_low(void)  { tim3_set_ch4_duty(0); }
static inline void pwm4_high(void) { tim3_set_ch4_duty(tim3_arr()); }
*/
#ifdef __cplusplus
}
#endif

#endif /*__TIMER3_H*/