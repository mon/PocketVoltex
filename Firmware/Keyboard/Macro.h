#ifndef MACRO_H_
#define MACRO_H_

#include "Config.h"
#include "asciihid.h"
#include "Keyboard.h"

extern sdvx_config_t sdvxConfig;

uint8_t macro_make_report(uint8_t* key);
void macro_on_frame(switch_t* macro);
void macro_fire_click(void);
void macro_fire_pin(void);

#endif