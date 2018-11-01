#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <LUFA/Drivers/USB/USB.h>
#include "LEDPatterns.h"
#include "Macro.h"

// 7 gameplay switches + macro combo switch
#define SWITCH_COUNT 8
#define JOYSTICK_PPR (24 * 4)
#define MAGIC_RESET_NUMBER 42
// divide by 10 for actual version
#define FIRMWARE_VERSION 15
// increment whenever config structure has breaking changes
#define CONFIG_VERSION 3

// not configurable since they're all the same switches
#define SWITCH_DEBOUNCE 30

typedef enum {
    CONTROL_KEYBOARD_MOUSE = 0,
    CONTROL_JOYSTICK = 1,
    CONTROL_EAC = 2
} control_mode_t;

typedef struct {
    // So we can detect and upgrade without losing settings
    uint16_t configVersion;
    // SWITCH ORDER: START, A-D, FXL-R
    uint8_t switches[SWITCH_COUNT];
    uint8_t controlMode;
    RGB_t btColour;
    RGB_t fxColour;
    RGB_t breatheColour;
    RGB_t knobColours[2];
    uint8_t lightsOn;
    uint8_t hidLights;
    uint8_t keyLights;
    uint8_t knobLights;
    uint8_t lightPattern;
    // When tapping or long-pressing the macro key
    Macro_t macroClick;
    Macro_t macroHold;
    // Reused for both tap or hold, at the end in case I decide to extend
    uint8_t macroLen; // placeholder not used yet
    uint8_t macroPin[4];
    
    // added in firmware 1.5
    uint8_t ledBrightness; // rev6 and up
    RGB_t startColour;
} ATTR_PACKED sdvx_config_t;

#define CONFIG_SIZE sizeof(sdvx_config_t)

typedef struct {
    uint16_t version;
    uint16_t board;
} version_t;

typedef enum {
    GETCONFIG = 1,
    SETCONFIG = 2,
    VERSION = 3, // synced with bootloader, DO NOT CHANGE
    DEFAULTCONFIG = 4,
    RESET = MAGIC_RESET_NUMBER
} command_action_t;

typedef enum {
    RESPOND = 0,
    IGNORE,
    REBOOT
} command_response_t;

typedef struct {
    command_action_t command;
    union {
        sdvx_config_t config;
        version_t     version;
    } data;
} command_t;

#define COMMAND_BYTES sizeof(command_t)

extern sdvx_config_t sdvxConfig;

extern void InitConfig(void);
extern void SetConfig(sdvx_config_t* config);
void UpdateConfig(void);
extern command_response_t HandleConfig(uint8_t* buffer);

#endif