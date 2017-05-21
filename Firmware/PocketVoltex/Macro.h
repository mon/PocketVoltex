#ifndef MACRO_H_
#define MACRO_H_

#include "PocketVoltex.h"

#define NUMPAD_START HID_KEYBOARD_SC_KEYPAD_1_AND_END
// because 0 is top instead of bottom
#define NUMPAD_0 HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT

enum MacroAction {
    KEYPRESS = 0,
    PIN_ENTRY = 1,
    LED_PATTERN_CHANGE = 2,
    LED_TOGGLE = 3,
};

typedef struct {
    uint8_t action;
    // if set
    uint8_t keypress;
} Macro_t;

uint8_t macro_make_report(uint8_t* key);
void macro_on_frame(switch_t* macro);
void macro_handle(Macro_t* macro);

#endif