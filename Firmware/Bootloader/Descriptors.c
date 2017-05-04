#include "Descriptors.h"

const USB_Descriptor_URL_t ConfigURL = URL_STRING_DESCRIPTOR(URL_HTTPS, "mon.im");
const USB_Descriptor_String_t ProductString = USB_STRING_DESCRIPTOR(L"Voltex Boot");
const USB_Descriptor_String_t LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

const uint8_t MS_OS_Descriptor[] =
{
    MS_OS_DESCRIPTOR_SET
    (
        MS_OS_COMPAT_ID_WINUSB
    )
};

const uint8_t BOSDescriptor[] =
{
    BOS_DESCRIPTOR
    (
        2, // 2 capability descriptors in use
        WEBUSB_CAPABILITY_DESCRIPTOR(WEBUSB_ID, URL_ID_Config), // Vendor request ID, URL ID
        // Required to force WinUSB driver for driverless WebUSB compatibility
        MS_OS_20_CAPABILITY_DESCRIPTOR(MS_OS_ID, sizeof(MS_OS_Descriptor)) // Vendor request ID, Descriptor set length
    )
};

const USB_Descriptor_Device_t DeviceDescriptor =
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
	.ReleaseNumber          = VERSION_BCD(0,0,2),

	.ManufacturerStrIndex   = NO_DESCRIPTOR,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Configuration_t ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 1,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},
        
	.Config_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_Config,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 0,

			.Class                  = 0xFF,
			.SubClass               = 0xFF,
			.Protocol               = 0xFF,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},
};

void USB_Process_BOS(void) {
    const void* Address = NULL;
	uint16_t    Size    = 0;
    
    /* Caller does this check */
    //if(!(Endpoint_IsSETUPReceived())) {
    //    return;
    //}
    if(USB_ControlRequest.bmRequestType != (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE)) {
        return;
    }
    
    switch(USB_ControlRequest.bRequest) {
        case WEBUSB_ID:
            if(USB_ControlRequest.wIndex == WEBUSB_REQUEST_GET_URL &&
                    USB_ControlRequest.wValue == URL_ID_Config) {
                Address = &ConfigURL;
                Size = ConfigURL.Header.Size;
            }
            break;
        case MS_OS_ID:
            if(USB_ControlRequest.wIndex == MS_OS_20_DESCRIPTOR_INDEX) {
                Address = &MS_OS_Descriptor;
                Size    = sizeof(MS_OS_Descriptor);
            }
            break;
    }
    if(Address != NULL) {
        Endpoint_ClearSETUP();
        Endpoint_Write_Control_Stream_LE(Address, Size);
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
	const uint8_t DescriptorType   = (wValue >> 8);
    const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	/* If/Else If chain compiles slightly smaller than a switch case */
	if (DescriptorType == DTYPE_Device)
	{
		Address = &DeviceDescriptor;
		Size    = sizeof(USB_Descriptor_Device_t);
	}
	else if (DescriptorType == DTYPE_Configuration)
	{
		Address = &ConfigurationDescriptor;
		Size    = sizeof(USB_Descriptor_Configuration_t);
	}
    else if (DescriptorType == DTYPE_BOS) {
        Address = &BOSDescriptor;
        Size    = sizeof(BOSDescriptor);
    }
    else if (DescriptorType == DTYPE_String) {
        switch (DescriptorNumber)
        {
            case STRING_ID_Language:
                Address = &LanguageString;
                Size    = LanguageString.Header.Size;
                break;
            case STRING_ID_Product:
                Address = &ProductString;
                Size    = ProductString.Header.Size;
                break;
        }
    }

	*DescriptorAddress = Address;
	return Size;
}

