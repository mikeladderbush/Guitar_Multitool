# Guitar_Multitool -- bare-metal STM32G031
#
#   make            build firmware.elf/.bin/.hex
#   make inspect    run every sanity check, print to console
#   make report     same checks, written to build/reports/*.txt
#   make clean

TARGET   := firmware
BUILD    := build
LDSCRIPT := STM32G031GBUX_FLASH.ld

SRCS := startup_stm32g031xx.s main.c

# ---------------------------------------------------------------- toolchain
PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
SIZE    := $(PREFIX)size
NM      := $(PREFIX)nm

# MinGW gcc needs a writable *native* Windows temp dir. Under msys make, TMP
# arrives unset and gcc falls back to C:\Windows, failing with "Cannot create
# temporary file". A POSIX /c/... path is rejected too -- it has to be the
# C:/... form that cygpath -m produces. No-op on Linux/macOS.
export TMP  := $(shell cygpath -m "$(CURDIR)/$(BUILD)/tmp" 2>/dev/null || echo "$(CURDIR)/$(BUILD)/tmp")
export TEMP := $(TMP)

# -------------------------------------------------------------------- flags
MCU := -mcpu=cortex-m0plus -mthumb

CFLAGS := $(MCU) -ffreestanding -O0 -g3 -Wall -Wextra

LDFLAGS := $(MCU) -nostdlib -nostartfiles -T$(LDSCRIPT) \
	-Wl,-Map=$(BUILD)/$(TARGET).map,--cref

# ------------------------------------------------------------------ objects
OBJS := $(addprefix $(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))

ELF := $(BUILD)/$(TARGET).elf
BIN := $(BUILD)/$(TARGET).bin
HEX := $(BUILD)/$(TARGET).hex

.PHONY: all
all: $(BIN) $(HEX)
	@$(SIZE) $(ELF)

$(BUILD) $(BUILD)/tmp $(BUILD)/reports:
	@mkdir -p $@

$(BUILD)/%.o: %.c | $(BUILD) $(BUILD)/tmp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s | $(BUILD) $(BUILD)/tmp
	$(CC) $(CFLAGS) -c $< -o $@

# Relinks if the linker script changes, not just the sources.
$(ELF): $(OBJS) $(LDSCRIPT) | $(BUILD)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# ------------------------------------------------------------------ checks
.PHONY: inspect
inspect: $(ELF)
	@echo "=============== undefined symbols (want: empty) ==============="
	@$(NM) -u $(ELF) || true
	@echo ""
	@echo "=============== sections (.isr_vector @ 0x08000000) ==========="
	@echo "===============  and .data VMA != LMA              ==========="
	@$(OBJDUMP) -h $(ELF)
	@echo ""
	@echo "=============== vector table ================================="
	@echo "=============== word0 0x2000____ , word1 0x0800____ and ODD ==="
	@$(OBJDUMP) -s -j .isr_vector $(ELF)
	@echo ""
	@echo "=============== size ========================================="
	@$(SIZE) -A -x $(ELF)
	@echo ""
	@echo "=============== symbols by address ==========================="
	@$(NM) -n $(ELF)

# Same information, written to files. Uses > so each run replaces the last.
.PHONY: report
report: $(ELF) | $(BUILD)/reports
	@$(NM)      -u          $(ELF) >  $(BUILD)/reports/undefined.txt
	@$(OBJDUMP) -h          $(ELF) >  $(BUILD)/reports/sections.txt
	@$(OBJDUMP) -s -j .isr_vector $(ELF) > $(BUILD)/reports/isr_vector.txt
	@$(SIZE)    -A -x       $(ELF) >  $(BUILD)/reports/size.txt
	@$(NM)      -n          $(ELF) >  $(BUILD)/reports/symbols.txt
	@$(OBJDUMP) -d          $(ELF) >  $(BUILD)/reports/disassembly.txt
	@echo "wrote $(BUILD)/reports/*.txt"

.PHONY: disasm
disasm: $(ELF)
	@$(OBJDUMP) -S $(ELF)

# ------------------------------------------------------------------- flash
# For when the Nucleo arrives.
.PHONY: flash
flash: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg \
		-c "program $< verify reset exit"

.PHONY: clean
clean:
	rm -rf $(BUILD)
