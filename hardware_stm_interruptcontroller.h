/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_INTERRUPTCONTROLLER_H
#define __HARDWARE_STM_INTERRUPTCONTROLLER_H
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


// register addresses
#define SYSTEM_CONTROL_BASE_ADDRESS                     (0xE000E000)
#define NVIC_BASE_ADDRESS                               (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

/* NVIC Interrupt Set-Enable Registers */
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31         (*(volatile uint32_t *)(NVIC_BASE_ADDRESS))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63        (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x4))
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95        (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x8))

/* NVIC Interrupt Clear-Enable Registers */
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31       (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x80))
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_32_63      (*(volatile uint32_t *)(NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31 + 0x4))
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_64_95      (*(volatile uint32_t *)(NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31 + 0x8))

/* NVIC Interrupt Set-Pending Registers */
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31        (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x100))
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_32_63       (*(volatile uint32_t *)(NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31 + 0x4))
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_64_95       (*(volatile uint32_t *)(NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31 + 0x8))

/* NVIC Interrupt Clear-Pending Registers */
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31      (*(volatile uint32_t *)(NVIC_BASE_ADDRESS + 0x180))
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_32_63     (*(volatile uint32_t *)(NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 + 0x4))
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_64_95     (*(volatile uint32_t *)(NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 + 0x8))

/* Interrupt Bit Definitions */
#define TIM3_INTERRUPT_BIT                              (0x20000000)
#define EXTI9_5_INTERRUPT_BIT                           (0x800000)

/* SYSCFG Base Address and Registers */
#define SYSCFG_BASE_ADDRESS                             ((uint32_t)(0x40013800))
#define SYSCFG_EXTERNAL_INTERRUPT_REGISTER_2            (*(volatile uint32_t *)(SYSCFG_BASE_ADDRESS + 0x0C))

/* SYSCFG EXTI Configuration Flags */
#define SYSCFG_EXTERNAL_INTERRUPT_6_BITS                ((uint32_t)0xF00)
#define SYSCFG_EXTERNAL_INTERRUPT_6_PORTC               ((uint32_t)0x200)

/* External Interrupt Controller Base Address */
#define EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS      ((uint32_t)(0x40013C00))

/* EXTI Registers */
#define EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER     (*(volatile uint32_t *)(EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS))
#define EXTERNAL_INTERRUPT_CONTROLLER_RTSR              (*(volatile uint32_t *)(EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS + 0x08))
#define EXTERNAL_INTERRUPT_CONTROLLER_FTSR              (*(volatile uint32_t *)(EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS + 0x0C))
#define EXTERNAL_INTERRUPT_CONTROLLER_PENDING_REGISTER  (*(volatile uint32_t *)(EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS + 0x14))

/* EXTI6 Specific Flags */
#define EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER_EXTI6   ((uint32_t)0x40)
#define EXTERNAL_INTERRUPT_CONTROLLER_RTSR_EXTI6            ((uint32_t)0x40)
#define EXTERNAL_INTERRUPT_CONTROLLER_FTSR_EXTI6            ((uint32_t)0x40)
#define EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6         ((uint32_t)0x40)

/*Function definitions---------------------------------------------------------*/
void enableEXTI6OnPortC(void);
void EXTI9_5_IRQHandler(void);
void TIM3_IRQHandler(void);

extern volatile int16_t    buttonPressFlag;
extern volatile int32_t    pressedTime;

#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTCONTROLLER_H */
