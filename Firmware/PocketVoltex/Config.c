#include <Config.h>

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define MAGIC_NUMBER 42

static sdvx_config_t defaults PROGMEM = {
    .switches = {
        HID_KEYBOARD_SC_Z,
        HID_KEYBOARD_SC_X,
        HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,
        HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,
        HID_KEYBOARD_SC_C,
        HID_KEYBOARD_SC_M,
        HID_KEYBOARD_SC_ENTER},
    .ledsOn = true,
    .debounce = 30,
    .macroClick = HID_KEYBOARD_SC_KEYPAD_PLUS,
    .macroPin = {HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT}
};

uint8_t firstRun EEMEM; // init to 255
sdvx_config_t eeConfig EEMEM;

sdvx_config_t sdvxConfig;

void InitConfig(void) {
    if (eeprom_read_byte(&firstRun) != MAGIC_NUMBER) { // store defaults
        memcpy_P(&sdvxConfig, &defaults, sizeof(sdvx_config_t));
        eeprom_write_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
        eeprom_write_byte(&firstRun, MAGIC_NUMBER); // defaults set
    }
    eeprom_read_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
    sdvxConfig.version = FIRMWARE_VERSION;
}

void SetConfig(sdvx_config_t* config) {
    memcpy(&sdvxConfig, config, sizeof(sdvx_config_t));
    // Version is set in firmware, not software
    sdvxConfig.version = FIRMWARE_VERSION;
    
    eeprom_write_block(&sdvxConfig, &eeConfig, sizeof(sdvx_config_t));
}