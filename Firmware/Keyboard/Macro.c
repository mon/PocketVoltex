#include <Macro.h>

// in ms
#define KEY_DELAY 30
#define LONGPRESS_DELAY 500

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
        return 1;
    } else {
        if(position >= dataLen) {
            return 0;
        }
        
        *key = data[position];
        countdown = KEY_DELAY;
       
        position++;
        liftoff = 1;
        return 1;
   }
}

void macro_fire_click(void) {
    data = &sdvxConfig.macroClick;
    dataLen = 1;
    position = 0;
}

void macro_fire_pin(void) {
    data =  sdvxConfig.macroPin;
    dataLen = 4;
    position = 0;
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
            macro_fire_pin();
        }
    // falling edge before longpress time
    } else if(!macro->state && macro->lastReport && longpress < LONGPRESS_DELAY) {
        macro_fire_click();
    }
    macro->lastReport = macro->state;
}