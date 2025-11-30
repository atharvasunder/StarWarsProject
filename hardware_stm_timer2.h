/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER2_H_
#define __TIMER2_H_

#include <cstdint>
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
// Register Definition
#define TIM2_BASE_ADDRESS        ((uint32_t)0x40000000)
#define TIM2_CR1_REGISTER_1 (TIM2_BASE_ADDRESS + 0X00) //Timer 3 control register 1 -> accessed as 16 bit register

//flag for CR1 register
#define CLOCK_DIVISION_BITS (uint16_t)0x600
#define COUNTER_ENABLE_BIT (uint16_t)0x01
#define UPDATE_REQUEST_JUST_OVERFLOW    (uint16_t)0X02
#define TIM2_CR1_REGISTER_2 (TIM2_BASE_ADDRESS + 0X04) //timer ctrl regis 2
#define TIM2_COUNTER_REGISTER (TIM2_BASE_ADDRESS + 0X24) //TIMER CNT REGIS 3
#define TIM2_PRESCALER_REGISTER (TIM2_BASE_ADDRESS + 0X28)
#define TIM2_AUTORELOAD_REGISTER (TIM2_BASE_ADDRESS + 0X2C)
#define TIM2_EVENT_GEN_REGISTER (TIM2_BASE_ADDRESS + 0X14)
#define TIM2_STATUS_REGISTER (TIM2_BASE_ADDRESS + 0X10)

//flag for status
#define TIM2_UIF         0X01
#define TIM2_CH1_CC1F    0X02
#define TIM2_CH3_CC3F    0X02 // timer chanel 3 capture/compare interrupt match

#define TIM2_INTERRUPT_ENABLE_REGISTER (TIM2_BASE_ADDRESS + 0x0C)

//flags  for interupt enable register
#define  TIM_CH3_CC_INTERRUPT_ENABLE    0X8
#define TIM_UPDATE_INTERRUPT_ENABLE     0X1

#define TIM2_CAPTURE_COMPARE_MODE1_REGISTER (TIM2_BASE_ADDRESS + 0x18)
#define TIM2_CAPTURE_COMPARE_MODE2_REGISTER (TIM2_BASE_ADDRESS + 0x1C)

// flag for TIM2_cmr register
#define TIM2_CCMR13_OC1M_0 (0b00010000)
#define TIM2_CCMR13_OC1M_1 (0b00100000)
#define TIM2_CCMR13_OC1M_2 (0b01000000)
#define TIM2_CCMR13_OC1M_12 (TIM2_CCMR13_OC1M_2 | TIM2_CCMR13_OC1M_1 | TIM2_CCMR13_OC1M_0)
#define TIM2_CCMR13_PWM1             (TIM2_CCMR13_OC1M_2 | TIM2_CCMR13_OC1M_1)
#define TIM2_CCMR13_OCPE             (0b00001000)
#define TIM2_CCMR13_OUTPUT           0x00
#define TIM2_CCMR13_CC1S_INPUT_TI1   0x01
#define TIM2_CCMR13_ICF_N1           ((0x01)<<4)
#define TIM2_CCMR13_ICF_N2           ((0x03)<<4)
#define TIM2_CCMR13_ICF_N8           ((0x09)<<4)

#define TIM2_COMPARE_1_REGISTER  (TIM2_BASE_ADDRESS + 0x34)
#define TIM2_COMPARE_2_REGISTER  (TIM2_BASE_ADDRESS + 0x38)
#define TIM2_COMPARE_3_REGISTER  (TIM2_BASE_ADDRESS + 0x3C)
#define TIM2_COMPARE_4_REGISTER  (TIM2_BASE_ADDRESS + 0x40)

#define TIM2_CAPTURE_COMPARE_ENABLE_REGISTER  (TIM2_BASE_ADDRESS + 0x20)
// flags for TIM2_ccer register for output
#define TIM2_CCER_CC3E  (0x100)
#define TIM2_CCER_CC1E  1

// flags for tim_ccr regi for input captue
// cc1np and cc1p bits select the active polarity of TI2FP1/TI2FP2 for trigger or capture operations
#define  TIM2_CM1_IN_POLARITY_BITS      0xF
#define  TIM2_CM1_IN_POLARITY_RISING    0x00
#define  TIM2_CH1_ENABLE                0x01
#define TIM_CCMR1_OC2M_MASK (0b111 << 12)
// #define TIM_CCMR1_OC2M_MASK (0b111 << 12)


// Value to set PWM Mode 1 (110) on Channel 2
#define TIM_CCMR1_OC2M_PWM1     ((uint32_t)(0b110 << 12))

#define TIM11_BASE_ADDRESS              ((uint32_t)0x40014800)

#define TIM11_CR1_REGISTER              (TIM11_BASE_ADDRESS + 0x00)
#define TIM11_CCMR1_REGISTER            (TIM11_BASE_ADDRESS + 0x18)
#define TIM11_CCER_REGISTER             (TIM11_BASE_ADDRESS + 0x20)
#define TIM11_CNT_REGISTER              (TIM11_BASE_ADDRESS + 0x24)
#define TIM11_PSC_REGISTER              (TIM11_BASE_ADDRESS + 0x28)
#define TIM11_ARR_REGISTER              (TIM11_BASE_ADDRESS + 0x2C)
#define TIM11_CCR1_REGISTER             (TIM11_BASE_ADDRESS + 0x34) // Channel 1
#define TIM11_AUTORELOAD_REGISTER       (TIM11_BASE_ADDRESS + 0X2C)
#define TIM11_COMPARE_2_REGISTER        (TIM11_BASE_ADDRESS + 0x38)

// --- Flags for Channel 1 (Different from Channel 2!) ---

// Mask to clear OC1M bits (Bits 6:4)
#define TIM_CCMR1_OC1M_MASK             ((uint32_t)(0b111 << 4))
// PWM Mode 1 for Channel 1 (110 shifted to bit 4)
#define TIM_CCMR1_OC1M_PWM1             ((uint32_t)(0b110 << 4))
// Preload Enable for Channel 1 (Bit 3)
#define TIM_CCMR1_OC1PE                 ((uint32_t)(1 << 3))
// Output Enable for Channel 1 (Bit 0 in CCER)
#define TIM_CCER_CC1E                   ((uint32_t)(1 << 0))

/*Function definitions---------------------------------------------------------*/


void initTimer2_CH2_PWM( void );
void initTimer11_CH1_PWM( void );



#ifdef __cplusplus
}
#endif

#endif /*__LED1_H */