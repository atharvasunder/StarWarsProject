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
void initTimer3PWM(uint16_t,uint16_t,float);
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