# Guitar_Multitool

A bare-metal guitar practice tool on an STM32G031, driving a small OLED.

No HAL, no CMSIS, no RTOS. Registers are written by hand from RM0444. The point
of the project is to learn register-level embedded development, so convenience
layers are deliberately avoided even where they'd be faster.

## Status

**Blinking on hardware.** Vector table, reset handler, and linker script verified
on a NUCLEO-G031K8, with GPIO driven by hand-written register definitions. Flash
and debug work over SWD via OpenOCD and GDB.

| Stage | State |
|---|---|
| Toolchain + build | done |
| Linker script, vector table, reset handler | done, verified |
| `.data` copy / `.bss` zero | done, verified via LMA/VMA |
| Blink (GPIO) | done — PC6 via RCC_IOPENR / MODER / BSRR |
| I2C + SSD1306 display | not started |
| Application logic | not started |

## Hardware

| | |
|---|---|
| **Target MCU** | STM32G031F8P6 — Cortex-M0+, TSSOP-20, 64K flash / 8K RAM |
| **Bring-up board** | NUCLEO-G031K8 — same die, LQFP-32, on-board ST-LINK |
| **Display** | 0.66" 64x48 OLED, SSD1306 controller, I2C |
| **Debug** | ST-LINK over SWD |

The G031K8 on the Nucleo and the bare G031F8P6 have identical memory maps, so
the same linker script serves both. Only the pinout differs.

## Layout

```
src/
  main.c                      placeholder main + .data/.bss test globals
  startup_stm32g031xx.s       vector table, Reset_Handler
  STM32G031GBUX_FLASH.ld      memory map (adapted from STM32CubeIDE)
  Makefile                    build + inspection targets
  useful_commands.txt         objdump/nm/size/readelf reference
  build/                      generated, gitignored
```

## Build

Run from `src/`. Use **Git Bash**, not PowerShell — see gotchas.

```
make            build firmware.elf/.bin/.hex, print size
make inspect    all sanity checks, printed to console
make report     same checks written to build/reports/*.txt
make disasm     disassembly interleaved with source
make clean      wipe build/
make flash      program via ST-LINK (needs OpenOCD + hardware)
```

Adding a source file means appending it to `SRCS` in the Makefile.

## Toolchain

Installed from MSYS2, which puts everything on PATH alongside the existing
`make` and host `gcc`:

```
pacman -S mingw-w64-x86_64-arm-none-eabi-gcc \
          mingw-w64-x86_64-arm-none-eabi-binutils \
          mingw-w64-x86_64-arm-none-eabi-newlib
```

The debugger is `gdb-multiarch` (`pacman -S mingw-w64-x86_64-gdb-multiarch`).
Note there is no `arm-none-eabi-gdb` package in MSYS2 — tutorials referencing
that name mean `gdb-multiarch` here.

## What "correct" looks like

`make inspect` should show all four:

1. **No undefined symbols.**
2. **`.isr_vector` at `0x08000000`.** The hardware reads the vector table from
   the start of flash; anywhere else and it loads garbage as SP and PC.
3. **`.data` VMA != LMA.** VMA in RAM (`0x20000000`), LMA in flash. If they
   match, the copy loop copies something onto itself and initialized globals
   come up as garbage.
4. **Vector table words.** Word 0 = `0x20002000` (RAM base + 8K). Word 1 =
   a `0x0800____` address that is **odd** — bit 0 set means Thumb, and a clear
   bit 0 faults the core immediately.

`objdump -s` prints raw little-endian bytes, so each word reads back-to-front:
`00200020` on screen is `0x20002000`.

## Gotchas

Things that cost real time, recorded so they don't cost it twice.

**PowerShell mangles `-Wl,` flags.** It treats the comma as an array separator
and fails with "Missing argument in parameter list". Every `-Wl,...` flag hits
this. Quote them, or just build from Git Bash.

**msys make gives MinGW gcc a broken `TMP`.** It arrives unset, gcc falls back
to the Win32 default of `C:\Windows`, and every compile dies with "Cannot create
temporary file". Setting a POSIX `/c/...` path doesn't help — it must be the
native `C:/...` form. The Makefile derives one with `cygpath -m`.

**ST's startup file assumes the full Cube environment.** It calls `SystemInit`
(clock config, lives in `system_stm32g0xx.c`) and `__libc_init_array` (runs
`.init_array`, drags in newlib, which then wants `_init` from the C runtime that
`-nostartfiles` excluded). Both calls were removed. If C++ gets added later,
`.init_array` needs its own small loop rather than newlib.

**`.weak` only covers the exact spelling.** The startup file's
`.weak`/`.thumb_set` pairs alias unused handlers onto `Default_Handler`, but a
typo in the vector table (`TIM3_IRGHandler` vs `TIM3_IRQHandler`) has no weak
definition at all and fails to link.

**A bare name in a linker script means "input file".** A stray space inside
`__exidx_start` made `ld` report `cannot find __exidx: No such file` — it was
parsing the symbol fragment as a filename.

**`.gitignore` only applies to untracked files.** Build artifacts that were
already committed keep being tracked; `git rm --cached` is the fix.

### Ahead of time, for later

**SSD1306 on a 64x48 panel** needs a **column offset of 32** — the controller
is a 128x64 part and the panel is a centred window on it. Otherwise text lands
off-screen. This is the classic failure with this display.

**PA13/PA14 are SWD**, and PA14 doubles as BOOT0 on TSSOP-20. Reconfiguring
them in firmware locks you out of the chip. Wire NRST so the debugger can
connect under reset.

**No clock setup is needed** to start. The G0 boots on HSI16 at 16 MHz, which is
fine for I2C and for this workload. A PLL is a later optimization, not a
prerequisite.

## Reference docs

| Doc | Covers |
|---|---|
| **RM0444** | STM32G0x1 reference manual — every peripheral register |
| **STM32G031 datasheet** | Memory map, pinout, electrical characteristics |
| **PM0223** | Cortex-M0+ programming manual — the *core*: vector table, NVIC, SysTick |
| **UM2591** | NUCLEO-G031K8 board manual (LD3 is on PC6) |
| **SSD1306 datasheet** | Display command table |

ST documents the peripherals, ARM documents the CPU. Core registers like `NVIC`
and `SysTick` are in PM0223, not RM0444 — a common source of fruitless
searching.

Useful reading: [From Zero to main()](https://interrupt.memfault.com/tag/zero-to-main/)
(targets a SAMD21, so concepts transfer but addresses do not).

## Open design decisions

Not yet settled, and deliberately left open until there's hardware to learn from:

- Scope of v1 — fretboard note recall is the core; chord progressions are a
  stretch goal conditional on flash/RAM headroom.
- Display budget — text only, or graphics? Font size and characters per line.
- Timing model — busy loop, SysTick, or a hardware timer for the reveal delay.
- Input — button count, polled vs interrupt-driven.
- Randomness seeding — ADC noise on a floating pin, or SysTick count at first
  button press. Without one, every boot replays the same drill sequence.
