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
        uint8_t level;
        int8_t dir;
    } flash;

    RGB_t singleColour;

    struct {
        uint8_t level;
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
    uint16_t fadeTimer;
    uint8_t fadeOut;
    int8_t fadeBuffer;
} KnobLights;

// OPTIONS SHOULD BE: blue, pink, green, yellow
static KnobLights knobs[2] = {
    // Aqua, mid left LEDs
    {{0,BRIGHTNESS_MAX,BRIGHTNESS_MAX}, {2,3}, {BRIGHTNESS_MAX, 0}},
    // Pink, mid right LEDs
    {{BRIGHTNESS_MAX,0,BRIGHTNESS_MAX}, {0,1}, {0, BRIGHTNESS_MAX}}
};

void led_knob_light_indiv(KnobLights* knob, const uint8_t* map);

uint8_t led_on_frame(void) {
    if(++frameCounter >= LED_MS_PER_FRAME) {
        frameCounter = 0;
        return 1;
    } else {
        return 0;
    }
}

// Called every 1ms
void led_animate(void) {
    switch(animMode) {
        case INIT_FLASH:
            if(pattern.flash.dir) {
                if(pattern.flash.level >= BRIGHTNESS_LEVELS - FLASH_SPEED) {
                    pattern.flash.level = BRIGHTNESS_MAX;
                    pattern.flash.dir = 0;
                } else {
                    pattern.flash.level += FLASH_SPEED;
                }
            } else {
                if(pattern.flash.level <= FLASH_SPEED) {
                    pattern.flash.level = 0;
                    led_set_all(0,0,0);
                    led_anim_follower();
                    break;
                } else {
                    pattern.flash.level -= FLASH_SPEED;
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
                        if(follow->value[i] >= BRIGHTNESS_LEVELS - FOLLOW_SPEED) {
                            follow->direction <<= 1;
                            if(follow->direction == 0b1000) {
                                follow->direction = 0b001;
                            }
                        } else {
                            follow->value[i] += FOLLOW_SPEED;
                        }
                    } else {
                        if(follow->value[i] >= FOLLOW_SPEED) {
                            follow->value[i] -= FOLLOW_SPEED;
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
}

void led_knob_lights(void) {
    led_knob_light_indiv(&knobs[0], ledLeftCircleMap);
    led_knob_light_indiv(&knobs[1], ledRightCircleMap);
}

void led_knob_light_indiv(KnobLights* knob, const uint8_t* map) {
    if(knob->fadeOut >= BRIGHTNESS_MAX) {
        return;
    }
    for(uint8_t led = 0; led < 2; led++) {
        uint8_t r = knob->rgb[0];
        uint8_t g = knob->rgb[1];
        uint8_t b = knob->rgb[2];
        uint8_t power = knob->levels[led];
        if(knob->fadeOut > power) {
            power = 0;
        } else {
            power -= knob->fadeOut;
        }
        //led_set_max(ledCircleMap[knobs[i].leds[led]], r, g, b);
        led_fade_over(pgm_read_byte(&map[knob->leds[led]]), r, g, b, power);
    }
}

void led_knob_indiv(KnobLights* knob, int8_t value) {
    if(value == 0) {
        // hold before fadeout
        if(knob->fadeTimer > 0) {
            knob->fadeTimer--;
            if(knob->fadeTimer == 0)
                knob->fadeBuffer = 0;
        // actually fade out
        } else if(knob->fadeOut < BRIGHTNESS_MAX - LED_KNOB_FADE) {
            knob->fadeOut += LED_KNOB_FADE;
        // completely faded
        } else {
            knob->fadeOut = BRIGHTNESS_MAX;
        }
        return;
    }
    knob->fadeBuffer += value;
    // bumped it enough to reenable lights, or the timeout has yet to occur
    if(knob->fadeTimer || knob->fadeBuffer > LED_KNOB_SENSITIVITY || knob->fadeBuffer < -LED_KNOB_SENSITIVITY) {
        knob->fadeTimer = LED_KNOB_HOLD;
        knob->fadeOut = 0;
    }
    /* This is annoying and repetitive but don't think there's a nicer way */
    value *= LED_KNOB_SPEED;
    if(value > 0) {
        if(value > knob->levels[1]) {
            // Not proper doing rollover since the knob doesn't spin that fast
            knob->levels[0] = 0;
            knob->levels[1] = BRIGHTNESS_MAX;
            knob->leds[1] = knob->leds[0];
            if(knob->leds[0] > 0) {
                knob->leds[0]--;
            } else {
                knob->leds[0] = LED_COUNT/2 - 1;
            }
        } else {
            knob->levels[0] += value;
            knob->levels[1] -= value;
        }
    } else if(value < 0) {
        if(-value > knob->levels[0]) {
            knob->levels[0] = BRIGHTNESS_MAX;
            knob->levels[1] = 0;
            knob->leds[0] = knob->leds[1];
            knob->leds[1]++;
            knob->leds[1] %= LED_COUNT/2;
        } else {
            knob->levels[0] += value;
            knob->levels[1] -= value;
        }
    }
}

void led_knobs_update(int8_t left, int8_t right) {
    led_knob_indiv(&knobs[0], left*2);
    led_knob_indiv(&knobs[1], right*2);
}

void led_anim_flash(void) {
    animMode = INIT_FLASH;
    // starting at 2 stops us getting a frame of red if you're not doing white flash
    //pattern.flash.level = 2;
    pattern.flash.level = 0;
    pattern.flash.dir = 1;
}

void led_anim_follower(void) {
    // Generated by FollowerGen.py
    static const PROGMEM uint8_t followStart[][4] = {
        {255, 0, 0, 0b010},
        {160, 0, 95, 0b001},
        {159, 96, 0, 0b010},
        {64, 0, 191, 0b001},
        {63, 192, 0, 0b010},
        {0, 31, 224, 0b100},
        {0, 223, 32, 0b100},
        {0, 127, 128, 0b100},
    };
    animMode = FOLLOWER;
    
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        for(uint8_t j = 0; j < 3; j++) {
            pattern.followers[i].value[j] = pgm_read_byte(&followStart[i][j]);
        }
        pattern.followers[i].direction = pgm_read_byte(&followStart[i][3]);
    }
}