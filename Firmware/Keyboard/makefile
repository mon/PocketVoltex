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
F_CPU        = 8000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = Keyboard
SRC          = $(TARGET).c Descriptors.c Config.c Encoder.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

AVRDUDE      = avrdude -B 8 -c usbasp -p $(MCU)


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

debug: CC_FLAGS += -DDEBUG
debug: clean flash

init: erase wfuse flashboot flash

initboot: erase wfuse flashboot

erase:
	$(AVRDUDE) -e

rfuse:
	$(AVRDUDE) -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h

wfuse:
	$(AVRDUDE) -U lfuse:w:0xde:m -U hfuse:w:0xdb:m -U efuse:w:0xf6:m

# To make this hex, compile HID Bootloader in LUFA
# FLASH_SIZE_KB        := 16
# BOOT_SECTION_SIZE_KB := 2
# MCU          = atmega16u2
# ARCH         = AVR8
flashboot:
	$(AVRDUDE) -U flash:w:DFU/BootloaderHID.hex:i

flash: all
	sleep 1
	python DFU/hid_bootloader_loader.py atmega16u2 Keyboard.hex
