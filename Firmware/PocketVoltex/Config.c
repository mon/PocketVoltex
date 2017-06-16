#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "Config.h"
#include "Macro.h"

sdvx_config_t eeConfig EEMEM;

sdvx_config_t sdvxConfig;

static const PROGMEM sdvx_config_t defaults = {
    .configVersion = CONFIG_VERSION,
    .switches = {
        HID_KEYBOARD_SC_ENTER,
        HID_KEYBOARD_SC_Z,
        HID_KEYBOARD_SC_X,
        HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,
        HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,
        HID_KEYBOARD_SC_C,
        HID_KEYBOARD_SC_M,
        HID_KEYBOARD_SC_KEYPAD_PLUS}, // macro
    .controlMode = CONTROL_JOYSTICK,
    .btColour = {BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX}, // white
    .fxColour = {BRIGHTNESS_MAX, BRIGHTNESS_MAX/4, 0},            // orange
    .breatheColour = {BRIGHTNESS_MAX/2, BRIGHTNESS_MAX/8, 0},     // dim orange
    .knobColours = {
        {0,BRIGHTNESS_MAX,BRIGHTNESS_MAX},                        // aqua
        {BRIGHTNESS_MAX,0,BRIGHTNESS_MAX}                         // pink
    },
    .lightsOn = 1,
    .hidLights = 1,
    .keyLights = 1,
    .knobLights = 1,
    .lightPattern = FOLLOWER,
    .macroClick = {
        .action = KEYPRESS,
        .keypress = HID_KEYBOARD_SC_KEYPAD_PLUS
    },
    .macroHold = {
        .action = PIN_ENTRY,
        .keypress = 0
    },
    .macroLen = 4,
    .macroPin = {HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,
                 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT},
};

void InitConfig(void) {
    // TODO: if config version changes, migrate settings
    switch(eeprom_read_word(&eeConfig.configVersion)) {
        case CONFIG_VERSION: // nothing needs to change
            break;
        default:
            memcpy_P(&sdvxConfig, &defaults, CONFIG_SIZE);
            eeprom_update_block(&sdvxConfig, &eeConfig, CONFIG_SIZE);
            break;
    }
    eeprom_read_block(&sdvxConfig, &eeConfig, CONFIG_SIZE);
}

void SetConfig(sdvx_config_t* config) {
    memcpy(&sdvxConfig, config, CONFIG_SIZE);
    sdvxConfig.configVersion = CONFIG_VERSION;
    eeprom_update_block(&sdvxConfig, &eeConfig, CONFIG_SIZE);
}

void UpdateConfig(void) {
    eeprom_update_block(&sdvxConfig, &eeConfig, CONFIG_SIZE);
}

command_response_t HandleConfig(uint8_t* buffer) {
    command_t *command = (command_t*)buffer;
    switch(command->command) {
        case VERSION:
            command->data.version.version = FIRMWARE_VERSION;
            command->data.version.serial = 0xDEAD; // TODO set properly later
            return RESPOND;
        case GETCONFIG:
            memcpy(&command->data.config, &sdvxConfig, CONFIG_SIZE);
            return RESPOND;
        case SETCONFIG:
            SetConfig(&command->data.config);
            return IGNORE;
        case RESET:
            return REBOOT;
        default:
            return IGNORE;
    }
}