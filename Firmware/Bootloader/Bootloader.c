/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the HID class bootloader. This file contains the complete bootloader logic.
 */

#include "Bootloader.h"

/** Flag to indicate if the bootloader should be running, or should exit and allow the application code to run
 *  via a soft reset. When cleared, the bootloader will abort, the USB interface will shut down and the application
 *  started via a forced watchdog reset.
 */
static bool RunBootloader = true;

uint16_t MagicBootKey ATTR_NO_INIT;


/** Special startup routine to check if the bootloader was started via a watchdog reset, and if the magic application
 *  start key has been loaded into \ref MagicBootKey. If the bootloader started via the watchdog and the key is valid,
 *  this will force the user application to start via a software jump.
 */
void Application_Jump_Check(void)
{
    /* Always boot bootloader if RESET held
     * NOTE: This makes the assumption that
     *  A) No code on the device uses RESET as an output
     *  B) The RSTDISBL fuse is set, and this value is valid
     *  C) There is an external pullup on the RESET pin
     */
    if(!(PINC & _BV(1)))
        return;
	/* If power on boot or magic key set */
	if ((MCUSR & _BV(PORF)) || MagicBootKey == MAGIC_BOOT_KEY)
	{
        // If there's no code, this avoids an infinite reboot loop
        MCUSR &= ~_BV(PORF);
		// cppcheck-suppress constStatement
		((void (*)(void))0x0000)();
	}
}

/** Main program entry point. This routine configures the hardware required by the bootloader, then continuously
 *  runs the bootloader processing routine until instructed to soft-exit.
 */
int main(void)
{
	/* Setup hardware required for the bootloader */
	SetupHardware();

	/* Enable global interrupts so that the USB stack can function */
	GlobalInterruptEnable();

	while (RunBootloader)
        USB_USBTask();

	/* Disconnect from the host - USB interface will be reset later along with the AVR */
	USB_Detach();

	/* Unlock the forced application start mode of the bootloader if it is restarted */
	MagicBootKey = MAGIC_BOOT_KEY;

	/* Enable the watchdog and force a timeout to reset the AVR */
	wdt_enable(WDTO_250MS);

	for (;;);
}

/** Configures all hardware required for the bootloader. */
static void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Relocate the interrupt vector table to the bootloader section */
	MCUCR = (1 << IVCE);
	MCUCR = (1 << IVSEL);

	/* Initialize USB subsystem */
	USB_Init();
}

/** Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
 *  to relay data to and from the attached USB host.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
    Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
    if(!Endpoint_IsSETUPReceived()) {
        return;
    }
    if(USB_ControlRequest.bRequest == CONFIG_ID) {
        Endpoint_ClearSETUP();
        uint16_t PageAddress = USB_ControlRequest.wValue;
        if(USB_ControlRequest.wIndex == COMMAND_PROGRAM) {
            /* Wait until the command has been sent by the host */
			while (!(Endpoint_IsOUTReceived()));
            /* Erase the given FLASH page, ready to be programmed */
            boot_page_erase(PageAddress);
            boot_spm_busy_wait();

            /* Write each of the FLASH page's bytes in sequence */
            for (uint8_t PageWord = 0; PageWord < (SPM_PAGESIZE / 2); PageWord++)
            {
                /* Check if endpoint is empty - if so clear it and wait until ready for next packet */
                if (!(Endpoint_BytesInEndpoint()))
                {
                    Endpoint_ClearOUT();
                    while (!(Endpoint_IsOUTReceived()));
                }

                /* Write the next data word to the FLASH page */
                boot_page_fill(PageAddress + ((uint16_t)PageWord << 1), Endpoint_Read_16_LE());
            }

            /* Write the filled FLASH page to memory */
            boot_page_write(PageAddress);
            boot_spm_busy_wait();

            /* Re-enable RWW section */
            boot_rww_enable();
        } else if(USB_ControlRequest.wIndex == COMMAND_VERSION) {
            Endpoint_Write_16_LE(65000);
            Endpoint_ClearIN();
        } else if(USB_ControlRequest.wIndex == COMMAND_STARTAPPLICATION) {
            RunBootloader = false;
        }
        Endpoint_ClearOUT();
        Endpoint_ClearStatusStage();
        return;
    }
    
    USB_Process_BOS();
}