/**

    * The information in this file was largely taken from the startup_stm32g031xx.s file created by the MCD Application Team.

*/

    .syntax unified
    .cpu cortex-m0plus
    .fpu softvfp
    .thumb

    .global g_pfnVectors
    .global Default_Handler

    /* start address for the initialization values of the .data section. Defined in the linker script*/
    .word _sidata
    /* start address for the .data section */
    .word _sdata
    /* end address for .data */
    .word _edata
    .word _sbss // same for .bss
    .word _ebss

    /* Code that starts immediately upon execution after a reset, eventually leads to a main() routine being called */
    .section .text.Reset_Handler
    .weak Reset_Handler // weakly aliases the reset handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_estack // load the zero register with the stack pointer
    mov sp, r0       // set stack pointer to r0

/* Initialize system clock */
    bl SystemInit

/* Copy data segment initializers from flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit 

/* Loads into register 4 the location found at r2 offset by r3, then stores the value of register 4 at the location r0 offset r3. Then the offset is incremented by 32 bits (4 bytes). */
CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

/* Loops the data initialization */
LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

/* zero fills bss */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

/* stores 0 in register 2, increments it by 32 bits, repeat. */
FillZerobss:
    str r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

/* static constructor calls for libc */
    bl __libc_init_array
/* calls the application at entry point main */
    bl main

LoopForever:
    b LoopForever

/* Debug metadata for recording the Reset_Handler size. */
.size Reset_Handler, .-Resent_Handler

/* Code that gets called for processor receiving unexpected interrupt. Enters an infinite loop and preserves the system state for debugger examination */

    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
    .size Default_Handler, .-Default_Handler

/* Vector Table for Cortex M0 */
    .section .isr_vector,"a",%progbits
    .type g_pfnVectors, %object
    .size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word 0
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler
    .word WWDG_IRQHandler // Window WatchDog
    .word PVD_IRQHandler // PVD trhough EXTI line detect
    .word RTC_TAMP_IRQHandler // RTC through the EXTI line
    .word FLASH_IRQHandler // FLASH
    .word RCC_IRQHandler // RCC
    .word EXTI0_1_IRQHandler // EXTI line 0 and 1
    .word EXTI2_3_IRQHandler // EXTI line 2 and 3
    .word EXTI4_15_IRQHandler // EXTI line 4 to 15
    .word 0                   // Reserved
    .word DMA1_Channel1_IRQHandler // DMA1 Channel 1
    .word DMA1_Channel2_3_IRQHandler // DMA1 Channel 2 and 3
    .word DMA1_ch4_5_DMAMUX1_OVR_IRQHandler // DMA1 Channel 4 to 5, DMAMUX1 overrun
    .word ADC1_IRQHandler // ADC1
    .word TIM1_BRK_UP_TRG_COM_IRQHandler // TIM1 Break, Update, Trigger and Commutation
    .word TIM1_CC_IRQHandler // TIM1 Capture Compare
    .word TIM2_IRQHandler // TIM2
    .word TIM3_IRGHandler // TIM3
    .word LPTIM1_IRQHandler // LPTIM1
    .word LPTIM2_IRQHandler // LPTIM2
    .word TIM14_IRQHandler // TIM14
    .word 0                // Reserved
    .word TIM16_IRQHandler // TIM16
    .word TIM17_IRQHandler //TIM17
    .word I2C1_IRQHandler // I2C1
    .word I2C2_IRQHandler // I2C2
    .word SPI1_IRQHandler // SPI1
    .word SPI2_IRQHandler // SPI2
    .word USART1_IRQHandler // USART1
    .word USART2_IRQHandler // USART2
    .word LPUART1_IRGHandler // LPUART1
    .word 0                 // Reserved


    /* weak aliases for each handler to the default */
  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler

  .weak      PVD_IRQHandler
  .thumb_set PVD_IRQHandler,Default_Handler

  .weak      RTC_TAMP_IRQHandler
  .thumb_set RTC_TAMP_IRQHandler,Default_Handler

  .weak      FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler

  .weak      RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler

  .weak      EXTI0_1_IRQHandler
  .thumb_set EXTI0_1_IRQHandler,Default_Handler

  .weak      EXTI2_3_IRQHandler
  .thumb_set EXTI2_3_IRQHandler,Default_Handler

  .weak      EXTI4_15_IRQHandler
  .thumb_set EXTI4_15_IRQHandler,Default_Handler

  .weak      DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

  .weak      DMA1_Channel2_3_IRQHandler
  .thumb_set DMA1_Channel2_3_IRQHandler,Default_Handler

  .weak      DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler
  .thumb_set DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler,Default_Handler

  .weak      ADC1_IRQHandler
  .thumb_set ADC1_IRQHandler,Default_Handler

  .weak      TIM1_BRK_UP_TRG_COM_IRQHandler
  .thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler,Default_Handler

  .weak      TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler

  .weak      TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler

  .weak      TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler

  .weak      LPTIM1_IRQHandler
  .thumb_set LPTIM1_IRQHandler,Default_Handler

  .weak      LPTIM2_IRQHandler
  .thumb_set LPTIM2_IRQHandler,Default_Handler

  .weak      TIM14_IRQHandler
  .thumb_set TIM14_IRQHandler,Default_Handler

  .weak      TIM16_IRQHandler
  .thumb_set TIM16_IRQHandler,Default_Handler

  .weak      TIM17_IRQHandler
  .thumb_set TIM17_IRQHandler,Default_Handler

  .weak      I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler

  .weak      I2C2_IRQHandler
  .thumb_set I2C2_IRQHandler,Default_Handler

  .weak      SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak      SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler

  .weak      USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler

  .weak      USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler

  .weak      LPUART1_IRQHandler
  .thumb_set LPUART1_IRQHandler,Default_Handler

/* The contents of this file have been copied from startup_stm32g031xx.s authored and owned by STMicroelectronics with minor changes/addendum */

/* END OF FILE */
