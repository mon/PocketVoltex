#ifndef _LED_PATTERNS_H
#define _LED_PATTERNS_H

#include <stdint.h>
#include "LED.h"

#define LED_FRAMERATE 30
#define LED_MS_PER_FRAME (1000 / LED_FRAMERATE)

// brightness levels per frame
#define FLASH_SPEED    (BRIGHTNESS_LEVELS/21)
#define FOLLOW_SPEED   (BRIGHTNESS_LEVELS/64)
#define LED_KNOB_SPEED (BRIGHTNESS_LEVELS/42)
#define LED_KNOB_HOLD  300
#define LED_KNOB_FADE  (BRIGHTNESS_LEVELS/128)
#define LED_KNOB_SENSITIVITY 3


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
void led_knob_lights(void);
void led_knobs_update(int8_t left, int8_t right);

#endif