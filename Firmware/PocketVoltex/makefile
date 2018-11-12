#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = USER
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = PocketVoltex
SRC          = $(TARGET).c asciihid.c Descriptors.c Config.c Encoder.c LED.c LEDPatterns.c Macro.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -Werror
LD_FLAGS     =

AVRDUDE      = avrdude -c usbasp -p $(MCU)


# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk

# This does not work with variables as pre-requisites. Use `make test TARGET=TestingApp`
#test: TARGET = TestingApp
test: all
	cat TestingApp.hex | grep -v '^:00000001FF' > TestFull.hex
	cat DFU/BootloaderUSB.hex >> TestFull.hex

$(TARGET)_WithBoot.hex: $(TARGET).hex
	cat $(TARGET).hex | grep -v '^:00000001FF' > $(TARGET)_WithBoot.hex
	cat DFU/BootloaderUSB.hex >> $(TARGET)_WithBoot.hex

debug: CC_FLAGS += -DDEBUG
debug: clean flash

rev4: CC_FLAGS += -DSOFT_LEDS
rev4: clean all

initboot: erase wfuse flashboot
initboot_rev4: erase wfuse flashboot_rev4

init: $(TARGET)_WithBoot.hex
	$(AVRDUDE) -U lfuse:w:0xde:m -U hfuse:w:0xda:m -U efuse:w:0xfe:m -U flash:w:$(TARGET)_WithBoot.hex:i
init_rev4: initboot_rev4 disablereset flash

erase:
	$(AVRDUDE) -e

rfuse:
	$(AVRDUDE) -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h

wfuse:
	# no more HWB, boot directly to bootloader
	$(AVRDUDE) -U lfuse:w:0xde:m -U hfuse:w:0xda:m -U efuse:w:0xfe:m

disablereset:
	$(AVRDUDE) -U lfuse:w:0xde:m -U hfuse:w:0x9a:m -U efuse:w:0xfe:m

# To make this hex, compile the bootloader up a level
flashboot_rev4:
	$(AVRDUDE) -U flash:w:DFU/BootloaderUSB_rev4.hex:i
flashboot:
	$(AVRDUDE) -U flash:w:DFU/BootloaderUSB.hex:i

flash: all
	cd DFU && python control_bootloader.py atmega16u2 ../$(TARGET).hex

flash_with_boot: $(TARGET)_WithBoot.hex
	$(AVRDUDE) -U flash:w:$(TARGET)_WithBoot.hex:i

testflash: test wfuse
	$(AVRDUDE) -U flash:w:TestFull.hex:i

md5: all
	md5sum $(TARGET).bin