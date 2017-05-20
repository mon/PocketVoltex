#ifndef _LED_PATTERNS_H
#define _LED_PATTERNS_H

// circular dependency with Config.h requires this to be here
enum LEDMode {
    INIT_FLASH = 0,
    NONE = 1,
    SINGLE_COLOUR = 2,
    FOLLOWER = 3,
    BREATHE = 4,
};

#include <stdint.h>
#include "LED.h"
#include "Config.h"

#define LED_FRAMERATE 30
#define LED_MS_PER_FRAME (1000 / LED_FRAMERATE)

// brightness levels per frame
#define FLASH_SPEED    (BRIGHTNESS_LEVELS/21)
#define FOLLOW_SPEED   (BRIGHTNESS_LEVELS/64)
#define BREATHE_SPEED  (BRIGHTNESS_LEVELS/64)

#define LED_KNOB_SPEED (BRIGHTNESS_LEVELS/42)
#define LED_KNOB_HOLD  300
#define LED_KNOB_FADE  (BRIGHTNESS_LEVELS/128)
#define LED_KNOB_SENSITIVITY 5

uint8_t led_on_frame(void);
void led_pattern_init(void);
void led_pattern_animate(void);
void led_pattern_next(void);
void led_overlay_knobs(void);
void led_knobs_update(int8_t left, int8_t right);

#endif