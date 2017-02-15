#include <Config.h>

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define MAGIC_NUMBER 42

uint8_t firstRun EEMEM; // init to 255
sdvx_config_t eeConfig EEMEM;

sdvx_config_t sdvxConfig;

static const PROGMEM sdvx_config_t defaults = {
    .switches = {
        HID_KEYBOARD_SC_Z,
        HID_KEYBOARD_SC_X,
        HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,
        HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,
        HID_KEYBOARD_SC_C,
        HID_KEYBOARD_SC_M,
        HID_KEYBOARD_SC_ENTER,
        HID_KEYBOARD_SC_KEYPAD_PLUS}, // macro
    .btColour = {BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX}, // white
    .fxColour = {BRIGHTNESS_MAX, BRIGHTNESS_MAX/4, 0},            // orange
    .knobColours = {
        {0,BRIGHTNESS_MAX,BRIGHTNESS_MAX},                        // aqua
        {BRIGHTNESS_MAX,0,BRIGHTNESS_MAX}                         // pink
    },
    .lightsOn = 1,
    .hidLights = 1,
    .keyLights = 1,
    .knobLights = 1,
    .lightPattern = FOLLOWER,
    .macroClick = HID_KEYBOARD_SC_KEYPAD_PLUS,
    .macroPin = {HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT}
    .joystickMode = 1,
};

void InitConfig(void) {
    if (eeprom_read_byte(&firstRun) != MAGIC_NUMBER) { // store defaults
        memcpy_P(&sdvxConfig, &defaults, sizeof(sdvx_config_t));
        eeprom_write_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
        eeprom_write_byte(&firstRun, MAGIC_NUMBER); // defaults set
    }
    eeprom_read_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
}

void SetConfig(sdvx_config_t* config) {
    memcpy(&sdvxConfig, config, sizeof(sdvx_config_t));
    
    eeprom_write_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
}

command_response_t HandleConfig(uint8_t* buffer) {
    if(buffer[0] == MAGIC_RESET_NUMBER) {
        return REBOOT;
    }
    return IGNORE;
}