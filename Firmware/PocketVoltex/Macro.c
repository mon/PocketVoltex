#include "Macro.h"
#include "LEDPatterns.h"

// in ms
#define KEY_DELAY 30
#define LONGPRESS_DELAY 500

extern sdvx_config_t sdvxConfig;

static uint8_t countdown = 0;
static uint16_t longpress = 0;
static uint8_t *data;
static uint8_t dataLen = 0;
static uint8_t position = 0;
static uint8_t liftoff = 0;

uint8_t macro_make_report(uint8_t* key) {
    if(countdown) {
        return 0;
    }
    
    if(liftoff) {
        liftoff = 0;
        countdown = KEY_DELAY;
        *key = 0;
    } else {
        if(position >= dataLen) {
            return 0;
        }
        
        *key = data[position];
        countdown = KEY_DELAY;
       
        position++;
        liftoff = 1;
   }
   return 1;
}

void macro_handle(Macro_t* macro) {
    switch(macro->action) {
        case KEYPRESS:
            data = &macro->keypress;
            dataLen = 1;
            position = 0;
            break;
        case PIN_ENTRY:
            data =  sdvxConfig.macroPin;
            dataLen = 4;
            position = 0;
            break;
        case LED_PATTERN_CHANGE:
            led_pattern_next();
            break;
        case LED_TOGGLE:
            sdvxConfig.lightsOn ^= 1;
            UpdateConfig();
            break;
    }
}

void macro_on_frame(switch_t* macro) {
    if(countdown) {
        countdown--;
    }
    
    // Rising edge
    if(macro->state && !macro->lastReport) {
        longpress = 0;
    // held high and not triggered longpress yet
    } else if(macro->state && macro->lastReport && longpress < LONGPRESS_DELAY) {
        longpress++;
        if(longpress >= LONGPRESS_DELAY) {
            macro_handle(&sdvxConfig.macroHold);
        }
    // falling edge before longpress time
    } else if(!macro->state && macro->lastReport && longpress < LONGPRESS_DELAY) {
        macro_handle(&sdvxConfig.macroClick);
    }
    macro->lastReport = macro->state;
}