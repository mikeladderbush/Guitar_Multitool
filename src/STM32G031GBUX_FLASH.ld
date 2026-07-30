/* This linker script is based off of an auto-generated from STM32CubeIDE */

/* Entry point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

_Min_Heap_Size = 0x200; // Required heap
_Min_Stack_size = 0x400; // Required stack

/* Memories definition */

MEMORY
{
    RAM     (xrw) : ORIGIN = 0x20000000, LENGTH = 8k
    FLASH_VECTAB (rx)   : ORIGIN = 0x08001000, LENGTH 192
    FLASH (rx) : ORIGIN = ORIGIN(FLASH_VECTAB) + LENGTH(FLASH_VECTAB) = 62k - 32 - LENGTH(FLASH_VECTAB)
    EEPROM (rx) : ORIGIN = 0x0800F800, LENGTH = 2k
    FILE_NAME (rx) : ORIGIN = ORIGIN(EEPROM) - 32, LENGTH = 32
}

/* Sections */
SECTIONS
{
    /* The startup code into FLASH rom type memory */
    .isr_vector :
    {
        . = ALIGN(4); // 4 byte alignment
        KEEP(*(.isr_vector)) // Startup Code
        . = ALIGN(4);
    } >FLASH_VECTAB

    /* The program code and other data into FLASH rom type memory */
    .text :
    {
        . = ALIGN(4);
        *(.text) // .text sections (code)
        *(.text*) // .text* sections
        *(.glue_7) // glue arm to thumb code
        *(.glue_7t) // glue thumb to arm code
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .; // Global symbols defined at the end of code
    } >FLASH

    /* Constant data into FLASH ROM */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata) // constants, strings, etc.
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .ARM.extab : {
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } >FLASH

    .ARM : {
        . = ALIGN(4);
        __exidx _start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } >FLASH

    .preinit_array
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } >FLASH

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >FLASH

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >FLASH

    .file_name :
    {
        . = ALIGN(4);
        KEEP (*(.file_name))
        . = ALIGN(4);
    } >FILE_NAME

    /* Used by startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data sections into RAM type memory */
    .data :
    {
        . = ALIGN(4);
        _sdata = .; // Global symbol at data start
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH

    /* Unitialized data section into RAM */
    . = ALIGN(4);
    .bss :
    {
        /* Initializing the .bss section */
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /* Heap section used to check if there is enough RAM left */
    ._user_heap_stack:
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = .);
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_size;
        . = ALIGN(8);
    } >RAM

    /* Remove information from the compiler libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}