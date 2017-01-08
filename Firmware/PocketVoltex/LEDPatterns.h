#ifndef _LED_PATTERNS_H
#define _LED_PATTERNS_H

#include <stdint.h>
#include "LED.h"

#define LED_FRAMERATE 30
#define LED_MS_PER_FRAME (1000 / LED_FRAMERATE)

enum LEDMode {
    NONE = 0,
    INIT_FLASH,
    FOLLOWER,
    BREATHE,
};

void led_frame(void);
void led_anim_flash(void);
void led_anim_follower(void);
void led_knobs_update(int8_t left, int8_t right);

#endif