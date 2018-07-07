#include "Descriptors.h"

#include "Config.h"
#include "LED.h"

#define WEBUSB_ID 0x01
#define MS_OS_ID 0x02

const USB_Descriptor_HIDReport_Datatype_t PROGMEM LEDReport[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x00), /* Undefined */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        // Globals
        HID_RI_REPORT_COUNT(8, LED_TOTAL_COUNT),
        HID_RI_REPORT_SIZE(8, 8),
        HID_RI_LOGICAL_MINIMUM(16, 0),
        HID_RI_LOGICAL_MAXIMUM(16, BRIGHTNESS_MAX),
        HID_RI_USAGE_PAGE(8, 0x0A), // Ordinals
        
        // Locals
        0x89, STRING_ID_LED_Indiv, //HID_RI_STRING_MINIMUM(8, STRING_ID_LED_Indiv),
        0x99, STRING_ID_LED_Indiv + LED_TOTAL_COUNT, //HID_RI_STRING_MAXIMUM(8, STRING_ID_LED_Indiv + LED_COUNT),
        HID_RI_USAGE_MINIMUM(8, 1), // LED 1
        HID_RI_USAGE_MAXIMUM(8, LED_TOTAL_COUNT), // LED 8 + buttons
        HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        
        // at least 1 INPUT is also needed to be recognised by Bemanitools
        HID_RI_USAGE_MINIMUM(8, 1),
        HID_RI_USAGE_MAXIMUM(8, 1),
        HID_RI_INPUT(8, HID_IOF_CONSTANT),
    HID_RI_END_COLLECTION(0),
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM InputsReport[] =
{
	/* Mouse Report
     *   Min/max X/Y -128 to 127
     *   NOTE: need at least 1 button or report does not work
	 *   Relative coordinates
	 */
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x02), /* Mouse */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, HID_REPORTID_MouseReport),
        HID_RI_USAGE(8, 0x01), /* Pointer */
        HID_RI_COLLECTION(8, 0x00), /* Physical */
            HID_RI_USAGE_PAGE(8, 0x09), /* Button */
            HID_RI_USAGE_MINIMUM(8, 0x01),
            HID_RI_USAGE_MAXIMUM(8, 1),
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_REPORT_COUNT(8, 1),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 7), /* 7 bits padding for our 1 button */
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
            HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
            HID_RI_USAGE(8, 0x30), /* Usage X */
            HID_RI_USAGE(8, 0x31), /* Usage Y */
            HID_RI_LOGICAL_MINIMUM(16, -128),
            HID_RI_LOGICAL_MAXIMUM(16, 127),
            HID_RI_PHYSICAL_MINIMUM(16, -128),
            HID_RI_PHYSICAL_MAXIMUM(16, 127),
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_REPORT_SIZE(8, 8),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),
        HID_RI_END_COLLECTION(0),
    HID_RI_END_COLLECTION(0),
    
	/* Keyboard Report */
	HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
	HID_RI_USAGE(8, 0x06), /* Keyboard */
	HID_RI_COLLECTION(8, 0x01), /* Application */
		HID_RI_REPORT_ID(8, HID_REPORTID_KeyboardReport),
		HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
		HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
		HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
		HID_RI_LOGICAL_MINIMUM(8, 0x00),
		HID_RI_LOGICAL_MAXIMUM(8, 0x01),
		HID_RI_REPORT_SIZE(8, 0x01),
		HID_RI_REPORT_COUNT(8, 0x08),
		HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
		HID_RI_REPORT_COUNT(8, 0x01),
		HID_RI_REPORT_SIZE(8, 0x08),
		HID_RI_INPUT(8, HID_IOF_CONSTANT),
		HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
		HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
		HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
		HID_RI_REPORT_COUNT(8, 0x05),
		HID_RI_REPORT_SIZE(8, 0x01),
		HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
		HID_RI_REPORT_COUNT(8, 0x01),
		HID_RI_REPORT_SIZE(8, 0x03),
		HID_RI_OUTPUT(8, HID_IOF_CONSTANT),
		HID_RI_LOGICAL_MINIMUM(8, 0x00),
		HID_RI_LOGICAL_MAXIMUM(8, 0xFF),
		HID_RI_USAGE_PAGE(8, 0x07), /* Keyboard */
		HID_RI_USAGE_MINIMUM(8, 0x00), /* Reserved (no event indicated) */
		HID_RI_USAGE_MAXIMUM(8, 0xFF),
		HID_RI_REPORT_COUNT(8, SWITCH_COUNT),
		HID_RI_REPORT_SIZE(8, 0x08),
		HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
	HID_RI_END_COLLECTION(0),
    
    /* Joystick report adjusted from HID_DESCRIPTOR_JOYSTICK */
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x05), /* Game pad */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, HID_REPORTID_JoystickReport),
        HID_RI_USAGE(8, 0x01), /* Pointer */
        HID_RI_COLLECTION(8, 0x00), /* Physical */
            HID_RI_USAGE(8, 0x30), /* X axis */
            HID_RI_USAGE(8, 0x31), /* Y axis */
            HID_RI_LOGICAL_MINIMUM(16, 0),
            HID_RI_LOGICAL_MAXIMUM(16, JOYSTICK_PPR-1),
            HID_RI_PHYSICAL_MINIMUM(16, 0),
            HID_RI_PHYSICAL_MAXIMUM(16, 1),
            HID_RI_REPORT_COUNT(8, 2), /* 2 axis */
            HID_RI_REPORT_SIZE(8, 8),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
        HID_RI_END_COLLECTION(0),
        HID_RI_USAGE_PAGE(8, 0x09), /* Button */
        HID_RI_USAGE_MINIMUM(8, 0x01), /* Button 1 */
        HID_RI_USAGE_MAXIMUM(8, SWITCH_COUNT), /* Button x */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_REPORT_COUNT(8, SWITCH_COUNT),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
        HID_RI_REPORT_SIZE(8, (SWITCH_COUNT % 8) ? (8 - (SWITCH_COUNT % 8)) : 0),
        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_INPUT(8, HID_IOF_CONSTANT),
    HID_RI_END_COLLECTION(0)
};


const uint8_t PROGMEM MS_OS_Descriptor[] =
{
    MS_OS_DESCRIPTOR_SET
    (
        MS_OS_CONFIG_SUBSET_HEADER
        (
            0x00, // Config 0
            MS_OS_FUNCTION_SUBSET_HEADER
            (
                INTERFACE_ID_Config, // Interface ID
                MS_OS_COMPAT_ID_WINUSB,
                
                
                /* I'm not entirely convinced this is actually required,
                   but USB on Windows is finnicky at best, and apparently
                   this is a good idea for composite devices. It's a mystery. */
                0x84, 0x00,   //wLength: 
                MS_OS_20_FEATURE_REG_PROPERTY, 0x00,   // wDescriptorType: MS_OS_20_FEATURE_REG_PROPERTY: 0x04 (Table 9)
                0x07, 0x00,   //wPropertyDataType: REG_MULTI_SZ (Table 15)
                0x2a, 0x00,   //wPropertyNameLength: 
                //bPropertyName: "DeviceInterfaceGUID"
                'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 
                'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 
                0x00, 0x00,
                0x50, 0x00,   // wPropertyDataLength
                //bPropertyData: "{975F44D9-0D08-43FD-8B3E-127CA8AFEF9D}".
                '{', 0x00, '9', 0x00, '7', 0x00, '5', 0x00, 'F', 0x00, '4', 0x00, '4', 0x00, 'D', 0x00, '9', 0x00, '-', 0x00, 
                '0', 0x00, 'D', 0x00, '0', 0x00, '8', 0x00, '-', 0x00, '4', 0x00, '3', 0x00, 'F', 0x00, 'D', 0x00, '-', 0x00, 
                '8', 0x00, 'B', 0x00, '3', 0x00, 'E', 0x00, '-', 0x00, '1', 0x00, '2', 0x00, '7', 0x00, 'C', 0x00, 'A', 0x00, 
                '8', 0x00, 'A', 0x00, 'F', 0x00, 'E', 0x00, 'F', 0x00, '9', 0x00, 'D', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
            )
        )
    )
};

const uint8_t PROGMEM BOSDescriptor[] =
{
    BOS_DESCRIPTOR
    (
        2, // 2 capability descriptors in use
        WEBUSB_CAPABILITY_DESCRIPTOR(WEBUSB_ID, URL_ID_Config), // Vendor request ID, URL ID
        // Required to force WinUSB driver for driverless WebUSB compatibility
        MS_OS_20_CAPABILITY_DESCRIPTOR(MS_OS_ID, sizeof(MS_OS_Descriptor)) // Vendor request ID, Descriptor set length
    )
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor_Standard =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(2,1,0),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
    // mon.im VID/PID pair, unique!
	.VendorID               = 0x16D0,
	.ProductID              = 0x0A6D,
    // To provide uniqueness
	.ReleaseNumber          = VERSION_BCD(0,0,3),

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = USE_INTERNAL_SERIAL,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor_EAC =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(2,1,0),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
    // to pretend we are someone else
	.VendorID               = 0x1CCF,
	.ProductID              = 0x1014,
    // To provide uniqueness
	.ReleaseNumber          = VERSION_BCD(0,0,3),

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = USE_INTERNAL_SERIAL,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 3,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = STRING_ID_Product,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(500)
		},
        
	.Config_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_Config,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 2,

			.Class                  = 0xFF,
			.SubClass               = 0xFF,
			.Protocol               = 0xFF,

			.InterfaceStrIndex      = STRING_ID_Config
		},

	.Config_DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CONFIG_OUT_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CONFIG_EPSIZE,
            // ignored but required
			.PollingIntervalMS      = 0x05
		},
        
	.Config_DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CONFIG_IN_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CONFIG_EPSIZE,
            // ignored but required
			.PollingIntervalMS      = 0x05
		},
        
	.HID1_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_Inputs,
			.AlternateSetting       = 0x00,

			.TotalEndpoints         = 1,

			.Class                  = HID_CSCP_HIDClass,
			.SubClass               = HID_CSCP_NonBootSubclass,
			.Protocol               = HID_CSCP_NonBootProtocol,

			.InterfaceStrIndex      = STRING_ID_Controls
		},

	.HID1_InputsHID =
		{
			.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(InputsReport)
		},

	.HID1_ReportINEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = INPUTS_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = INPUTS_EPSIZE,
			.PollingIntervalMS      = 0x01
		},
        
	.HID2_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_LED,
			.AlternateSetting       = 0x00,

			.TotalEndpoints         = 1,

			.Class                  = HID_CSCP_HIDClass,
			.SubClass               = HID_CSCP_NonBootSubclass,
			.Protocol               = HID_CSCP_NonBootProtocol,

			.InterfaceStrIndex      = STRING_ID_LED
		},

	.HID2_LEDHID =
		{
			.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(LEDReport)
		},

	.HID2_ReportINEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = LED_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = LED_EPSIZE,
			.PollingIntervalMS      = 255
		},
};


const USB_Descriptor_URL_t PROGMEM ConfigURL = URL_STRING_DESCRIPTOR(URL_HTTPS, "mon.im/sdvx/config");
const USB_Descriptor_URL_t PROGMEM LocalhostURL = URL_STRING_DESCRIPTOR(URL_HTTP, "localhost");

const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"mon.im");
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"Pocket Voltex");
const USB_Descriptor_String_t PROGMEM LEDString = USB_STRING_DESCRIPTOR(L"Pocket Voltex LEDs");
const USB_Descriptor_String_t PROGMEM ControlsString = USB_STRING_DESCRIPTOR(L"Pocket Voltex Controls");
const USB_Descriptor_String_t PROGMEM ConfigString = USB_STRING_DESCRIPTOR(L"Pocket Voltex Config");

const USB_Descriptor_String_t PROGMEM
    LEDString_01 = USB_STRING_DESCRIPTOR(L"Top Left R"),
    LEDString_02 = USB_STRING_DESCRIPTOR(L"Top Left G"),
    LEDString_03 = USB_STRING_DESCRIPTOR(L"Top Left B"),
    LEDString_04 = USB_STRING_DESCRIPTOR(L"Top Right R"),
    LEDString_05 = USB_STRING_DESCRIPTOR(L"Top Right G"),
    LEDString_06 = USB_STRING_DESCRIPTOR(L"Top Right B"),
    LEDString_07 = USB_STRING_DESCRIPTOR(L"Upper Left R"),
    LEDString_08 = USB_STRING_DESCRIPTOR(L"Upper Left G"),
    LEDString_09 = USB_STRING_DESCRIPTOR(L"Upper Left B"),
    LEDString_10 = USB_STRING_DESCRIPTOR(L"Upper Right R"),
    LEDString_11 = USB_STRING_DESCRIPTOR(L"Upper Right G"),
    LEDString_12 = USB_STRING_DESCRIPTOR(L"Upper Right B"),
    LEDString_13 = USB_STRING_DESCRIPTOR(L"Lower Left R"),
    LEDString_14 = USB_STRING_DESCRIPTOR(L"Lower Left G"),
    LEDString_15 = USB_STRING_DESCRIPTOR(L"Lower Left B"),
    LEDString_16 = USB_STRING_DESCRIPTOR(L"Lower Right R"),
    LEDString_17 = USB_STRING_DESCRIPTOR(L"Lower Right G"),
    LEDString_18 = USB_STRING_DESCRIPTOR(L"Lower Right B"),
    LEDString_19 = USB_STRING_DESCRIPTOR(L"Bottom Left R"),
    LEDString_20 = USB_STRING_DESCRIPTOR(L"Bottom Left G"),
    LEDString_21 = USB_STRING_DESCRIPTOR(L"Bottom Left B"),
    LEDString_22 = USB_STRING_DESCRIPTOR(L"Bottom Right R"),
    LEDString_23 = USB_STRING_DESCRIPTOR(L"Bottom Right G"),
    LEDString_24 = USB_STRING_DESCRIPTOR(L"Bottom Right B"),
    LEDString_25 = USB_STRING_DESCRIPTOR(L"BT-A"),
    LEDString_26 = USB_STRING_DESCRIPTOR(L"BT-B"),
    LEDString_27 = USB_STRING_DESCRIPTOR(L"BT-C"),
    LEDString_28 = USB_STRING_DESCRIPTOR(L"BT-D"),
    LEDString_29 = USB_STRING_DESCRIPTOR(L"FX-L"),
    LEDString_30 = USB_STRING_DESCRIPTOR(L"FX-R"),
    LEDString_31 = USB_STRING_DESCRIPTOR(L"START");
const USB_Descriptor_String_t *LEDString_indiv[] = {
    &LEDString_01,
    &LEDString_02,
    &LEDString_03,
    &LEDString_04,
    &LEDString_05,
    &LEDString_06,
    &LEDString_07,
    &LEDString_08,
    &LEDString_09,
    &LEDString_10,
    &LEDString_11,
    &LEDString_12,
    &LEDString_13,
    &LEDString_14,
    &LEDString_15,
    &LEDString_16,
    &LEDString_17,
    &LEDString_18,
    &LEDString_19,
    &LEDString_20,
    &LEDString_21,
    &LEDString_22,
    &LEDString_23,
    &LEDString_24,
    &LEDString_25,
    &LEDString_26,
    &LEDString_27,
    &LEDString_28,
    &LEDString_29,
    &LEDString_30,
    &LEDString_31,
};

void USB_Process_BOS(void) {
    const void* Address = NULL;
	uint16_t    Size    = 0;
    
    if(!(Endpoint_IsSETUPReceived()) ||
        USB_ControlRequest.bmRequestType != (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE)) {
        return;
    }
    switch(USB_ControlRequest.bRequest) {
        case WEBUSB_ID:
            if(USB_ControlRequest.wIndex == WEBUSB_REQUEST_GET_URL &&
                    USB_ControlRequest.wValue == URL_ID_Config) {
                Address = &ConfigURL;
                Size = pgm_read_byte(&ConfigURL.Header.Size);
            }
            break;
        case MS_OS_ID:
            if(USB_ControlRequest.wIndex == MS_OS_20_DESCRIPTOR_INDEX) {
                Address = &MS_OS_Descriptor;
                Size    = sizeof(MS_OS_Descriptor);
            }
            break;
        default:
            return;
    }
    if(Address != NULL) {
        Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

        Endpoint_ClearSETUP();

        Endpoint_Write_Control_PStream_LE(Address, Size);
        Endpoint_ClearOUT();
    }
}

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
            if(sdvxConfig.controlMode == CONTROL_EAC) {
                Address = &DeviceDescriptor_EAC;
            } else {
                Address = &DeviceDescriptor_Standard;
            }
            Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
        case DTYPE_BOS:
            Address = &BOSDescriptor;
			Size    = sizeof(BOSDescriptor);
			break;
		case DTYPE_String:
			switch (DescriptorNumber)
			{
				case STRING_ID_Language:
					Address = &LanguageString;
					Size    = pgm_read_byte(&LanguageString.Header.Size);
					break;
				case STRING_ID_Manufacturer:
					Address = &ManufacturerString;
					Size    = pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case STRING_ID_Product:
					Address = &ProductString;
					Size    = pgm_read_byte(&ProductString.Header.Size);
					break;
                case STRING_ID_LED:
                    Address = &LEDString;
					Size    = pgm_read_byte(&LEDString.Header.Size);
					break;
                case STRING_ID_Controls:
                    Address = &ControlsString;
					Size    = pgm_read_byte(&ControlsString.Header.Size);
					break;
                case STRING_ID_Config:
                    Address = &ConfigString;
					Size    = pgm_read_byte(&ConfigString.Header.Size);
					break;
                default:
                    if(DescriptorNumber >= STRING_ID_LED_Indiv && DescriptorNumber < (STRING_ID_LED_Indiv + LED_TOTAL_COUNT)) {
                        Address = LEDString_indiv[DescriptorNumber - STRING_ID_LED_Indiv];
                        Size    = pgm_read_byte(&(LEDString_indiv[DescriptorNumber - STRING_ID_LED_Indiv]->Header.Size));
                    }
			}
			break;
		case HID_DTYPE_HID:
            Size = sizeof(USB_HID_Descriptor_HID_t);
			switch (wIndex) {
				case INTERFACE_ID_Inputs:
					Address = &ConfigurationDescriptor.HID1_InputsHID;
					break;
				case INTERFACE_ID_LED:
					Address = &ConfigurationDescriptor.HID2_LEDHID;
					break;
			}
			break;
		case HID_DTYPE_Report:
			switch (wIndex) {
				case INTERFACE_ID_Inputs:
					Address = &InputsReport;
					Size    = sizeof(InputsReport);
					break;
                case INTERFACE_ID_LED:
					Address = &LEDReport;
					Size    = sizeof(LEDReport);
					break;
			}
			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

