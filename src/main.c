/* Placeholder main for configuring baremetal essentials before fleshing out the project 

Build the ELF with:

compiler toolchain, the microcontoller type, arm or thumb, doesnt need anything else "ffreestanding", no standard library, no start files, optimization level 0, -g3, all warnings, extra warnings, -T, linker script, -W1, -Map=firmware.map, object files, firmware.elf, startup file name, output file name

arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -ffreestanding -nostdlib -nostartfiles -O0 -g3 -Wall -Wextra -T STM32G031GBUX_FLASH.ld "-Wl,-Map=firmware.map" -o firmware.elf startup_stm32g031xx.s main.c


P.S. commas need quotes in PowerShell
*/

#include <stdint.h>

#define RCC_IOPENR_REG (*(volatile uint32_t *)0x40021034)
#define GPIOC_MODER_REG (*(volatile uint32_t *)0x50000800)
#define GPIOC_BSRR_REG (*(volatile uint32_t *)0x50000818)


int main(void){

    RCC_IOPENR_REG |= (0b1u << 2); // Sets bit for GPIOC

    GPIOC_MODER_REG &= ~(0b11u << 12); // Clears 12-13 for pin 6.
    GPIOC_MODER_REG |= (0b01u << 12); // Sets 12-13

    for(;;){
        
        GPIOC_BSRR_REG = (0b1u << 6); // LED on, Drives PC6 high
        
        for(volatile int i = 0; i < 20000; i++){ } // Delay
        
        GPIOC_BSRR_REG = (0b1u << 22); // LED off, Drives low
        
        for(volatile int i = 0; i < 20000; i++){ }
    }
}