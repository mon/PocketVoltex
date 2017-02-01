#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <LUFA/Drivers/USB/USB.h>
#include "LED.h"

// 7 gameplay switches + macro combo switch
#define SWITCH_COUNT 8
#define MAGIC_RESET_NUMBER 42
// divide by 10 for actual version
#define FIRMWARE_VERSION 1

// not configurable since they're all the same switches
#define SWITCH_DEBOUNCE 30

typedef struct {
    // SWITCH ORDER: A-D, FXL-R, START
    uint8_t switches[SWITCH_COUNT];
    RGB_t btColour;
    RGB_t fxColour;
    RGB_t knobColours[2];
    bool lightsOn;
    bool hidLights;
    bool keyLights;
    bool knobLights;
    uint8_t lightPattern;
    // When tapping or long-pressing the macro key
    uint8_t macroClick;
    uint8_t macroPin[4];
} sdvx_config_t;

#define CONFIG_BYTES sizeof(sdvx_config_t)

typedef struct {
    uint8_t version;
    uint16_t serial;
} version_t;

typedef enum {
    GETCONFIG = 1,
    SETCONFIG = 2,
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
extern command_response_t HandleConfig(uint8_t* buffer);

#endif