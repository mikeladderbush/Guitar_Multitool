#include <stdint.h>

#include "reg_manip.h"

#define RCC_IOPENR_REG (*(volatile uint32_t*)0x40021034)
#define RCC_APBENR1_REG (*(volatile uint32_t*)0x4002103C)
#define RCC_APBENR2_REG (*(volatile uint32_t*)0x40021040)

#define ADC_CR_REG (*(volatile uint32_t*)0x40012408)
#define ADC_CHSELR_REG (*(volatile uint32_t*)0x40012428)
#define ADC_SMPR_REG (*(volatile uint32_t*)0x40012414)
#define ADC_ISR_REG (*(volatile uint32_t*)0x40012400)
#define ADC_DR_REG (*(volatile uint32_t*)0x40012440)

#define I2C_CR1_REG (*(volatile uint32_t*)0x40005400)
#define I2C_CR2_REG (*(volatile uint32_t*)0x40005404)
#define I2C_TIMINGR_REG (*(volatile uint32_t*)0x40005410)
#define I2C_TXDR_REG (*(volatile uint32_t*)0x40005428)
#define I2C_ISR_REG (*(volatile uint32_t*)0x40005418)

#define GPIOA_MODER_REG (*(volatile uint32_t*)0x50000000)

#define GPIOB_AFRL_REG (*(volatile uint32_t*)0x50000420) // Alt function PB6
#define GPIOB_AFRH_REG (*(volatile uint32_t*)0x50000424) // Alt function PB9
#define GPIOB_OTYPER_REG (*(volatile uint32_t*)0x50000404)
#define GPIOB_MODER_REG (*(volatile uint32_t*)0x50000400)
#define GPIOB_BSRR_REG (*(volatile uint32_t*)0x50000418)
#define GPIOB_PUPDR_REG (*(volatile uint32_t*)0x5000040C)
#define GPIOB_IDR_REG (*(volatile uint32_t*)0x50000410)


/*
*********************************************************************************************

        Button initialization.
        PB6 and PB9 are I2C1_SCL and I2C1_SDA respectively.


*********************************************************************************************
*/
void init_buttons(){

    RCC_IOPENR_REG |= (0b1u << 1); // Sets bit for GPIOB

    GPIOB_MODER_REG &= ~(0b11u << 0); // clears 0-1 for pin 0
    GPIOB_MODER_REG &= ~(0b11u << 2); // clears 2-3 for pin 1
    GPIOB_MODER_REG &= ~(0b11u << 4); // clears 4-5 for pin 2

    GPIOB_PUPDR_REG &= ~(0b11u << 0);
    GPIOB_PUPDR_REG &= ~(0b11u << 2);
    GPIOB_PUPDR_REG &= ~(0b11u << 4);
    GPIOB_PUPDR_REG |= (0b01u << 0); // set pull-up on pin 0
    GPIOB_PUPDR_REG |= (0b01u << 2); // set pull-up on pin 1 
    GPIOB_PUPDR_REG |= (0b01u << 4); // set pull-up on pin 2

}


/*
*********************************************************************************************

    Initialization and sampling of ADC for randomness.

*********************************************************************************************
*/
void seed_rng(){
        
    RCC_IOPENR_REG |= (0b1u << 0);

    RCC_APBENR2_REG |= (0b1u << 20);

    GPIOA_MODER_REG &= ~(0b11u << 0); // Clear and set GPIOA PA0 to analog.
    GPIOA_MODER_REG |= (0b11u << 0);

    ADC_CR_REG |= (0b1u << 28); // ADC Regen

    for (volatile uint16_t i = 0; i < 400; i++){}
    ADC_CR_REG |= (0b1u << 31); // ADC Calibrate
    while ((ADC_CR_REG >> 31) & 0b1u){} // Spin until calibrate flag is finished.
    ADC_CR_REG |= (0b1u << 0); // ADC Enabled

    while (((ADC_ISR_REG >> 0) & 0b1u) == 0){} // Wait for ready flag.

    ADC_CHSELR_REG |= (0b1u << 0); // Channel Selection (Channel 0)

    ADC_SMPR_REG &= ~(0b111u << 0); // Set sample rate in sampler to 1.5 ADC clock cycles.
    ADC_SMPR_REG &= ~(0b1u << 8); // Set channel 0 sample selection to SMP1

    uint32_t accumulated_bits;

    for (uint8_t i = 0; i < 32; i++){

        ADC_CR_REG |= (0b1u << 2); // ADC Start

        while(((ADC_ISR_REG >> 2) & 0b1u) == 0){} // Wait until EOC flag is set

        accumulated_bits = (accumulated_bits << 1) | (ADC_DR_REG & 0b1u); // Shift accumulated bits and then set to ADC_DR value.
    }

    rng_state = accumulated_bits;

}

uint32_t rng_next(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}


/*
*********************************************************************************************

    Tools for drawing to the display.

    I2C Timing Calculations:
        Based on generic timing example for 100kHz standard mode
        SCL period = (SCLL + 1 + SCLH + 1) * (PRESC + 1) / 16 MHz
        PRESC = 0x3
        SCLL + 1 = 20 = 0x13 + 1
        SCLH + 1 = 20 = 0x13 allowed for time rise = 0x0F
        SDADEL = 0x2
        SCLDEL = 0x4

    GDB Testing Tools:
        Setting the PE bit on the I2C_CR1 Register to enable the I2C peripheral:
            0x40005400 = 1
        Clear the NACKF and STOPF flags from the I2C_ICR Register:
            0x4000541C = 0x30
        Describe and launch transfer on the I2C_CR2 register:
            0x40005404 = 0x02002078
        Read the result from I2C_ISR:
            read 0x40005418

    SSD1306 Charge Pump requires two commands to be activated:
        From SSD1306 Manual, Solomon Systech
        "
        Note
            (1) The Charge Pump must be
            enabled by the following command:
            8Dh ; Charge Pump Setting
            14h ; Enable Charge Pump
            AFh; Display ON
        "

*********************************************************************************************
*/
void init_display_conn(){

    RCC_APBENR1_REG |= (0b1u << 21); // Set I2C clock on

    I2C_CR1_REG &= ~(0b1u << 0); // Set PE disabled
    I2C_TIMINGR_REG = 
                        (0x3u << 28) // Hex PRESC = 3, 250ns tick
                    |   (0x4u << 20) // SCLDEL = 4, 4 ticks before rise
                    |   (0x2u << 16) // SDADEL = 2, 2 ticks after fall
                    |   (0x0Fu << 8) // SCLH = 15, 16 ticks high
                    |   (0x13u << 0); // SCLL = 19, 20 ticks low

    GPIOB_OTYPER_REG |= (0b1u << 6);
    GPIOB_OTYPER_REG |= (0b1u << 9);

    GPIOB_AFRL_REG &= ~(0b1111u << 24); // PB6 alt function AF6
    GPIOB_AFRH_REG &= ~(0b1111u << 4); // PB9 alt function AF6
    GPIOB_AFRL_REG |= (0b0110u << 24); // I2C1_SCL
    GPIOB_AFRH_REG |= (0b0110u << 4); // I2C1_SDA

    GPIOB_MODER_REG &= ~(0b11u << 12);
    GPIOB_MODER_REG &= ~(0b11u << 18);
    GPIOB_MODER_REG |= (0b10u << 12); // Alt for PB6
    GPIOB_MODER_REG |= (0b10u << 18); // Alt for PB9

    I2C_CR1_REG |= (0b1u << 0); // Enable PE
    I2C_CR2_REG = 
                    (0x3C << 1) // Set CR2 Slave Address
                |   (0b1u << 10) // Set to read
                |   (0b11111111u << 16) // Set NBYTES for init check
                |   (0b1u << 25) // Auto-end to 1
                |   (0b1u << 13); // Start

    const uint8_t init_bytes[] = { 0x00, 0x8D, 0x14, 0xAF, 0xA5 };
    for (int i = 0; i < sizeof(init_bytes); i++){
        while ((I2C_ISR_REG >> 0 & 0b1u) == 0){} 
        // See documentation above for charge pump explanation.
        I2C_TXDR_REG = init_bytes[i];
    }

}


/*
*********************************************************************************************

    Button Checks and manipulations.

*********************************************************************************************
*/
uint8_t check_button(uint8_t button_num){
    uint8_t checked_button = (GPIOB_IDR_REG >> button_num) & 0b1u;
    return checked_button;
}