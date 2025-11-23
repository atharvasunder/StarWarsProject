/**
  ******************************************************************************
  * @file    hardware_stm_adc.c 
  * @author  paussava@andrew.cmu.edu
  * @version 1.0
  * @date    Nov-2025
  * @brief   Controls STM32F446ze ADC
  ******************************************************************************
  */

#include "hardware_stm_adc.h"
#include "hardware_stm_gpio.h"
#include "stm32f4xx_mort2.h"
#include "stm32f4xx_rcc_mort.h"

// Base addresses
#define ADC1_BASE_ADDRESS     ((uint32_t)0x40012000)
#define ADC2_BASE_ADDRESS     ((uint32_t)0x40012100)
#define ADC3_BASE_ADDRESS     ((uint32_t)0x40012200)
#define ADC_COMMON_BASE       ((uint32_t)0x40012300)

// Key registers for ADC1
#define ADC1_SR        (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x00))
#define ADC1_CR1       (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x04))
#define ADC1_CR2       (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x08))
#define ADC1_SMPR1     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x0C))
#define ADC1_SMPR2     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x10))
#define ADC1_JOFR1     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x14))
#define ADC1_JOFR2     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x18))
#define ADC1_JOFR3     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x1C))
#define ADC1_JOFR4     (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x20))
#define ADC1_HTR       (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x24))
#define ADC1_LTR       (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x28))
#define ADC1_SQR1      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x2C))
#define ADC1_SQR2      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x30))
#define ADC1_SQR3      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x34))
#define ADC1_JSQR      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x38))
#define ADC1_JDR1      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x3C))
#define ADC1_JDR2      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x40))
#define ADC1_JDR3      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x44))
#define ADC1_JDR4      (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x48))
#define ADC1_DR        (*(volatile uint32_t *)(ADC1_BASE_ADDRESS + 0x4C))

#define ADC2_SR        (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x00))
#define ADC2_CR1       (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x04))
#define ADC2_CR2       (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x08))
#define ADC2_SMPR1     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x0C))
#define ADC2_SMPR2     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x10))
#define ADC2_JOFR1     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x14))
#define ADC2_JOFR2     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x18))
#define ADC2_JOFR3     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x1C))
#define ADC2_JOFR4     (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x20))
#define ADC2_HTR       (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x24))
#define ADC2_LTR       (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x28))
#define ADC2_SQR1      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x2C))
#define ADC2_SQR2      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x30))
#define ADC2_SQR3      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x34))
#define ADC2_JSQR      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x38))
#define ADC2_JDR1      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x3C))
#define ADC2_JDR2      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x40))
#define ADC2_JDR3      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x44))
#define ADC2_JDR4      (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x48))
#define ADC2_DR        (*(volatile uint32_t *)(ADC2_BASE_ADDRESS + 0x4C))

#define ADC3_SR        (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x00))
#define ADC3_CR1       (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x04))
#define ADC3_CR2       (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x08))
#define ADC3_SMPR1     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x0C))
#define ADC3_SMPR2     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x10))
#define ADC3_JOFR1     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x14))
#define ADC3_JOFR2     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x18))
#define ADC3_JOFR3     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x1C))
#define ADC3_JOFR4     (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x20))
#define ADC3_HTR       (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x24))
#define ADC3_LTR       (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x28))
#define ADC3_SQR1      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x2C))
#define ADC3_SQR2      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x30))
#define ADC3_SQR3      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x34))
#define ADC3_JSQR      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x38))
#define ADC3_JDR1      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x3C))
#define ADC3_JDR2      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x40))
#define ADC3_JDR3      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x44))
#define ADC3_JDR4      (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x48))
#define ADC3_DR        (*(volatile uint32_t *)(ADC3_BASE_ADDRESS + 0x4C))

// Common ADC registers
#define ADC_CCR     (*(volatile uint32_t *)(ADC_COMMON_BASE + 0x04))

typedef struct {
    uint8_t adc_num;     // 1, 2, or 3
    uint8_t channel;     // INx number
    char port;           // 'A', 'B', 'C', 'F'
    uint8_t pin;         // 0–15
} ADC_PinMap;

static const ADC_PinMap adc_map[] = {
    {1, 0, 'A', 0}, {1, 1, 'A', 1}, {1, 2, 'A', 2}, {1, 3, 'A', 3},
    {1, 4, 'A', 4}, {1, 5, 'A', 5}, {1, 6, 'A', 6}, {1, 7, 'A', 7},
    {3, 4, 'F', 6}, {3, 5, 'F', 7}, {3, 6, 'F', 8}, {3, 7, 'F', 9},
    {3, 8, 'F',10}, {3, 9, 'F', 3}, {3,14, 'F', 4}, {3,15, 'F', 5}
};

void initADC1(void)
{
    // Enable ADC1 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // ADC prescaler (PCLK2/4)
    ADC_CCR &= ~(3UL << 16);
    ADC_CCR |=  (1UL << 16);  // 01 = divide by 4

    // Right alignment
    ADC1_CR2 &= ~(1UL << 11);

    // Single conversion mode
    ADC1_CR2 &= ~(1UL << 1);  // CONT = 0 (single)

    // 12-bit resolution
    ADC1_CR1 &= ~(3UL << 24);

    // Enable ADC
    ADC1_CR2 |= (1UL << 0);   // ADON = 1
}

void initADC3(void)
{
    // Enable ADC3 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    // Prescaler PCLK2/4 for ADC common
    ADC_CCR &= ~(3UL << 16);
    ADC_CCR |=  (1UL << 16);

    // Right alignment, single conversion
    ADC3_CR2 &= ~(1UL << 11);  // ALIGN = 0 (right)
    ADC3_CR2 &= ~(1UL << 1);   // CONT = 0 (single mode)

    // 12-bit resolution
    ADC3_CR1 &= ~(3UL << 24);

    // Enable ADC3
    ADC3_CR2 |= (1UL << 0);    // ADON = 1
}

void initADC3_DMA(uint16_t *buffer, uint8_t length)
{
    // Enable scan mode (for multiple channels)
    ADC3_CR1 |= (1UL << 8);   // SCAN = 1

    // Enable DMA and continuous mode
    ADC3_CR2 |= (1UL << 8);   // DMA = 1
    ADC3_CR2 |= (1UL << 9);   // DDS = 1 (DMA requests continuously)
    ADC3_CR2 |= (1UL << 1);   // CONT = 1 (continuous conversion)

    // Set number of conversions (length)
    ADC3_SQR1 &= ~(0xF << 20);
    ADC3_SQR1 |= ((length - 1) << 20);

    // Example: PF7, PF8, PF9 → channels 5, 6, 7
    ADC3_SQR3 = (5U) | (6U << 5) | (7U << 10);

    // Configure sampling time (239.5 cycles)
    ADC3_SMPR2 |= (7U << (5*3)) | (7U << (6*3)) | (7U << (7*3));

    // Enable ADC3
    ADC3_CR2 |= (1UL << 0);
}

uint16_t readADC1Channel(uint8_t channel) //polling
{
    // Configure channel sequence: 1 conversion in SQR1/SQR3
    ADC1_SQR1 &= ~(0xF << 20);              // 1 conversion
    ADC1_SQR3 &= ~(0x1F << 0);              // clear SQ1 bits
    ADC1_SQR3 |= (channel & 0x1F);          // set channel number

    // Sampling time (239.5 cycles)
    if (channel < 10)
        ADC1_SMPR2 |= (7UL << (channel * 3));
    else
        ADC1_SMPR1 |= (7UL << ((channel - 10) * 3));

    // Start conversion
    ADC1_CR2 |= (1UL << 30);                // SWSTART = 1

    // Wait until conversion complete
    while (!(ADC1_SR & (1UL << 1)));        // EOC = 1

    // Read data
    uint16_t value = (uint16_t)(ADC1_DR & 0xFFFF);

    return value;
}

uint16_t readADC2Channel(uint8_t channel) //polling
{
    // Configure channel sequence: 1 conversion in SQR1/SQR3
    ADC2_SQR1 &= ~(0xF << 20);              // 1 conversion
    ADC2_SQR3 &= ~(0x1F << 0);              // clear SQ1 bits
    ADC2_SQR3 |= (channel & 0x1F);          // set channel number

    // Sampling time (239.5 cycles)
    if (channel < 10)
        ADC2_SMPR2 |= (7UL << (channel * 3));
    else
        ADC2_SMPR1 |= (7UL << ((channel - 10) * 3));

    // Start conversion
    ADC2_CR2 |= (1UL << 30);                // SWSTART = 1

    // Wait until conversion complete
    while (!(ADC2_SR & (1UL << 1)));        // EOC = 1

    // Read data
    uint16_t value = (uint16_t)(ADC2_DR & 0xFFFF);

    return value;
}

uint16_t readADC3Channel(uint8_t channel) //polling
{
    // 1 conversion total
    ADC3_SQR1 &= ~(0xF << 20);
    // Select channel number
    ADC3_SQR3 &= ~(0x1F << 0);
    ADC3_SQR3 |= (channel & 0x1F);

    // Sampling time (239.5 cycles)
    if (channel < 10)
        ADC3_SMPR2 |= (7UL << (channel * 3));
    else
        ADC3_SMPR1 |= (7UL << ((channel - 10) * 3));

    // Start conversion
    ADC3_CR2 |= (1UL << 30);        // SWSTART = 1

    // Wait for end of conversion (EOC)
    while (!(ADC3_SR & (1UL << 1)));

    return (uint16_t)(ADC3_DR & 0xFFFF);
}

int initADCxINy(uint8_t adc_number, uint8_t channel)
{
    // Find the entry
    for (int i = 0; i < sizeof(adc_map)/sizeof(adc_map[0]); ++i)
    {
        if (adc_map[i].adc_num == adc_number && adc_map[i].channel == channel)
        {
            // Step 1: init GPIO as analog
            switch (adc_map[i].port)
            {
                case 'A': initGpioAxAsAnalog(adc_map[i].pin); break;
                //case 'B': initGpioBxAsAnalog(adc_map[i].pin); break;
                //case 'C': initGpioCxAsAnalog(adc_map[i].pin); break;
                case 'F': initGpioFxAsAnalog(adc_map[i].pin); break;
                default: return -2; // unsupported port
            }

            // Step 2: enable ADC clock
            if (adc_number == 1)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
            else if (adc_number == 2)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
            else if (adc_number == 3)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

            return 0; // success
        }
    }

    // Not found
    return -1;
}

uint32_t adc_get_data_register_address(uint8_t adc_number)
{
    switch (adc_number)
    {
        case 1: return ADC1_BASE_ADDRESS + 0x4C; //ADC1_DR
        case 2: return ADC2_BASE_ADDRESS + 0x4C; //ADC2_DR
        case 3: return ADC3_BASE_ADDRESS + 0x4C; //ADC3_DR
        default: return 0;
    }
}

void initADC_DMA(uint8_t adc_num, const uint8_t *channels, uint8_t length)
{
    if (length == 0 || channels == NULL) return;

    volatile uint32_t *ADC_CR1_reg;
    volatile uint32_t *ADC_CR2_reg;
    volatile uint32_t *ADC_SQR1_reg;
    volatile uint32_t *ADC_SQR3_reg;
    volatile uint32_t *ADC_SMPR1_reg;
    volatile uint32_t *ADC_SMPR2_reg;

    // Select correct ADC register set
    switch (adc_num)
    {
        case 1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
            ADC_CR1_reg   = &ADC1_CR1;
            ADC_CR2_reg   = &ADC1_CR2;
            ADC_SQR1_reg  = &ADC1_SQR1;
            ADC_SQR3_reg  = &ADC1_SQR3;
            ADC_SMPR1_reg = &ADC1_SMPR1;
            ADC_SMPR2_reg = &ADC1_SMPR2;
            break;

        case 2:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
            ADC_CR1_reg   = &ADC2_CR1;
            ADC_CR2_reg   = &ADC2_CR2;
            ADC_SQR1_reg  = &ADC2_SQR1;
            ADC_SQR3_reg  = &ADC2_SQR3;
            ADC_SMPR1_reg = &ADC2_SMPR1;
            ADC_SMPR2_reg = &ADC2_SMPR2;
            break;

        case 3:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
            ADC_CR1_reg   = &ADC3_CR1;
            ADC_CR2_reg   = &ADC3_CR2;
            ADC_SQR1_reg  = &ADC3_SQR1;
            ADC_SQR3_reg  = &ADC3_SQR3;
            ADC_SMPR1_reg = &ADC3_SMPR1;
            ADC_SMPR2_reg = &ADC3_SMPR2;
            break;

        default:
            return; // Invalid ADC number
    }

    // Common prescaler (PCLK2 / 4)
    ADC_CCR &= ~(3UL << 16);
    ADC_CCR |=  (1UL << 16);

    // --- CONFIGURATION SEQUENCE ---

    // 1. Enable scan mode (multi-channel)
    *ADC_CR1_reg |= (1UL << 8);  // SCAN = 1

    // 2. Enable DMA, continuous mode, DDS (DMA continuous requests)
    *ADC_CR2_reg |= (1UL << 8);  // DMA = 1
    *ADC_CR2_reg |= (1UL << 9);  // DDS = 1
    *ADC_CR2_reg |= (1UL << 1);  // CONT = 1

    // 3. Set number of conversions
    *ADC_SQR1_reg &= ~(0xF << 20);
    *ADC_SQR1_reg |= ((length - 1) << 20);

    // 4. Fill channel sequence into SQR3
    uint32_t sqr3 = 0;
    for (uint8_t i = 0; i < length; ++i)
        sqr3 |= (channels[i] & 0x1F) << (5 * i);
    *ADC_SQR3_reg = sqr3;

    // 5. Configure sampling time (max 239.5 cycles)
    for (uint8_t i = 0; i < length; ++i)
    {
        uint8_t ch = channels[i];
        if (ch < 10)
            *ADC_SMPR2_reg |= (7UL << (ch * 3));
        else
            *ADC_SMPR1_reg |= (7UL << ((ch - 10) * 3));
    }

    // 6. Right alignment, 12-bit resolution
    *ADC_CR2_reg &= ~(1UL << 11);  // ALIGN = 0
    *ADC_CR1_reg &= ~(3UL << 24);  // RES = 00 (12-bit)

    // 7. Enable ADC
    *ADC_CR2_reg |= (1UL << 0);    // ADON = 1
}

void startADC_DMA(uint8_t adc_num)
{
    switch (adc_num)
    {
        case 1: ADC1_CR2 |= (1UL << 30); break;  // SWSTART
        case 2: ADC2_CR2 |= (1UL << 30); break;
        case 3: ADC3_CR2 |= (1UL << 30); break;
    }
}