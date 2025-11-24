/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Septembr-2025
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"


/* MACRO definitions----------------------------------------------------------*/
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


/* function definitions----------------------------------------------------------*/
void setGPIOBx(uint16_t x){
    PORTB_BSRR = (1UL << x);
}

void clearGPIOBx(uint16_t x){
    PORTB_BSRR = (1UL << (x + 16));
}

uint32_t checkGPIOBx(uint16_t x)
{
    return (PORTB_IDR >> x) & 0x01;
}

uint32_t checkGPIOCx(uint16_t x){
    /* Read the input data register for GPIOC and check bit 6 */
    return (PORTC_IDR >> x) & 0x01;
}

void initGpioBxAsOutput(uint16_t x){
    /* Enable clock for GPIOB */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Configure PB0 as output */
    PORTB_MODER = PORTB_MODER & ~(3UL << (x * 2)); // Clear bits
    PORTB_MODER = PORTB_MODER |  (1UL << (x * 2)); // 01 = General purpose output

    /* Output type push-pull */
    PORTB_OTYPER = PORTB_OTYPER & ~(1UL << x);

    /* No pull-up/pull-down */
    PORTB_PUPDR = PORTB_PUPDR & ~(3UL << (x * 2));

    /* Set PB0 high initially */
    setGPIOBx(x);  // Set PBx
}

void initGpioBxAsInput(uint16_t x)
{
    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOC Pin x as input */
    PORTB_MODER = PORTB_MODER & ~(3UL << (x * 2)); // Clear bits 2x+1:2x to set as input (00)

    /* PUSH-PULL Pin */
    PORTB_OTYPER = PORTB_OTYPER & ~(1UL << x);

    /* GPIOC pin x high speed */
    PORTB_OSPEEDR = PORTB_OSPEEDR | (3UL << (x * 2)); // Set bits 2x+1:2x to 11 for high speed

    /* Configure pulled-down for active-HIGH button/sensor */
    PORTB_PUPDR = PORTB_PUPDR & ~(3UL << (x * 2)); // Clear bits first
    PORTB_PUPDR = PORTB_PUPDR |  (2UL << (x * 2)); // Set to 10 for pull-down
}

void initGpioCxAsInput(uint16_t x){
    /* GPIOC Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    /* GPIOC Pin x as input */
    PORTC_MODER = PORTC_MODER & ~(3UL << (x * 2)); // Clear bits 2x+1:2x to set as input (00)
    
    /* PUSH-PULL Pin */
    PORTC_OTYPER = PORTC_OTYPER & ~(1UL << x);
    
    /* GPIOC pin x high speed */
    PORTC_OSPEEDR = PORTC_OSPEEDR | (3UL << (x * 2)); // Set bits 2x+1:2x to 11 for high speed
    
    /* Configure pulled-down */
    PORTC_PUPDR = PORTC_PUPDR & ~(3UL << (x * 2)); // Clear bits first
    PORTC_PUPDR = PORTC_PUPDR | (2UL << (x * 2));  // Set to 10 for pull-down
}

void initGpioCxAsInput_PU(uint16_t x){
    /* GPIOC Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    /* GPIOC Pin x as input */
    PORTC_MODER = PORTC_MODER & ~(3UL << (x * 2)); // Clear bits 2x+1:2x to set as input (00)
    
    /* PUSH-PULL Pin */
    PORTC_OTYPER = PORTC_OTYPER & ~(1UL << x);
    
    /* GPIOC pin x high speed */
    PORTC_OSPEEDR = PORTC_OSPEEDR | (3UL << (x * 2)); // Set bits 2x+1:2x to 11 for high speed
    
    /* Configure pulled-down */
    PORTC_PUPDR = PORTC_PUPDR & ~(3UL << (x * 2)); // Clear bits first
    PORTC_PUPDR = PORTC_PUPDR | (1UL << (x * 2));  // Set to 01 for pull-down
}

void initGpioCxAsInputNoPull(uint16_t x)
{
    /* GPIOC Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* GPIOC Pin x as input (MODER:00) */
    PORTC_MODER = PORTC_MODER & ~(3UL << (2U * x));

    /* PUSH-PULL Pin */
    PORTC_OTYPER = PORTC_OTYPER & ~(1UL << x);

    /* GPIOC pin x high speed */
    PORTC_OSPEEDR = PORTC_OSPEEDR | (3UL << (x * 2)); // Set bits 2x+1:2x to 11 for high speed

    /* PUPDR: 00 (no pull) */
    PORTC_PUPDR = PORTC_PUPDR & ~(3UL << (2U * x));
}

void initGpioBxAsAF2(uint16_t x)
{
    // Step 1: Enable the AHB1 clock for GPIOB
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // Step 2: Set MODERx to Alternate Function mode (10)
    PORTB_MODER = PORTB_MODER & ~(3UL << (x * 2));  // Clear bits 1:0
    PORTB_MODER = PORTB_MODER | (2UL << (x * 2));   // Set to 10 (Alternate Function)
    
    // Step 3: Set Port B pin x to push-pull
    PORTB_OTYPER = PORTB_OTYPER & ~(1UL << x);  // 0 = Push-pull
    
    // Step 4: Set Port B pin x to High Speed
    PORTB_OSPEEDR = PORTB_OSPEEDR & ~(3UL << (x * 2));  // Clear bits
    PORTB_OSPEEDR = PORTB_OSPEEDR | (2UL << (x * 2));   // 10 = High speed
    
    // Step 5: No pull-up or pull-down
    PORTB_PUPDR = PORTB_PUPDR & ~(3UL << (x * 2));  // 00 = No pull-up, no pull-down
    
    // Step 6: Set Port B pin x to Alternative Function 2 (TIM3_CH3)
    if (x < 8U)
    {
        PORTB_AFRL = PORTB_AFRL & ~(0xFUL << (x * 4));  // Clear AFRL bits (bits 4x+3:4x)
        PORTB_AFRL = PORTB_AFRL | (2UL << (x * 4));   // Set to 10 (AF2)
    }
    else
    {
        uint32_t y = (uint32_t)x - 8U;
        PORTB_AFRH = PORTB_AFRH & ~(0xFUL << (y * 4)); // Clear AFRH bits 
        PORTB_AFRH = PORTB_AFRH | (2UL << (y * 4));   // Set to 10 (AF2)
    }
}

void initGpioCxAsAF2(uint16_t x)
{   
    // Step 1: Enable the AHB1 clock for GPIOC
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    // Step 2: Set MODERx to Alternate Function mode (10)
    PORTC_MODER = PORTC_MODER & ~(3UL << (x * 2));  // Clear bits 13:12
    PORTC_MODER = PORTC_MODER | (2UL << (x * 2));   // Set to 10 (Alternate Function)
    
    // Step 3: Set Port C pin x to push-pull
    PORTC_OTYPER = PORTC_OTYPER & ~(1UL << x);  // 0 = Push-pull
    
    // Step 4: Set Port C pin x to High Speed
    PORTC_OSPEEDR = PORTC_OSPEEDR & ~(3UL << (x * 2));  // Clear bits
    PORTC_OSPEEDR = PORTC_OSPEEDR | (2UL << (x * 2));   // 10 = High speed
    
    // Step 5: No pull-up or pull-down
    PORTC_PUPDR = PORTC_PUPDR & ~(3UL << (x * 2));  // 00 = No pull-up, no pull-down
    
    // Step 6: Set Port C pin x to Alternative Function 2 (TIM3_CH1)
    if (x < 8U)
    {
        PORTC_AFRL = PORTC_AFRL & ~(0xF << (x * 4));   // Clear AFRLx bits (bits 4x+3:4x)
        PORTC_AFRL = PORTC_AFRL | (0x2 << (x * 4));           // Set to (AF2)
    }
    else
    {
        uint32_t y = (uint32_t)x - 8U;
        PORTC_AFRH = PORTC_AFRH & ~(0xFUL << (y * 4)); // Clear AFRH bits 
        PORTC_AFRH = PORTC_AFRH | (2UL << (y * 4));   // Set to 10 (AF2)
    }
}


void toggleGPIOBx(uint16_t x){
    /* Get the current value of the pin */
    uint32_t value = PORTB_ODR & (1UL << x);
    
    if (value > 0){
        /* If high, clear the bit */
        PORTB_BSRR = (1UL << (x + 16));
    }
    else{
        /* If low, set the bit */
        PORTB_BSRR = (1UL << x);
    } 
}

void initGpioBxForPWM(uint16_t x)
{
    uint32_t * reg_pointer;
    
    /* GPIOB Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    /* Set PB0 to Alternate Function mode (10) */
    PORTB_MODER = PORTB_MODER & ~(0x3 << (x * 2));        // Clear bits
    PORTB_MODER = PORTB_MODER | (0x2 << (x * 2));         // Set to AF mode
    
    /* Set PB0 to Push-Pull output type */
    PORTB_OTYPER = PORTB_OTYPER & ~(0x1 << (x * 2));        // 0 = Push-pull
    
    /* Set PB0 to High Speed */
    PORTB_OSPEEDR = PORTB_OSPEEDR | (0x3 << (x * 2));         // 11 = High speed
    
    /* Set PB0 to No Pull-up/Pull-down */
    PORTB_PUPDR = PORTB_PUPDR & ~(0x3 << (x * 2));        // 00 = No pull
    
    /* Set PB0 to Alternate Function 2 (TIM3_CH3) in AFRL */
    PORTB_AFRL = PORTB_AFRL & ~(0xF << (x * 4));        // Clear AF bits for pin 0
    PORTB_AFRL = PORTB_AFRL | (0x2 << (x * 4));         // AF2 = TIM3
}

void initGpioFxAsOutput(uint16_t x)
{
    /* Enable clock for GPIOF */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    /* Configure PFx as general-purpose output (MODER: 01) */
    PORTF_MODER &= ~(3UL << (x * 2));   // Clear bits
    PORTF_MODER |=  (1UL << (x * 2));   // Set to output mode

    /* Output type: Push-pull (OTYPER: 0) */
    PORTF_OTYPER &= ~(1UL << x);

    /* Output speed: High speed (OSPEEDR: 11) */
    PORTF_OSPEEDR &= ~(3UL << (x * 2));
    PORTF_OSPEEDR |=  (3UL << (x * 2));

    /* No pull-up, no pull-down (PUPDR: 00) */
    PORTF_PUPDR &= ~(3UL << (x * 2));

    /* Initialize PFx low (optional) */
    PORTF_BSRR = (1UL << (x + 16)); // Reset bit
}

void setGPIOFx(uint16_t x)   { PORTF_BSRR = (1UL << x); }
void clearGPIOFx(uint16_t x) { PORTF_BSRR = (1UL << (x + 16)); }
uint32_t checkGPIOFx(uint16_t x) { return (PORTF_IDR >> x) & 0x01; }

void initGpioFxAsAnalog(uint16_t x)
{
    // Enable GPIOF clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    // Set pin x to analog mode (MODER = 11)
    PORTF_MODER &= ~(3UL << (x * 2));
    PORTF_MODER |=  (3UL << (x * 2));

    // No pull-up/pull-down
    PORTF_PUPDR &= ~(3UL << (x * 2));
}

void initGpioAxAsAnalog(uint16_t x)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Analog mode (MODER = 11)
    PORTA_MODER &= ~(3UL << (x * 2));
    PORTA_MODER |=  (3UL << (x * 2));

    // No pull-up/down
    PORTA_PUPDR &= ~(3UL << (x * 2));
}

void initGpioAxAsOutput(uint16_t x)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // MODER = 01 (General-purpose output)
    PORTA_MODER &= ~(3UL << (x * 2));
    PORTA_MODER |=  (1UL << (x * 2));

    // OTYPER = 0 (Push-pull)
    PORTA_OTYPER &= ~(1UL << x);

    // OSPEEDR = 10 (High speed)
    PORTA_OSPEEDR &= ~(3UL << (x * 2));
    PORTA_OSPEEDR |=  (2UL << (x * 2));

    // PUPDR = 00 (No pull)
    PORTA_PUPDR &= ~(3UL << (x * 2));
}

void setGPIOAx(uint16_t x) {
    PORTA_BSRR = (1UL << x);
}

void clearGPIOAx(uint16_t x) {
    PORTA_BSRR = (1UL << (x + 16));
}

uint32_t checkGPIOAx(uint16_t x) {
    return (PORTA_IDR >> x) & 0x01;
}

void initGpioAxAsAF2(uint16_t x)
{
    // Step 1: Enable clock for GPIOA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Step 2: Set pin mode to Alternate Function (MODER = 10)
    PORTA_MODER &= ~(3UL << (x * 2));
    PORTA_MODER |=  (2UL << (x * 2));

    // Step 3: Set push-pull output type (OTYPER = 0)
    PORTA_OTYPER &= ~(1UL << x);

    // Step 4: High speed (OSPEEDR = 10)
    PORTA_OSPEEDR &= ~(3UL << (x * 2));
    PORTA_OSPEEDR |=  (2UL << (x * 2));

    // Step 5: No pull-up / no pull-down (PUPDR = 00)
    PORTA_PUPDR &= ~(3UL << (x * 2));

    // Step 6: Set Alternate Function 2 (TIM3/TIM1/TIM9 depending on mapping)
    if (x < 8U)
    {
        PORTA_AFRL &= ~(0xFUL << (x * 4));
        PORTA_AFRL |=  (0x2UL << (x * 4));   // AF2
    }
    else
    {
        uint32_t y = x - 8U;
        PORTA_AFRH &= ~(0xFUL << (y * 4));
        PORTA_AFRH |=  (0x2UL << (y * 4));   // AF2
    }
}

void initGpioBxAsAF4(uint8_t pin)
{
    /* Enable GPIOB clock if not already */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* ---------- MODE: Alternate Function (10) ---------- */
    PORTB_MODER &= ~(3UL << (pin * 2U));
    PORTB_MODER |=  (2UL << (pin * 2U));

    /* ---------- OTYPER: Open-drain (1) ---------- */
    PORTB_OTYPER |= (1UL << pin);

    /* ---------- OSPEEDR: High speed (10) ---------- */
    PORTB_OSPEEDR &= ~(3UL << (pin * 2U));
    PORTB_OSPEEDR |=  (2UL << (pin * 2U));

    /* ---------- PUPDR: Pull-up (01) ---------- */
    PORTB_PUPDR &= ~(3UL << (pin * 2U));
    PORTB_PUPDR |=  (1UL << (pin * 2U));

    /* ---------- AFRH for pins 8–15 ---------- */
    if (pin >= 8)
    {
        uint32_t shift = (pin - 8) * 4U;
        PORTB_AFRH &= ~(0xFUL << shift);
        PORTB_AFRH |=  (4UL  << shift);   // AF4
    }
    else
    {
        uint32_t shift = pin * 4U;
        PORTB_AFRL &= ~(0xFUL << shift);
        PORTB_AFRL |=  (4UL  << shift);
    }
}

void initGpioAxAsAF4(uint8_t pin)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    PORTA_MODER &= ~(3UL << (pin * 2U));
    PORTA_MODER |=  (2UL << (pin * 2U));

    PORTA_OTYPER |=  (1UL << pin);
    PORTA_OSPEEDR &= ~(3UL << (pin * 2U));
    PORTA_OSPEEDR |=  (2UL << (pin * 2U));

    PORTA_PUPDR &= ~(3UL << (pin * 2U));
    PORTA_PUPDR |=  (1UL << (pin * 2U));

    if (pin < 8) {
        uint32_t shift = pin * 4U;
        PORTA_AFRL &= ~(0xFUL << shift);
        PORTA_AFRL |=  (4UL << shift);
    } else {
        uint32_t shift = (pin - 8) * 4U;
        PORTA_AFRH &= ~(0xFUL << shift);
        PORTA_AFRH |=  (4UL << shift);
    }
}

void initGpioCxAsAF4(uint8_t pin)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    PORTC_MODER &= ~(3UL << (pin * 2U));
    PORTC_MODER |=  (2UL << (pin * 2U));

    PORTC_OTYPER |=  (1UL << pin);
    PORTC_OSPEEDR &= ~(3UL << (pin * 2U));
    PORTC_OSPEEDR |=  (2UL << (pin * 2U));

    PORTC_PUPDR &= ~(3UL << (pin * 2U));
    PORTC_PUPDR |=  (1UL << (pin * 2U));

    if (pin < 8) {
        uint32_t shift = pin * 4U;
        PORTC_AFRL &= ~(0xFUL << shift);
        PORTC_AFRL |=  (4UL << shift);
    } else {
        uint32_t shift = (pin - 8) * 4U;
        PORTC_AFRH &= ~(0xFUL << shift);
        PORTC_AFRH |=  (4UL << shift);
    }
}

void initGpioFxAsAF4(uint8_t pin)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    PORTF_MODER &= ~(3UL << (pin * 2U));
    PORTF_MODER |=  (2UL << (pin * 2U));

    PORTF_OTYPER |=  (1UL << pin);
    PORTF_OSPEEDR &= ~(3UL << (pin * 2U));
    PORTF_OSPEEDR |=  (2UL << (pin * 2U));

    PORTF_PUPDR &= ~(3UL << (pin * 2U));
    PORTF_PUPDR |=  (1UL << (pin * 2U));

    if (pin < 8) {
        uint32_t shift = pin * 4U;
        PORTF_AFRL &= ~(0xFUL << shift);
        PORTF_AFRL |=  (4UL << shift);
    } else {
        uint32_t shift = (pin - 8) * 4U;
        PORTF_AFRH &= ~(0xFUL << shift);
        PORTF_AFRH |=  (4UL << shift);
    }
}

void initGpioHxAsAF4(uint8_t pin)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

    PORTH_MODER &= ~(3UL << (pin * 2U));
    PORTH_MODER |=  (2UL << (pin * 2U));

    PORTH_OTYPER |=  (1UL << pin);
    PORTH_OSPEEDR &= ~(3UL << (pin * 2U));
    PORTH_OSPEEDR |=  (2UL << (pin * 2U));

    PORTH_PUPDR &= ~(3UL << (pin * 2U));
    PORTH_PUPDR |=  (1UL << (pin * 2U));

    if (pin < 8) {
        uint32_t shift = pin * 4U;
        PORTH_AFRL &= ~(0xFUL << shift);
        PORTH_AFRL |=  (4UL << shift);
    } else {
        uint32_t shift = (pin - 8) * 4U;
        PORTH_AFRH &= ~(0xFUL << shift);
        PORTH_AFRH |=  (4UL << shift);
    }
}