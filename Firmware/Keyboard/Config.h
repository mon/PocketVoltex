#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <LUFA/Drivers/USB/USB.h>

// 7 gameplay switches + macro combo switch
#define SWITCH_COUNT 8
#define MAGIC_RESET_NUMBER 42
#define FIRMWARE_VERSION 1

typedef struct {
    // used to reboot into programming mode
    uint8_t reboot;
    uint8_t version;
    // SWITCH ORDER: A-D, FXL-R, START
    uint8_t switches[SWITCH_COUNT];
    bool ledsOn;
    uint8_t debounce;
    uint8_t version;
    // used to reboot into programming mode
    uint8_t reboot;
} sdvx_config_t;

// + 1 for some reason 
// TODO: what is the reason
#define CONFIG_BYTES sizeof(sdvx_config_t)

extern sdvx_config_t sdvxConfig;

extern void InitConfig(void);
extern void SetConfig(sdvx_config_t* config);

#endif