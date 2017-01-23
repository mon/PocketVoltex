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
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <avr/pgmspace.h>

		#include <LUFA/Drivers/USB/USB.h>
        
        #include "Config.h"
        #include "LED.h"

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// Keyboard HID Interface
			USB_Descriptor_Interface_t            HID1_Interface;
			USB_HID_Descriptor_HID_t              HID1_KeyboardHID;
			USB_Descriptor_Endpoint_t             HID1_ReportINEndpoint;
            
            // Mouse HID Interface
			USB_Descriptor_Interface_t            HID2_MouseInterface;
			USB_HID_Descriptor_HID_t              HID2_MouseHID;
			USB_Descriptor_Endpoint_t             HID2_ReportINEndpoint;
            
            // LED HID Interface for pretty lights
			USB_Descriptor_Interface_t            HID3_Interface;
			USB_HID_Descriptor_HID_t              HID3_LEDHID;
			USB_Descriptor_Endpoint_t             HID3_ReportINEndpoint;
            
			// Config interface as we can't talk to composite directly
			USB_Descriptor_Interface_t            Config_Interface;
			USB_Descriptor_Endpoint_t             Config_DataInEndpoint;
			USB_Descriptor_Endpoint_t             Config_DataOutEndpoint;
		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device.
		 */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_Keyboard = 0, /**< Keyboard interface descriptor ID */
            INTERFACE_ID_Mouse    = 1, /**< Mouse interface descriptor ID */
            INTERFACE_ID_LED      = 2, /**< LED interface descriptor ID  */
            INTERFACE_ID_Config   = 3  /**< Config interface descriptor ID  */
		};

		/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 */
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
            STRING_ID_Knobs        = 3, 
            STRING_ID_Config       = 4, 
            STRING_ID_LED          = 5,  /**< LED string ID */
            STRING_ID_LED_Indiv    = 6,
		};
        
        enum URLDescriptors_t
        {
            URL_ID_Localhost  = 1,
            URL_ID_Config     = 2,
        };

	/* Macros: */
		#define KEYBOARD_EPADDR              (ENDPOINT_DIR_IN  | 1)
        #define MOUSE_IN_EPADDR              (ENDPOINT_DIR_IN  | 2)
        #define LED_EPADDR                   (ENDPOINT_DIR_IN  | 3)
        #define CONFIG_IN_EPADDR             (ENDPOINT_DIR_IN  | 4)
        #define CONFIG_OUT_EPADDR            (ENDPOINT_DIR_OUT | 5)

		#define KEYBOARD_EPSIZE              SWITCH_COUNT + 2
        #define MOUSE_EPSIZE                 8
        #define LED_EPSIZE                   LED_TOTAL_COUNT
        // MUST be 8, 16, 32 or 64 bytes
        #define CONFIG_EPSIZE CONFIG_BYTES <= 8 ? 8 :   \
                              CONFIG_BYTES <= 16 ? 16 : \
                              CONFIG_BYTES <= 32 ? 32 : 64 // if it's larger than 64 bytes, you're out of luck

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint16_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
        
        void USB_Process_BOS(void);

#endif

