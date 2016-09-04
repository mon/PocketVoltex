#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <LUFA/Drivers/USB/USB.h>

// For ease of code sharing with the OsuPad
#define SWITCH_COUNT 7
// essentially sizeof the config type + 1 for some reason 
// TODO: what is the reason
#define CONFIG_BYTES (SWITCH_COUNT + 4)
#define MAGIC_RESET_NUMBER 42
#define FIRMWARE_VERSION 1

typedef struct {
    // SWITCH ORDER: A-D, FXL-R, START
    uint8_t switches[SWITCH_COUNT];
    bool ledsOn;
    uint8_t debounce;
    uint8_t version;
} sdvx_config_t;

extern sdvx_config_t sdvxConfig;

extern void InitConfig(void);
extern void SetConfig(uint8_t* config);

#endif