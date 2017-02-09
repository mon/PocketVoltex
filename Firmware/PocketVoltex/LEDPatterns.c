#include "LEDPatterns.h"
#include <avr/pgmspace.h>

static enum LEDMode animMode = NONE;
static uint8_t frameCounter = 0;

typedef struct {
    uint8_t value[3];
    uint8_t direction;
} RGBFader;

// stuff for the different modes
// because only 1 mode is active at a time, config is stored in EEPROM, and
// modes are initialised, we can save a little RAM and store these as a union
typedef union {
    struct {
        int8_t level;
        int8_t dir;
    } flash;

    RGB_t singleColour;

    struct {
        int8_t level;
        RGB_t colour;
    } breathe;

    RGBFader followers[LED_COUNT];
} Patterns_t;

static Patterns_t pattern;

// These can always be active and are thus not unionised. Capitalism wins again.

typedef struct {
    const uint8_t rgb[3];
    uint8_t leds[2];
    uint8_t levels[2];
} KnobLights;

// OPTIONS SHOULD BE: blue, pink, green, yellow
static KnobLights knobs[2] = {
    // Aqua, top left LEDs
    {{0,1,1}, {0,1}, {BRIGHTNESS_LEVELS-1, 0}},
    // Pink, top right LEDs
    {{1,0,1}, {6,7}, {0, BRIGHTNESS_LEVELS-1}}
};

// Called every 1ms
void led_frame(void) {
    if(++frameCounter < LED_MS_PER_FRAME) {
        return;
    }
    
    frameCounter = 0;
    
    switch(animMode) {
        case INIT_FLASH:
            if(pattern.flash.dir) {
                pattern.flash.level += 3;
                if(pattern.flash.level >= BRIGHTNESS_LEVELS - 1) {
                    pattern.flash.level = BRIGHTNESS_LEVELS - 1;
                    pattern.flash.dir = 0;
                }
            } else {
                if(pattern.flash.level <= 3) {
                    pattern.flash.level = 0;
                    led_anim_follower();
                } else {
                    pattern.flash.level -= 3;
                }
            }
        
            uint8_t bright = pgm_read_byte(&ledLogCurve[pattern.flash.level]);
            // yellowy orange
            //led_set_all(bright, bright/2, 0);
            // I think white looks nicer
            led_set_all(bright, bright, bright);
            break;
        case FOLLOWER:
            for(uint8_t led = 0; led < LED_COUNT; led++) {
                uint8_t followDir;
                RGBFader* follow;
                
                follow = pattern.followers + led;
                followDir = follow->direction;
                for(uint8_t i = 0; i < 3; i++) {
                    if(followDir & 1) {
                        if(follow->value[i]++ >= BRIGHTNESS_LEVELS - 1) {
                            follow->value[i] = BRIGHTNESS_LEVELS - 1;
                            follow->direction <<= 1;
                            if(follow->direction == 0b1000) {
                                follow->direction = 0b001;
                            }
                        }
                    } else {
                        if(follow->value[i] > 0) {
                            follow->value[i]--;
                        }
                    }
                    followDir >>= 1;
                }
                led_set(led, follow->value[0], follow->value[1], follow->value[2]);
            }
            break;
        case BREATHE:
        default:
            break;
    }
    
    // Knob lights, what shall I do with you?
    /*//led_set_all(0,0,0);
    for(uint8_t i = 0; i < 2; i++) {
        for(uint8_t led = 0; led < 2; led++) {
            uint8_t r = knobs[i].levels[led] * knobs[i].rgb[0];
            uint8_t g = knobs[i].levels[led] * knobs[i].rgb[1];
            uint8_t b = knobs[i].levels[led] * knobs[i].rgb[2];
            led_set_max(ledCircleMap[knobs[i].leds[led]], r, g, b);
        }
    }*/
}

void led_knob_indiv(KnobLights* knob, int8_t value) {
    /* This is annoying and repetitive but don't think there's a nicer way */
    if(value > 0) {
        knob->levels[0] += value;
        if(value > knob->levels[1]) {
            // Not properly doing rollover since the knob doesn't spin that fast
            knob->levels[1] = 0;
        } else {
            knob->levels[1] -= value;
        }
        
        if(knob->levels[0] > BRIGHTNESS_LEVELS) {
            knob->levels[0] = 0;
            knob->levels[1] = BRIGHTNESS_LEVELS - 1;
            knob->leds[1] = knob->leds[0];
            if(knob->leds[0] > 0) {
                knob->leds[0]--;
            } else {
                knob->leds[0] = LED_COUNT - 1;
            }
        }
    } else if(value < 0) {
        if(-value > knob->levels[0]) {
            knob->levels[0] = 0;
        } else {
            knob->levels[0] += value;
        }
        knob->levels[1] -= value;
        
        if(knob->levels[1] > BRIGHTNESS_LEVELS) {
            knob->levels[0] = BRIGHTNESS_LEVELS - 1;
            knob->levels[1] = 0;
            knob->leds[0] = knob->leds[1];
            knob->leds[1]++;
            knob->leds[1] %= LED_COUNT;
        }
    }
}

void led_knobs_update(int8_t left, int8_t right) {
    led_knob_indiv(&knobs[0], left*2);
    led_knob_indiv(&knobs[1], right*2);
}

void led_anim_flash(void) {
    animMode = INIT_FLASH;
    // starting at 2 stops us getting a frame of red
    pattern.flash.level = 2;
    pattern.flash.dir = 1;
}

void led_anim_follower(void) {
    // Generated by FollowerGen.py
    static uint8_t PROGMEM followStart[][4] = {
        {63, 0, 0, 0b010},
        {40, 0, 23, 0b001},
        {39, 24, 0, 0b010},
        {16, 0, 47, 0b001},
        {15, 48, 0, 0b010},
        {0, 7, 56, 0b100},
        {0, 55, 8, 0b100},
        {0, 31, 32, 0b100},
    };
    animMode = FOLLOWER;
    
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        for(uint8_t j = 0; j < 3; j++) {
            pattern.followers[i].value[j] = pgm_read_byte(&followStart[i][j]);
        }
        pattern.followers[i].direction = pgm_read_byte(&followStart[i][3]);
    }
}