#include "LEDPatterns.h"
#include <avr/pgmspace.h>

static uint8_t flash = 0;;
static uint8_t frameCounter = 0;

static RGB_t initFlashColour = {BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX};

typedef struct {
    uint8_t value[3];
    uint8_t direction;
} RGBFader;

typedef struct {
    struct {
        uint8_t level;
        uint8_t dir;
        RGB_t *colour;
    } breathe;

    RGB_t singleColour;

    RGBFader followers[LED_COUNT];
} Patterns_t;

static Patterns_t pattern;

typedef struct {
    uint8_t leds[2];
    uint8_t levels[2];
    uint16_t fadeTimer;
    uint8_t fadeOut;
    int8_t fadeBuffer;
} KnobLights;

// OPTIONS SHOULD BE: blue, pink, green, yellow
static KnobLights knobs[2] = {
    // mid left LEDs
    {{2,3}, {BRIGHTNESS_MAX, 0}},
    // mid right LEDs
    {{0,1}, {0, BRIGHTNESS_MAX}}
};

void led_knob_light_indiv(KnobLights* knob, RGB_t* colour, const uint8_t* map);
void led_update_breather(uint8_t speed);
void led_update_follower(void);
void led_init_flash(void);
void led_init_breathe(void);
void led_init_follower(void);

void led_pattern_init(void) {
    led_init_flash();
    led_init_follower();
}

// Called every 1ms
uint8_t led_on_frame(void) {
    if(++frameCounter >= LED_MS_PER_FRAME) {
        frameCounter = 0;
        return 1;
    } else {
        return 0;
    }
}

void led_pattern_animate(void) {
    if(flash) {
        led_update_breather(FLASH_SPEED);
        if(pattern.breathe.level < 5) {
            led_init_breathe();
            flash = 0;
            return;
        }
    }
    switch(sdvxConfig.lightPattern) {
        case NONE:
            led_set_all(0,0,0);
            break;
        case SINGLE_COLOUR:
            led_set_all_rgb(&sdvxConfig.breatheColour);
            break;
        case FOLLOWER:
            led_update_follower();
            break;
        case BREATHE:
            led_update_breather(BREATHE_SPEED);
            break;
        default:
            break;
    }
}

void led_pattern_next(void) {
    sdvxConfig.lightPattern++;
    if(sdvxConfig.lightPattern > BREATHE)
        // since we don't want the init flash
        sdvxConfig.lightPattern = NONE;
}

void led_overlay_knobs(void) {
    led_knob_light_indiv(&knobs[0], &sdvxConfig.knobColours[0], ledLeftCircleMap);
    led_knob_light_indiv(&knobs[1], &sdvxConfig.knobColours[1], ledRightCircleMap);
}

void led_knob_light_indiv(KnobLights* knob, RGB_t* colour, const uint8_t* map) {
    if(knob->fadeOut >= BRIGHTNESS_MAX) {
        return;
    }
    for(uint8_t led = 0; led < 2; led++) {
        uint8_t power = knob->levels[led];
        if(knob->fadeOut > power) {
            power = 0;
        } else {
            power -= knob->fadeOut;
        }
        led_fade_over_rgb(pgm_read_byte(&map[knob->leds[led]]), colour, power);
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

void led_update_breather(uint8_t speed) {
    if(pattern.breathe.dir) {
        if(pattern.breathe.level >= BRIGHTNESS_LEVELS - speed) {
            pattern.breathe.level = BRIGHTNESS_MAX;
            pattern.breathe.dir = 0;
        } else {
            pattern.breathe.level += speed;
        }
    } else {
        if(pattern.breathe.level <= speed) {
            pattern.breathe.level = 0;
            pattern.breathe.dir = 1;
        } else {
            pattern.breathe.level -= speed;
        }
    }
    
    uint8_t bright = pattern.breathe.level;
    //uint8_t bright = pgm_read_byte(&ledLogCurve[pattern.breathe.level]);
    led_fade_all_rgb(pattern.breathe.colour, bright);
}

void led_update_follower(void) {
    uint8_t followDir;
    RGBFader* follow;
    
    for(uint8_t led = 0; led < LED_COUNT; led++) {
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
}

void led_init_flash(void) {
    pattern.breathe.level = 0;
    pattern.breathe.dir = 1;
    pattern.breathe.colour = &initFlashColour;
    flash = 1;
}

void led_init_breathe(void) {
    pattern.breathe.level = 0;
    pattern.breathe.dir = 1;
    pattern.breathe.colour = &sdvxConfig.breatheColour;
}

void led_init_follower(void) {
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
    
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        for(uint8_t j = 0; j < 3; j++) {
            pattern.followers[i].value[j] = pgm_read_byte(&followStart[i][j]);
        }
        pattern.followers[i].direction = pgm_read_byte(&followStart[i][3]);
    }
}