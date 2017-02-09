#ifndef _LED_PATTERNS_H
#define _LED_PATTERNS_H

#include <stdint.h>
#include "LED.h"

#define LED_FRAMERATE 30
#define LED_MS_PER_FRAME (1000 / LED_FRAMERATE)

// brightness levels per frame
#define FLASH_SPEED    12
#define FOLLOW_SPEED   4
#define LED_KNOB_SPEED 4
#define LED_KNOB_HOLD  200
#define LED_KNOB_FADE  12

enum LEDMode {
    NONE = 0,
    INIT_FLASH,
    SINGLE_COLOUR,
    FOLLOWER,
    BREATHE,
};

uint8_t led_on_frame(void);
void led_animate(void);
void led_anim_flash(void);
void led_anim_follower(void);
void led_knobs_update(int8_t left, int8_t right);

#endif