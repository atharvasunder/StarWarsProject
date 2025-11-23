#ifndef HARDWARE_STM_TIMER4_H
#define HARDWARE_STM_TIMER4_H


#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

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
