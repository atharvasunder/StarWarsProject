/**
  ******************************************************************************
  * @file    hardware_stm_timer3.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Oct-2025
  * @brief   Controls STM32F446ze timer3
  ******************************************************************************
  */

#include "hardware_stm_timer3.h"
#include "hardware_stm_gpio.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>
/* MACRO definitions----------------------------------------------------------*/

// TIM3 Base Address
#define TIM3_BASE_ADDRESS   ((uint32_t)0x40000400)

// TIM3 Register Addresses (Base + Offset) - directly accessible as registers
#define TIM3_CR1                                (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x00))
// this one is TIM3_INTERRUPT_ENABLE_REGISTER 
#define TIM3_DIER                               (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x0C))
#define TIM3_SR                                 (*(volatile uint32_t *)(TIM3_BASE_ADDRESS + 0x10))
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


// TIM3_SR Bit Definitions (Status Register)
#define TIM3_SR_UIF         (1 << 0)    // Update Interrupt Flag
#define TIM3_SR_CC1IF       (1 << 1)    // Capture/Compare 1 Interrupt Flag
#define TIM3_SR_CC2IF       (1 << 2)    // Capture/Compare 2 Interrupt Flag
#define TIM3_SR_CC3IF       (1 << 3)    // Capture/Compare 3 Interrupt Flag
#define TIM3_SR_CC4IF       (1 << 4)    // Capture/Compare 4 Interrupt Flag

// TIM3_CCMR1 Bit Definitions (for Channel 1)
#define TIM3_CCMR1_CC1S_OUTPUT  (0x0 << 0)  // CC1 channel configured as output
#define TIM3_CCMR1_CC1S_TI1     (0x1 << 0)  // CC1 channel configured as input, mapped to TI1
#define TIM3_CCMR1_CC1S_TI2     (0x2 << 0)  // CC1 channel configured as input, mapped to TI2
#define TIM3_CCMR1_IC1PSC       (0x3 << 2)  // Input Capture 1 Prescaler
#define TIM3_CCMR1_IC1F         (0xF << 4)  // Input Capture 1 Filter
#define TIM3_CCMR1_OC1FE        (1 << 2)    // Output Compare 1 Fast enable
#define TIM3_CCMR1_OC1PE        (1 << 3)    // Output Compare 1 Preload enable
#define TIM3_CCMR1_OC1M_TOGGLE  (0x3 << 4)  // OC1M = 011 (Toggle mode)
#define TIM3_CCMR1_OC1M_PWM1    (0x6 << 4)  // OC1M = 110 (PWM mode 1)
#define TIM3_CCMR1_OC1M_PWM2    (0x7 << 4)  // OC1M = 111 (PWM mode 2)

// TIM3_CCMR2 Bit Definitions (for Channel 3)
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

void timer3_init(uint16_t prescale_value, uint16_t period){
    // Step 1: Enable the APB1 clock for TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // stop timer if running
    TIM3_CR1 = TIM3_CR1 & ~TIM3_CR1;

    // Step 2: Clear the update event flag in TIM3_SR
    TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;

    // Make sure this start from 0
    TIM3_CNT = TIM3_CNT & ~TIM3_CNT;
    
    // Step 3: Upload the prescale value to TIM3_PSC
    TIM3_PSC = prescale_value;
    
    // Step 4: Set the period value to TIM3_ARR
    TIM3_ARR = period;
    
    // Step 5: Enable the timer by setting CEN bit in TIM3_CR1
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
}

void timer3_channel3_output_compare(uint16_t prescale_value,uint16_t period, uint16_t compare_value){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
    TIM3_PSC = prescale_value;
    TIM3_ARR = period;
    
    // Configure channel 3 as output and set to Toggle mode
    TIM3_CCMR2 = TIM3_CCMR2 & ~(0xFF << 0);  // Clear channel 3 bits (bits 7:0)
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_CC3S_OUTPUT /* CC3 as output */
                            | TIM3_CCMR2_OC3M_TOGGLE;/* OC3M = 011 (toggle)*/

    // Step 6: Set the compare value in CCR3
    TIM3_CCR3 = compare_value;
    
    // Step 7: Enable channel 3 output by setting CC3E bit in CCER
    TIM3_CCER = TIM3_CCER | TIM3_CCER_CC3E;
    
    // Step 8: Enable the timer by setting CEN bit in TIM3_CR1
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;

}

void timer3_ch1_input_capture_init(uint16_t prescale_value, uint16_t period)
{
    // Step 1: Enable the APB1 clock for TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    // Step 2: Clear the update event flag in TIM3_SR
    TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
    // Step 3: Upload the prescale value to TIM3_PSC
    TIM3_PSC = prescale_value;
    // Step 4: Set the period value to TIM3_ARR
    TIM3_ARR = period;
    // Step 5: Set the input capture mode in CCMR1 register for channel 1
    // Configure CC1S bits to 01 (IC1 mapped to TI1)
    TIM3_CCMR1 = TIM3_CCMR1 & ~(0xFF << 0);  // Clear channel 1 bits (bits 7:0)
    TIM3_CCMR1 = TIM3_CCMR1 | TIM3_CCMR1_CC1S_TI1;  // Set CC1S = 01 (input, mapped to TI1)
    // Step 6: Enable channel 1 by setting CC1E bit in CCER
    TIM3_CCER = TIM3_CCER | TIM3_CCER_CC1E;
    // Step 7: Enable the timer by setting CEN bit in TIM3_CR1
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
}

uint16_t getCaptureTimer3CH1(void)
{
    // Check if capture event occurred (CC1IF flag is bit 1)
    if(TIM3_SR & TIM3_SR_CC1IF)
    {
        TIM3_SR = TIM3_SR & ~TIM3_SR_CC1IF;  // Clear CC1IF flag
        return TIM3_CCR1;
    }
    else
    {
        return 0;  // No new capture event
    }
}

void enableNVIC_Timer3(void)
{
    NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 = TIM3_INTERRUPT_BIT;
}

void initTimer3ToInterrupt(uint16_t prescalervalue, uint16_t autoreloadvalue)
{
    
    /* Timer 3 APB1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Enable the NVIC interrupt for Timer 3 */
    enableNVIC_Timer3();
    
    /* Clear any pending flags in the status register */
    TIM3_SR = 0;
    
    /* Set Prescaler value */
    TIM3_PSC = prescalervalue;
    
    /* Set Auto-reload value (defines the period) */
    TIM3_ARR = autoreloadvalue;
    
    /* Set Compare value for Channel 3 (25% of period) */
    TIM3_CCR3 = autoreloadvalue / 4;
    
    /* Enable Preload register for Channel 3 */
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_OC3PE;
    
    /* Configure Channel 3 as output (CC3S = 00) */
    TIM3_CCMR2 = TIM3_CCMR2 & ~TIM3_CCMR2_CC3S_MASK;  // Clear CC3S bits
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_CC3S_OUTPUT;
    
    /* Enable Channel 3 output */
    TIM3_CCER = TIM3_CCER | TIM3_CCER_CC3E;
    
    /* Enable interrupts for:
     * - Update event (counter overflow)
     * - Channel 3 Compare match
     */
    TIM3_DIER = TIM3_DIER | (TIM3_DIER_UIE | TIM3_DIER_CC3IE);
    
    /* Enable Timer 3 counter */
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
}

void TIM3_ClearCC3Flag(void)
{
    TIM3_SR = TIM3_SR & ~TIM3_SR_CC3IF;
}

void TIM3_ClearUpdateFlag(void)
{
    TIM3_SR = TIM3_SR & ~TIM3_SR_UIF;
}

uint8_t TIM3_GetUpdateFlag(void)
{
    return (TIM3_SR & TIM3_SR_UIF) && (TIM3_DIER & TIM3_DIER_UIE);
}

uint8_t TIM3_GetCC3Flag(void){
    return (TIM3_SR & TIM3_SR_CC3IF) && (TIM3_DIER & TIM3_DIER_CC3IE);
}

void initTimer3PWM(uint16_t prescalervalue, uint16_t autoreloadvalue, float duty_cycle)
{
    /* Enable Timer 3 APB1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Clear status register */
    TIM3_SR = 0;
    
    /* Set Prescaler: 90 MHz / 9000 = 10 kHz */
    TIM3_PSC = prescalervalue;
    
    /* Set Auto-Reload: 10 kHz / 40000 = 0.25 Hz (4 second period) */
    TIM3_ARR = autoreloadvalue;

     /* Clamp duty cycle */
    if (duty_cycle < 0.0f) { duty_cycle = 0.0f; }
    if (duty_cycle > 1.0f) { duty_cycle = 1.0f; }
    
    /* Set Compare value for  duty cycle */
    TIM3_CCR3 = (uint16_t)((float)autoreloadvalue * duty_cycle);  // 
    
    /* Configure Channel 3 as Output */
    TIM3_CCMR2 = TIM3_CCMR2 & ~TIM3_CCMR2_CC3S_MASK;        // Clear CC3S bits
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_CC3S_OUTPUT;       // CC3 as output
    
    /* Enable Output Compare 3 Preload */
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_OC3PE;
    
    /* Set PWM Mode 1 on Channel 3 */
    TIM3_CCMR2 = TIM3_CCMR2 & ~TIM3_CCMR2_OC3M_MASK;        // Clear OC3M bits
    TIM3_CCMR2 = TIM3_CCMR2 | TIM3_CCMR2_OC3M_PWM1;         // Set PWM mode 1
    
    /* Enable Channel 3 output with default polarity (active high) */
    TIM3_CCER = TIM3_CCER | TIM3_CCER_CC3E;
    
    /* Enable Auto-Reload Preload (optional but recommended) */
    TIM3_CR1 = TIM3_CR1 | (1 << 7);  // ARPE bit
    
    /* Start the timer */
    TIM3_CR1 = TIM3_CR1 | TIM3_CR1_CEN;
}

void Timer3_ComputePrescalerAndARR(uint32_t timer_clock_hz,
                                   uint32_t delay_ms,
                                   uint16_t *prescaler_out,
                                   uint16_t *autoreload_out)
{
    /*
     * Formula:
     *   period_seconds = (PSC + 1) * (ARR + 1) / timer_clock
     *   delay_ms / 1000 = (PSC + 1) * (ARR + 1) / timer_clock
     *   → (PSC + 1) * (ARR + 1) = timer_clock * delay_ms / 1000
     */

    uint32_t target_counts = (timer_clock_hz / 1000U) * delay_ms;
    uint32_t prescaler = 0U;
    uint32_t autoreload = 0U;

    /* we constrain ARR to 16-bit range (max 65535) for simplicity */
    if (target_counts <= 65535U)
    {
        prescaler = 0U;
        autoreload = target_counts - 1U;
    }
    else
    {
        /* find prescaler so ARR fits into 16 bits */
        prescaler = (target_counts / 65535U);
        if (prescaler > 0U)
        {
            prescaler = prescaler - 1U;
        }

        autoreload = (target_counts / (prescaler + 1U)) - 1U;
    }

    *prescaler_out = (uint16_t)prescaler;
    *autoreload_out = (uint16_t)autoreload;
}

void timer3_clearSR(void)
{
    TIM3_SR  = TIM3_SR & ~TIM3_SR;
}
void timer3_stopC1(void)
{
    TIM3_CR1 = TIM3_CR1 & ~TIM3_CR1;
}
uint8_t timer3_checkUIF(void)
{
    return TIM3_SR & TIM3_SR_UIF;
}

void timer3_pwm_dual_init_B01(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // PB0 -> TIM3_CH3, PB1 -> TIM3_CH4
    initGpioBxAsAF2(0);
    initGpioBxAsAF2(1);

    // 30 kHz: ARR = 2799, PSC = 0  (84 MHz / (0+1) / (2799+1) = 30 kHz)
    TIM3_PSC = 0;
    TIM3_ARR = 2799;

    // Channel 3: PWM1 + preload
    TIM3_CCMR2 = (TIM3_CCMR2 & ~(TIM3_CCMR2_OC3M_MASK | TIM3_CCMR2_CC3S_MASK))
               | TIM3_CCMR2_OC3M_PWM1 | TIM3_CCMR2_OC3PE;

    // Channel 4: PWM1 + preload (bits [14:12] are OC4M, [8] is OC4PE)
    // Reuse CCMR2: CH4 shares CCMR2 high bits
    TIM3_CCMR2 |= (0x6U << 12) | (1U << 11);  // OC4M=110 (PWM1), OC4PE=1

    // Enable CH3 and CH4 outputs (active high)
    TIM3_CCER |= TIM3_CCER_CC3E | TIM3_CCER_CC4E;

    // start with 0% duty
    TIM3_CCR3 = 0;
    TIM3_CCR4 = 0;

    // ARPE (auto-reload preload) so period updates are buffered (optional)
    TIM3_CR1 |= (1U << 7);

    // Counter enable
    TIM3_CR1 |= TIM3_CR1_CEN;
}