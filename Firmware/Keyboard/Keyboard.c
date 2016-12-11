#include "Keyboard.h"
#include "Config.h"
#include "Encoder.h"
#include "LED.h"
#include "LEDPatterns.h"

#define READ_SWITCH(x) (!(*pins[switches[x].switchPort] & _BV(switches[x].switchPin)))
#define MAGIC_BOOT_KEY            0xDEADBE7A
// offset * word size
#define BOOTLOADER_START_ADDRESS  (0x1c00 * 2)

// How long to wait before moving to internal lighting
#define HID_LED_TIMEOUT 5000

typedef struct
{
    uint8_t Modifier; // Keyboard modifier byte indicating pressed modifier keys (\c HID_KEYBOARD_MODIFER_* masks)
    uint8_t Reserved; // Reserved for OEM use, always set to 0.
    uint8_t KeyCode[SWITCH_COUNT]; // Length determined by the number of keys that can be reported
} Keyboard_Report_t;

typedef struct
{
    uint8_t mainLights[LED_RAW_COUNT];
    uint8_t btFx[6];
} LED_Report_t;

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(Keyboard_Report_t)];
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];
static uint8_t PrevGenericHIDReportBuffer[sizeof(sdvx_config_t)];
static uint8_t PrevLEDHIDReportBuffer[sizeof(LED_Report_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = INTERFACE_ID_Keyboard,
            .ReportINEndpoint             =
                {
                    .Address              = KEYBOARD_EPADDR,
                    .Size                 = KEYBOARD_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
        },
};

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the mouse HID
 *  interface within the device.
 */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = INTERFACE_ID_Mouse,
            .ReportINEndpoint             =
                {
                    .Address              = MOUSE_IN_EPADDR,
                    .Size                 = MOUSE_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = PrevMouseHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevMouseHIDReportBuffer),
        },
};

USB_ClassInfo_HID_Device_t Generic_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = INTERFACE_ID_Generic,
            .ReportINEndpoint             =
                {
                    .Address              = GENERIC_EPADDR,
                    .Size                 = GENERIC_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = PrevGenericHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevGenericHIDReportBuffer),
        },
};

USB_ClassInfo_HID_Device_t LED_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = INTERFACE_ID_LED,
            .ReportINEndpoint             =
                {
                    .Address              = LED_EPADDR,
                    .Size                 = LED_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = PrevLEDHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevLEDHIDReportBuffer),
        },
};

// NOTE: atemga16u2 does not have a PORTA
typedef enum {
    B = 0,
    C,
    D
} port_t;

static volatile uint8_t *ports[] = {&PORTB, &PORTC, &PORTD};
static volatile uint8_t *pins[] = {&PINB, &PINC, &PIND};
static volatile uint8_t *ddrs[] = {&DDRB, &DDRC, &DDRD};

typedef struct {
    port_t switchPort;
    uint8_t switchPin;
    uint8_t state;
    uint8_t lastReport;
    uint8_t debounce;
} switch_t;

static switch_t switches[SWITCH_COUNT] = {
    {B, 1}, // A
    {D, 7}, // B
    {D, 5}, // C
    {D, 4}, // D
    {B, 0}, // FX L
    {D, 6}, // FX R
    {C, 2} // START
};
static uint8_t switchesChanged = 1;

// Set to max already so we have our init flash
static uint16_t hidTimeout = HID_LED_TIMEOUT;

uint32_t Boot_Key ATTR_NO_INIT;

void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);
void Bootloader_Jump_Check(void)
{
    // If the reset source was the bootloader and the key is correct, clear it and jump to the bootloader
    if ((MCUSR & (1 << WDRF)) && (Boot_Key == MAGIC_BOOT_KEY))
    {
        Boot_Key = 0;
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void RebootToBootloader(void) {
    // With this uncommented, reboot fails. Odd.
    //USB_Disable();
    cli();

    // Back to the bootloader
    Boot_Key = MAGIC_BOOT_KEY;
    wdt_enable(WDTO_250MS);
    while(1);
}

void update_switches(void) {
    uint8_t i, newState;
    
    for(i = 0; i < SWITCH_COUNT; i++) {
        newState = READ_SWITCH(i);
        if(!switches[i].debounce && newState != switches[i].lastReport) {
            switches[i].state = newState;
            switches[i].debounce = sdvxConfig.debounce;
            switchesChanged = 1;
        }
    }
}

int main(void)
{
    GlobalInterruptDisable();
    
    InitConfig();
    
	SetupHardware();

	GlobalInterruptEnable();
    
    // Blink to show we're not in bootloader
    led_anim_flash();

	for (;;)
	{
		HID_Device_USBTask(&Keyboard_HID_Interface);
        HID_Device_USBTask(&Mouse_HID_Interface);
		HID_Device_USBTask(&Generic_HID_Interface);
		HID_Device_USBTask(&LED_HID_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals */
void SetupHardware()
{
    uint8_t i;
    
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
    
    for(i = 0; i < SWITCH_COUNT; i++) {
        switches[i].state = 0;
        switches[i].lastReport = 0;
        switches[i].debounce = 0;
        // setup switches to be inputs
        *ddrs[switches[i].switchPort] &= ~_BV(switches[i].switchPin);
        // with internal pullups
        *ports[switches[i].switchPort] |= _BV(switches[i].switchPin);
    }
    
    // FX_L held while plugging in
    if(READ_SWITCH(4)) {
        RebootToBootloader();
    }

	/* Hardware Initialization */
    encoder_init();
    led_init();
    
	USB_Init();
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
    if(ReportType != HID_REPORT_ITEM_In) {
        *ReportSize = 0;
        return false;
    }
    if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
        Keyboard_Report_t* KeyboardReport = (Keyboard_Report_t*)ReportData;
        update_switches();
        
        if(!switchesChanged) {
            *ReportSize = 0;
            return false;
        }
         
        for(uint8_t i = 0; i < SWITCH_COUNT; i++) {
            KeyboardReport->KeyCode[i] = switches[i].state ? sdvxConfig.switches[i] : 0;
            switches[i].lastReport = switches[i].state;
            // Update blinkenlights
            if(switches[i].state) {
                if(sdvxConfig.ledsOn) {
                    // TODO
                }
            }
        }
         
        *ReportSize = sizeof(Keyboard_Report_t);
         
        switchesChanged = 0;
        return true;
    } else if(HIDInterfaceInfo == &Mouse_HID_Interface) {
        USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
        
        MouseReport->X = encoder_get(0);
        MouseReport->Y = -encoder_get(1);
        
        led_knobs_update(MouseReport->X, MouseReport->Y);
        
        encoder_set(0, 0);
        encoder_set(1, 0);

		*ReportSize = sizeof(USB_MouseReport_Data_t);
		return true;
    } else if(HIDInterfaceInfo == &Generic_HID_Interface) {
        uint8_t* ConfigReport = (uint8_t*)ReportData;
        memcpy(ConfigReport, &sdvxConfig, sizeof(sdvx_config_t));
        *ReportSize = CONFIG_BYTES;
        return true;
    }/* else if(HIDInterfaceInfo == &LED_HID_Interface) {
        uint8_t* LEDReport = (uint8_t*)ReportData;
        memcpy(LEDReport, (uint8_t*)leds, LED_TOTAL_COUNT);
        *ReportSize = LED_TOTAL_COUNT;
        return true;
    }*/
    *ReportSize = 0;
    return false;

}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize) {
    if(HIDInterfaceInfo == &Generic_HID_Interface && ReportType == HID_REPORT_ITEM_Out) {
        uint8_t* ConfigReport = (uint8_t*)ReportData;
        // So we can upgrade firmware without having to hit the button
        if(ConfigReport[CONFIG_BYTES-1] == MAGIC_RESET_NUMBER) {
            RebootToBootloader();
        }
        SetConfig(ConfigReport);
    } else if(HIDInterfaceInfo == &LED_HID_Interface && ReportType == HID_REPORT_ITEM_Out) {
        // reset our timeout
        hidTimeout = 0;
        
        LED_Report_t* LEDReport = (LED_Report_t*)ReportData;
        memcpy((uint8_t*)leds, LEDReport->mainLights, LED_RAW_COUNT);
        
        // Keep normal lights but override when we get flashes on BT or FX
        // BT LEDs flash pure white
        for(uint8_t i = 0; i < 4; i++) {
            if(LEDReport->btFx[i]) {
                led_set(ledMap[i], BRIGHTNESS_LEVELS, BRIGHTNESS_LEVELS, BRIGHTNESS_LEVELS);
            }
        }
        // FX LEDs flash orange
        for(uint8_t i = 4; i < 6; i++) {
            if(LEDReport->btFx[i]) {
                led_set(ledMap[i], BRIGHTNESS_LEVELS, BRIGHTNESS_LEVELS/4, 0);
            }
        }
    }
}


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
    HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);
	HID_Device_ConfigureEndpoints(&Generic_HID_Interface);
	HID_Device_ConfigureEndpoints(&LED_HID_Interface);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
    HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
	HID_Device_ProcessControlRequest(&LED_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
    HID_Device_MillisecondElapsed(&Mouse_HID_Interface);
    HID_Device_MillisecondElapsed(&Generic_HID_Interface);
    HID_Device_MillisecondElapsed(&LED_HID_Interface);
    
    if(hidTimeout > HID_LED_TIMEOUT) {
        led_frame();
    } else {
        hidTimeout++;
    }
    
    for(int i = 0; i < SWITCH_COUNT; i++) {
        if(switches[i].debounce) {
            switches[i].debounce--;
        }
    }
}
