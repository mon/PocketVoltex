#include "Descriptors.h"

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
        HID_RI_LOGICAL_MINIMUM(8, 0),
        HID_RI_LOGICAL_MAXIMUM(8, BRIGHTNESS_LEVELS-1),
        HID_RI_USAGE_PAGE(8, 0x0A), // Ordinals
        
        // Locals
        0x79, STRING_ID_LED_Indiv, //HID_RI_STRING_MINIMUM(8, STRING_ID_LED_Indiv),
        0x89, STRING_ID_LED_Indiv + LED_TOTAL_COUNT, //HID_RI_STRING_MAXIMUM(8, STRING_ID_LED_Indiv + LED_COUNT),
        HID_RI_USAGE_MINIMUM(8, 1), // LED 1
        HID_RI_USAGE_MAXIMUM(8, LED_TOTAL_COUNT), // LED 8 + buttons
        HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        
        // at least 1 INPUT is also needed to be recognised by Bemanitools
        HID_RI_USAGE_MINIMUM(8, 1),
        HID_RI_USAGE_MAXIMUM(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        //0x79, STRING_ID_LED_Indiv, //HID_RI_STRING_MINIMUM(8, STRING_ID_LED_Indiv),
        //0x89, STRING_ID_LED_Indiv + LED_TOTAL_COUNT, //HID_RI_STRING_MAXIMUM(8, STRING_ID_LED_Indiv + LED_COUNT),
        //HID_RI_USAGE_MINIMUM(8, 1), // LED 1
        //HID_RI_USAGE_MAXIMUM(8, LED_TOTAL_COUNT), // LED 8 + buttons
        //HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
    HID_RI_END_COLLECTION(0),
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM KeyboardReport[] =
{
	// Use the HID class driver's standard Keyboard report
	HID_DESCRIPTOR_KEYBOARD(SWITCH_COUNT)
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM MouseReport[] =
{
	/* Use the HID class driver's standard Mouse report.
     *   Min/max X/Y -128 to 127
     *   NOTE: need at least 1 button or report does not work
	 *   Relative coordinates
	 */
	HID_DESCRIPTOR_MOUSE(-128, 127, -128, 127, 1, false)
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
                MS_OS_COMPAT_ID_WINUSB
            )
        )
    )
};

const uint8_t PROGMEM WebUSBAllowedOrigins[] = {
    WEBUSB_ALLOWED_ORIGINS_HEADER
    (
        1, // 1 config header present
        WEBUSB_CONFIG_SUBSET_HEADER
        (
            0x00, 1, // Config 0, 1 function header
            // Config interface accessible from the web, 2 valid URLs
            WEBUSB_FUNCTION_SUBSET_HEADER(INTERFACE_ID_Config, URL_ID_Config, URL_ID_Localhost)
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

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
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
	.ReleaseNumber          = VERSION_BCD(0,0,1),

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = STRING_ID_Product,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 4,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = STRING_ID_Product,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(500)
		},

	.HID1_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_Keyboard,
			.AlternateSetting       = 0x00,

			.TotalEndpoints         = 1,

			.Class                  = HID_CSCP_HIDClass,
			.SubClass               = HID_CSCP_NonBootSubclass,
			.Protocol               = HID_CSCP_NonBootProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.HID1_KeyboardHID =
		{
			.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(KeyboardReport)
		},

	.HID1_ReportINEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = KEYBOARD_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = KEYBOARD_EPSIZE,
			.PollingIntervalMS      = 0x01
		},
        
    .HID2_MouseInterface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_Mouse,
			.AlternateSetting       = 0x00,

			.TotalEndpoints         = 1,

			.Class                  = HID_CSCP_HIDClass,
			.SubClass               = HID_CSCP_BootSubclass,
			.Protocol               = HID_CSCP_MouseBootProtocol,

			.InterfaceStrIndex      = STRING_ID_Knobs
		},

	.HID2_MouseHID =
		{
			.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(MouseReport)
		},

	.HID2_ReportINEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = MOUSE_IN_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = MOUSE_EPSIZE,
			.PollingIntervalMS      = 0x01
		},
        
	.HID3_Interface =
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

	.HID3_LEDHID =
		{
			.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(LEDReport)
		},

	.HID3_ReportINEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = LED_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = LED_EPSIZE,
			.PollingIntervalMS      = 255
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

	.Config_DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CONFIG_IN_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CONFIG_EPSIZE,
            // ignored but required
			.PollingIntervalMS      = 0x05
		},

	.Config_DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CONFIG_OUT_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CONFIG_EPSIZE,
            // ignored but required
			.PollingIntervalMS      = 0x05
		}
};


const USB_Descriptor_URL_t PROGMEM ConfigURL = URL_STRING_DESCRIPTOR(URL_HTTPS, "mon.im/sdvx/config");
const USB_Descriptor_URL_t PROGMEM LocalhostURL = URL_STRING_DESCRIPTOR(URL_HTTP, "localhost");

const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"mon.im");
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"Pocket Voltex");
const USB_Descriptor_String_t PROGMEM LEDString = USB_STRING_DESCRIPTOR(L"Pocket Voltex LEDs");
const USB_Descriptor_String_t PROGMEM KnobString = USB_STRING_DESCRIPTOR(L"Pocket Voltex Knobs");
const USB_Descriptor_String_t PROGMEM ConfigString = USB_STRING_DESCRIPTOR(L"Pocket Voltex Config");
// There may be a better way to do this
const USB_Descriptor_String_t PROGMEM LEDString_indiv[] = {
    USB_STRING_DESCRIPTOR(L"L1-B"),
    USB_STRING_DESCRIPTOR(L"L1-G"),
    USB_STRING_DESCRIPTOR(L"L1-R"),
    USB_STRING_DESCRIPTOR(L"L2-B"),
    USB_STRING_DESCRIPTOR(L"L2-G"),
    USB_STRING_DESCRIPTOR(L"L2-R"),
    USB_STRING_DESCRIPTOR(L"L3-B"),
    USB_STRING_DESCRIPTOR(L"L3-G"),
    USB_STRING_DESCRIPTOR(L"L3-R"),
    USB_STRING_DESCRIPTOR(L"L4-B"),
    USB_STRING_DESCRIPTOR(L"L4-G"),
    USB_STRING_DESCRIPTOR(L"L4-R"),
    USB_STRING_DESCRIPTOR(L"L5-B"),
    USB_STRING_DESCRIPTOR(L"L5-G"),
    USB_STRING_DESCRIPTOR(L"L5-R"),
    USB_STRING_DESCRIPTOR(L"L6-B"),
    USB_STRING_DESCRIPTOR(L"L6-G"),
    USB_STRING_DESCRIPTOR(L"L6-R"),
    USB_STRING_DESCRIPTOR(L"L7-B"),
    USB_STRING_DESCRIPTOR(L"L7-G"),
    USB_STRING_DESCRIPTOR(L"L7-R"),
    USB_STRING_DESCRIPTOR(L"L8-B"),
    USB_STRING_DESCRIPTOR(L"L8-G"),
    USB_STRING_DESCRIPTOR(L"L8-R"),
    USB_STRING_DESCRIPTOR(L"BT-A"),
    USB_STRING_DESCRIPTOR(L"BT-B"),
    USB_STRING_DESCRIPTOR(L"BT-C"),
    USB_STRING_DESCRIPTOR(L"BT-D"),
    USB_STRING_DESCRIPTOR(L"FX-L"),
    USB_STRING_DESCRIPTOR(L"FX-R"),
};

void USB_Process_BOS(void) {
    const void* Address = NULL;
	uint16_t    Size    = 0;
    
    if(!(Endpoint_IsSETUPReceived()) ||
        USB_ControlRequest.bmRequestType != (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE)) {
        return;
    }
    led_set_all(16,16,16);
    switch(USB_ControlRequest.bRequest) {
        case WEBUSB_ID:
            switch(USB_ControlRequest.wIndex) {
                case WEBUSB_REQUEST_GET_ALLOWED_ORIGINS:
                    Address = &WebUSBAllowedOrigins;
                    Size = sizeof(WebUSBAllowedOrigins);
                    break;
                case WEBUSB_REQUEST_GET_URL:
                    switch(USB_ControlRequest.wValue) {
                        case URL_ID_Localhost:
                            Address = &LocalhostURL;
                            Size = pgm_read_byte(&LocalhostURL.Header.Size);
                            break;
                        case URL_ID_Config:
                            Address = &ConfigURL;
                            Size = pgm_read_byte(&ConfigURL.Header.Size);
                            break;
                    }
                    break;
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
			Address = &DeviceDescriptor;
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
                case STRING_ID_Knobs:
                    Address = &KnobString;
					Size    = pgm_read_byte(&KnobString.Header.Size);
					break;
                case STRING_ID_Config:
                    Address = &ConfigString;
					Size    = pgm_read_byte(&ConfigString.Header.Size);
					break;
                default:
                    if(DescriptorNumber >= STRING_ID_LED_Indiv) {
                        Address = &(LEDString_indiv[STRING_ID_LED_Indiv - DescriptorNumber]);
                        Size    = pgm_read_byte(&LEDString_indiv[STRING_ID_LED_Indiv - DescriptorNumber].Header.Size);
                    }
			}
			break;
		case HID_DTYPE_HID:
            Size = sizeof(USB_HID_Descriptor_HID_t);
			switch (wIndex) {
				case INTERFACE_ID_Keyboard:
					Address = &ConfigurationDescriptor.HID1_KeyboardHID;
					break;
                case INTERFACE_ID_Mouse:
					Address = &ConfigurationDescriptor.HID2_MouseHID;
					break;
				case INTERFACE_ID_LED:
					Address = &ConfigurationDescriptor.HID3_LEDHID;
					break;
			}
			break;
		case HID_DTYPE_Report:
			switch (wIndex) {
				case INTERFACE_ID_Keyboard:
					Address = &KeyboardReport;
					Size    = sizeof(KeyboardReport);
					break;
                case INTERFACE_ID_Mouse:
					Address = &MouseReport;
					Size    = sizeof(MouseReport);
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

