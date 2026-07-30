/* Placeholder main for configuring baremetal essentials before fleshing out the project 

Build the ELF with:

compiler toolchain, the microcontoller type, arm or thumb, doesnt need anything else "ffreestanding", no standard library, no start files, optimization level 0, -g3, all warnings, extra warnings, -T, linker script, -W1, -Map=firmware.map, object files, firmware.elf, startup file name, output file name

arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -ffreestanding -nostdlib -nostartfiles -O0 -g3 -Wall -Wextra -T STM32G031GBUX_FLASH.ld "-Wl,-Map=firmware.map" -o firmware.elf startup_stm32g031xx.s main.c


P.S. commas need quotes in PowerShell
*/

int data_placeholder = 1; // .data test for debugging
int bss_placeholder; // .bss test for debugging

int main(void){

    data_placeholder = data_placeholder + 1;
    bss_placeholder = bss_placeholder + 1;

    for(;;){}
}