#include "PocketVoltex.h"
#include "Config.h"
#include "Encoder.h"
#include "LED.h"
#include "LEDPatterns.h"
#include "Macro.h"
#include <util/delay.h>

#undef SK9822_BRIGHTNESS
#define SK9822_BRIGHTNESS 1

#define LOAD_SWITCH(source, sourceBit, result, resultBit) result |= !((source) & _BV(sourceBit)) << resultBit

// B 0,4,5
#define SWITCH_MASKB 0b00110001
// C 2
#define SWITCH_MASKC 0b00000100
// D 4,5,6,7
#define SWITCH_MASKD 0b11110000

// How long to wait before moving to internal lighting
#define HID_LED_TIMEOUT 2000

// If I add more buttons with the macro key I don't need to care
#if SWITCH_COUNT <= 8
    #define SWITCH_BITMASK_UINT uint8_t
#elif SWITCH_COUNT <= 16
    #define SWITCH_BITMASK_UINT uint16_t
#else
    #error TOO MANY SWITCHES
#endif
typedef struct
{
    int8_t  X; // VOL-L
    int8_t  Y; // VOL-R
    SWITCH_BITMASK_UINT Buttons; // bitmask
} Joystick_Report_t;

typedef struct
{
    uint8_t Modifier; // Keyboard modifier byte indicating pressed modifier keys (\c HID_KEYBOARD_MODIFER_* masks)
    uint8_t Reserved; // Reserved for OEM use, always set to 0.
    uint8_t KeyCode[SWITCH_COUNT]; // Length determined by the number of keys that can be reported
} Keyboard_Report_t;

typedef struct
{
    uint8_t mainLights[LED_PHYSICAL_COUNT];
    uint8_t btFx[6];
} LED_Report_t;

static uint8_t updateLEDs = 1;

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Inputs_HID_Interface =
{
    .Config =
        {
            .InterfaceNumber              = INTERFACE_ID_Inputs,
            .ReportINEndpoint             =
                {
                    .Address              = INPUTS_EPADDR,
                    .Size                 = INPUTS_EPSIZE,
                    .Banks                = 1,
                },
            .PrevReportINBuffer           = NULL,
            .PrevReportINBufferSize       = MAX(MAX(sizeof(Keyboard_Report_t), sizeof(Joystick_Report_t)), sizeof(USB_MouseReport_Data_t)),
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
            .PrevReportINBuffer           = NULL,
            .PrevReportINBufferSize       = sizeof(LED_Report_t),
        },
};


// Set to max already so we have our init flash

void RebootToBootloader(void) {
	/* Disconnect from the host - USB interface will be reset later along with the AVR */
	USB_Detach();

    // Back to the bootloader
    wdt_enable(WDTO_250MS);
    while(1);
}

int main(void)
{
    GlobalInterruptDisable();
    
    InitConfig();
    
    SetupHardware();

    GlobalInterruptEnable();
    
    while(1) {
        int8_t x = encoder_get(0);
        int8_t y = encoder_get(1);
        led_knobs_update(x, y);
        encoder_set(0, 0);
        encoder_set(1, 0);
        if(led_on_frame()) {
            updateLEDs = 1;
        }
        
        if(updateLEDs) {
            updateLEDs = 0;
            led_set_all(32,32,32);
            //led_pattern_animate();
            // knob lights go above all
            led_overlay_knobs();
            led_commit();
        }
        _delay_ms(1);
    }

    for (;;)
    {
        HID_Device_USBTask(&Inputs_HID_Interface);
        HID_Device_USBTask(&LED_HID_Interface);
        USB_USBTask();
        
        Endpoint_SelectEndpoint(CONFIG_OUT_EPADDR);
        if (Endpoint_IsOUTReceived()) {
            uint8_t ReceivedData[COMMAND_BYTES];
            Endpoint_Read_Stream_LE(ReceivedData, COMMAND_BYTES, NULL);
            Endpoint_ClearOUT();
            
            command_response_t respond = HandleConfig(ReceivedData);
            switch(respond) {
                // we are returning the requested data
                case RESPOND:
                    Endpoint_SelectEndpoint(CONFIG_IN_EPADDR);
                    Endpoint_Write_Stream_LE(ReceivedData, COMMAND_BYTES, NULL);
                    Endpoint_ClearIN();
                    break;
                case REBOOT:
                    RebootToBootloader();
                    break;
                // no data to return
                default:
                    break;
            }
        }
        
        if(updateLEDs) {
            updateLEDs = 0;
            led_set_all(32,32,32);
            //led_pattern_animate();
            // knob lights go above all
            led_overlay_knobs();
            led_commit();
        }
    }
}

/** Configures the board hardware and chip peripherals */
void SetupHardware()
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~_BV(WDRF);
    wdt_disable();
    
    // Inputs
    DDRB &= ~SWITCH_MASKB;
    DDRC &= ~SWITCH_MASKC;
    DDRD &= ~SWITCH_MASKD;
    // Pullups
    PORTB |= SWITCH_MASKB;
    PORTC |= SWITCH_MASKC;
    PORTD |= SWITCH_MASKD;

    /* Hardware Initialization */
    encoder_init();
    led_init();
    led_pattern_init();
    
    //USB_Init();
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
                                          const uint16_t ReportSize)
{
    
}


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    led_set_all(0,0,0);
    led_commit();
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    Endpoint_ConfigureEndpoint(CONFIG_OUT_EPADDR, EP_TYPE_BULK, CONFIG_EPSIZE, 1);
    Endpoint_ConfigureEndpoint(CONFIG_IN_EPADDR,  EP_TYPE_BULK, CONFIG_EPSIZE, 1);
    HID_Device_ConfigureEndpoints(&Inputs_HID_Interface);
    HID_Device_ConfigureEndpoints(&LED_HID_Interface);

    USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    USB_Process_BOS();
    HID_Device_ProcessControlRequest(&Inputs_HID_Interface);
    HID_Device_ProcessControlRequest(&LED_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Inputs_HID_Interface);
    HID_Device_MillisecondElapsed(&LED_HID_Interface);
    
    int8_t x = encoder_get(0);
    int8_t y = encoder_get(1);
    led_knobs_update(x, y);
    encoder_set(0, 0);
    encoder_set(1, 0);
    // we use a sentinel since this is actually inside an interrupt!
    // less LED flicker if ran outside
    if(led_on_frame()) {
        updateLEDs = 1;
    }
}
