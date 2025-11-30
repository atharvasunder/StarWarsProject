/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_GPIO_H_
#define __HARDWARE_STM_GPIO_H_

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

// RCC base address
#define RCC_BASE_ADDRESS ((uint32_t)0x40023800)

// Port A base address and registers
#define PORTA_BASE_ADDRESS      ((uint32_t)0x40020000)
#define PORTA_MODER             (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x00))
#define PORTA_OTYPER            (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x04))
#define PORTA_OSPEEDR           (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x08))
#define PORTA_PUPDR             (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x0C))
#define PORTA_IDR               (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x10))
#define PORTA_ODR               (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x14))
#define PORTA_BSRR              (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x18))
#define PORTA_LCKR              (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x1C))
#define PORTA_AFRL              (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x20))
#define PORTA_AFRH              (*(volatile uint32_t *)(PORTA_BASE_ADDRESS + 0x24))

// Port B base address and registers
#define PORTB_BASE_ADDRESS      ((uint32_t)0x40020400)
#define PORTB_MODER             (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x00))
#define PORTB_OTYPER            (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x04))
#define PORTB_OSPEEDR           (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x08))
#define PORTB_PUPDR             (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x0C))
#define PORTB_IDR               (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x10))
#define PORTB_ODR               (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x14))
#define PORTB_BSRR              (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x18))
#define PORTB_LCKR              (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x1C))
#define PORTB_AFRL              (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x20))
#define PORTB_AFRH              (*(volatile uint32_t *)(PORTB_BASE_ADDRESS + 0x24))

// Port C base address and registers
#define PORTC_BASE_ADDRESS      ((uint32_t)0x40020800)
#define PORTC_MODER             (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x00))
#define PORTC_OTYPER            (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x04))
#define PORTC_OSPEEDR           (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x08))
#define PORTC_PUPDR             (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x0C))
#define PORTC_IDR               (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x10))
#define PORTC_ODR               (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x14))
#define PORTC_BSRR              (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x18))
#define PORTC_LCKR              (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x1C))
#define PORTC_AFRL              (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x20))
#define PORTC_AFRH              (*(volatile uint32_t *)(PORTC_BASE_ADDRESS + 0x24))

// Port F base address and registers
#define PORTF_BASE_ADDRESS      ((uint32_t)0x40021400)
#define PORTF_MODER             (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x00))
#define PORTF_OTYPER            (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x04))
#define PORTF_OSPEEDR           (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x08))
#define PORTF_PUPDR             (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x0C))
#define PORTF_IDR               (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x10))
#define PORTF_ODR               (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x14))
#define PORTF_BSRR              (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x18))
#define PORTF_LCKR              (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x1C))
#define PORTF_AFRL              (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x20))
#define PORTF_AFRH              (*(volatile uint32_t *)(PORTF_BASE_ADDRESS + 0x24))

// Port H base address and registers
#define PORTH_BASE_ADDRESS    ((uint32_t)0x40021C00U)
#define PORTH_MODER           (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x00U))
#define PORTH_OTYPER          (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x04U))
#define PORTH_OSPEEDR         (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x08U))
#define PORTH_PUPDR           (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x0CU))
#define PORTH_IDR             (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x10U))
#define PORTH_ODR             (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x14U))
#define PORTH_BSRR            (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x18U))
#define PORTH_AFRL            (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x20U))
#define PORTH_AFRH            (*(volatile uint32_t *)(PORTH_BASE_ADDRESS + 0x24U))


/*Function definitions---------------------------------------------------------*/
void initGpioBxAsInput(uint16_t );
void initGpioBxAsOutput(uint16_t );
void toggleGPIOBx( uint16_t );
void setGPIOBx( uint16_t );
void clearGPIOBx( uint16_t );
void initGpioCxAsInput( uint16_t );
void initGpioCxAsInput_PU( uint16_t);
void initGpioCxAsInputNoPull(uint16_t);
uint32_t checkGPIOBx(uint16_t);
uint32_t checkGPIOCx(uint16_t);
void initGpioBxAsAF2(uint16_t);
void initGpioCxAsAF2(uint16_t);
void initGpioBxForPWM(uint16_t);
void initGpioFxAsOutput(uint16_t);
void setGPIOFx(uint16_t);
void clearGPIOFx(uint16_t);
uint32_t checkGPIOFx(uint16_t );
void initGpioFxAsAnalog(uint16_t);
void initGpioAxAsOutput(uint16_t);
void setGPIOAx(uint16_t);
void clearGPIOAx(uint16_t);
uint32_t checkGPIOAx(uint16_t );
void initGpioAxAsAnalog(uint16_t);
void initGpioAxAsAF2(uint16_t);
void initGpioBxAsAF4(uint8_t);
void initGpioAxAsAF4(uint8_t);
void initGpioCxAsAF4(uint8_t);
void initGpioFxAsAF4(uint8_t);
void initGpioHxAsAF4(uint8_t);
void initGpioBxAsAF1(uint16_t);
void initGpioFxAsAF3(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */
