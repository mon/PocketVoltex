#include "PocketVoltex.h"
#include "Config.h"
#include "Encoder.h"
#include "LED.h"
#include "LEDPatterns.h"
#include "Macro.h"

#define LOAD_SWITCH(source, sourceBit, result, resultBit) result |= !((source) & _BV(sourceBit)) << resultBit
// B 0,1
#define SWITCH_MASKB 0b00000011
// C 1,2
#define SWITCH_MASKC 0b00000110
// D 4,5,6,7
#define SWITCH_MASKD 0b11110000

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
    uint8_t mainLights[LED_PHYSICAL_COUNT];
    uint8_t btFx[6];
} LED_Report_t;

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevInputsHIDReportBuffer[MAX(sizeof(Keyboard_Report_t), sizeof(USB_MouseReport_Data_t))];
static uint8_t PrevLEDHIDReportBuffer[sizeof(LED_Report_t)];

static uint8_t sendKeyboard = 0;

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
            .PrevReportINBuffer           = PrevInputsHIDReportBuffer,
            .PrevReportINBufferSize       = sizeof(PrevInputsHIDReportBuffer),
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

static switch_t switches[SWITCH_COUNT];
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

/* This is verbose and annoying but the nice method wastes 30 bytes of RAM */
uint8_t load_switches(void) {
    uint8_t tmp;
    uint8_t result = 0;
    
    tmp = PINB;
    LOAD_SWITCH(tmp, 1, result, 0); // PINB1, A
    LOAD_SWITCH(tmp, 0, result, 4); // PINB0, FX L
    tmp = PINC;
    LOAD_SWITCH(tmp, 2, result, 6); // PINC2, START
    LOAD_SWITCH(tmp, 1, result, 7); // PINC1, MACRO
    tmp = PIND;
    LOAD_SWITCH(tmp, 7, result, 1); // PIND7, B
    LOAD_SWITCH(tmp, 5, result, 2); // PIND5, C
    LOAD_SWITCH(tmp, 4, result, 3); // PIND4, D
    LOAD_SWITCH(tmp, 6, result, 5); // PIND6, FX L
    
    return result;
}

void update_switches(void) {
    uint8_t i, newState;
    uint8_t switchRead = load_switches();
    
    for(i = 0; i < SWITCH_COUNT; i++) {
        newState = switchRead & 1;
        if(!switches[i].debounce && newState != switches[i].lastReport) {
            switches[i].state = newState;
            switches[i].debounce = SWITCH_DEBOUNCE;
            switchesChanged = 1;
        }
        switchRead >>= 1;
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
        HID_Device_USBTask(&Inputs_HID_Interface);
        HID_Device_USBTask(&LED_HID_Interface);
        USB_USBTask();
        
        Endpoint_SelectEndpoint(CONFIG_OUT_EPADDR);
        if (Endpoint_IsOUTReceived()) {
            uint8_t ReceivedData[CONFIG_EPSIZE];
            Endpoint_Read_Stream_LE(ReceivedData, CONFIG_EPSIZE, NULL);
            Endpoint_ClearOUT();
            
            command_response_t respond = HandleConfig(ReceivedData);
            switch(respond) {
                // we are returning the requested data
                case RESPOND:
                    Endpoint_SelectEndpoint(CONFIG_IN_EPADDR);
                    Endpoint_Write_Stream_LE(ReceivedData, CONFIG_EPSIZE, NULL);
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
    }
    // Inputs
    DDRB &= ~SWITCH_MASKB;
    DDRC &= ~SWITCH_MASKC;
    DDRD &= ~SWITCH_MASKD;
    // Pullups
    PORTB |= SWITCH_MASKB;
    PORTC |= SWITCH_MASKC;
    // RESET has its own pullup
    PORTC &= ~_BV(1);
    PORTD |= SWITCH_MASKD;
    
    // RESET held while plugging in
    if(!(PINC & _BV(1))) {
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
    if(ReportType != HID_REPORT_ITEM_In || HIDInterfaceInfo != &Inputs_HID_Interface) {
        *ReportSize = 0;
        return false;
    }
    sendKeyboard ^= 1;
    if (sendKeyboard) {
        Keyboard_Report_t* KeyboardReport = (Keyboard_Report_t*)ReportData;
        
        // Only the first 4 bytes are read by bemanitools, so let's use the first
        uint8_t macroUpdated = macro_make_report(&KeyboardReport->KeyCode[0]);
        update_switches();
        
        if(!switchesChanged && !macroUpdated) {
            *ReportSize = 0;
            return false;
        }
        
        // NOTE: using SWITCH_COUNT-1 so we don't write the report for the macro pin
        for(uint8_t i = 0; i < SWITCH_COUNT-1; i++) {
            // i+1 to take care of the shift from above
            KeyboardReport->KeyCode[i+1] = switches[i].state ? sdvxConfig.switches[i] : 0;
            switches[i].lastReport = switches[i].state;
            // Update blinkenlights
            if(switches[i].state) {
                //if(sdvxConfig.ledsOn) {
                //    // TODO
                //}
            }
        }
         
        *ReportID = HID_REPORTID_KeyboardReport;
        *ReportSize = sizeof(Keyboard_Report_t);
         
        switchesChanged = 0;
    } else {
        USB_MouseReport_Data_t* MouseReport = (USB_MouseReport_Data_t*)ReportData;
        
        MouseReport->X = encoder_get(0);
        MouseReport->Y = encoder_get(1);
        
        led_knobs_update(MouseReport->X, MouseReport->Y);
        
        encoder_set(0, 0);
        encoder_set(1, 0);

        *ReportID = HID_REPORTID_MouseReport;
        *ReportSize = sizeof(USB_MouseReport_Data_t);
    }
    return true;
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
    if(HIDInterfaceInfo == &LED_HID_Interface && ReportType == HID_REPORT_ITEM_Out) {
        // reset our timeout
        hidTimeout = 0;
        
        LED_Report_t* LEDReport = (LED_Report_t*)ReportData;
        //memcpy((uint8_t*)leds, LEDReport->mainLights, LED_PHYSICAL_COUNT);
        // Load the user set colours as R/G/B instead of B/G/R
        for(uint8_t i = 0; i < LED_PHYSICAL_COUNT; ) {
            uint8_t offset = i+2;
            for(uint8_t j = 0; j < 3; j++) {
                // cast away the volatile for faster ops
                ((uint8_t*)leds)[i++] = LEDReport->mainLights[offset--];
            }
        }
        
        // Keep normal lights but override when we get flashes on BT or FX
        // BT LEDs flash pure white
        for(uint8_t i = 0; i < 4; i++) {
            if(LEDReport->btFx[i]) {
                led_set(ledMap[i], BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX);
            }
        }
        // FX LEDs flash orange
        for(uint8_t i = 4; i < 6; i++) {
            if(LEDReport->btFx[i]) {
                led_set(ledMap[i], BRIGHTNESS_MAX, BRIGHTNESS_LEVELS/4, 0);
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
    led_set_all(0,0,0);
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
    
    if(hidTimeout > HID_LED_TIMEOUT) {
        led_frame();
    } else {
        hidTimeout++;
    }
    
    macro_on_frame(&switches[SWITCH_COUNT-1]);
    
    for(int i = 0; i < SWITCH_COUNT; i++) {
        if(switches[i].debounce) {
            switches[i].debounce--;
        }
    }
}
