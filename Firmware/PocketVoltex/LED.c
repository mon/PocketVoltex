#include "LED.h"

uint8_t leds[LED_PHYSICAL_COUNT];

// I can't decide if this is disgusting or delightful
#ifdef SOFT_LEDS
    #include "LED_Driver_Software.c"
#else
    #include "LED_Driver_SK9822.c"
#endif

void led_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t offset = num * 3;
    leds[offset+R] = r;
    leds[offset+G] = g;
    leds[offset+B] = b;
}

// Applies a crossfade between the current colour and an overlay colour with a given strength
void led_fade_over(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t strength) {
    uint8_t offset = num * 3;
    // going outside max val for a signed int8
    int16_t scales[3];
    // get colour distances
    scales[R] = r - leds[offset+R];
    scales[G] = g - leds[offset+G];
    scales[B] = b - leds[offset+B];
    for(uint8_t i = 0; i < 3; i++) {
        // perform scaling with div0 check
        if(scales[i] == 0) {
            scales[i] = BRIGHTNESS_LEVELS;
        } else {
            // won't ever be 0, don't check later
            scales[i] = BRIGHTNESS_LEVELS / scales[i];
        }
        int16_t new = leds[offset+i] + strength/scales[i];
        // Integer division strikes again
        if(new > BRIGHTNESS_MAX)
            new = BRIGHTNESS_MAX;
        if(new < 0)
            new = 0;
        leds[offset+i] = new;
    }
}

// goes from a setpoint of 0 instead
void led_fade_all(uint8_t r, uint8_t g, uint8_t b, uint8_t strength) {
    // going outside max val for a signed int8
    int16_t scales[3];
    // colour distances from setpoint of 0
    scales[R] = r;
    scales[G] = g;
    scales[B] = b;
    for(uint8_t i = 0; i < 3; i++) {
        // perform scaling with div0 check
        if(scales[i] == 0) {
            scales[i] = BRIGHTNESS_LEVELS;
        } else {
            // won't ever be 0, don't check later
            scales[i] = BRIGHTNESS_LEVELS / scales[i];
        }
        int16_t new = strength/scales[i];
        // Integer division strikes again
        if(new > BRIGHTNESS_MAX)
            new = BRIGHTNESS_MAX;
        if(new < 0)
            new = 0;
        scales[i] = new;
    }
    led_set_all(scales[R], scales[G], scales[B]);
}

void led_set_all(uint8_t r, uint8_t g, uint8_t b) {
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        led_set(i, r, g, b);
    }
}

void led_set_indiv(uint8_t num, uint8_t val) {
    leds[num] = val;
}

void led_set_rgb(uint8_t num, RGB_t* colour) {
    led_set(num, colour->r, colour->g, colour->b);
}

void led_fade_over_rgb(uint8_t num, RGB_t* colour, uint8_t strength) {
    led_fade_over(num, colour->r, colour->g, colour->b, strength);
}

void led_fade_all_rgb(RGB_t* colour, uint8_t strength) {
    led_fade_all(colour->r, colour->g, colour->b, strength);
}

void led_set_all_rgb(RGB_t* colour) {
    led_set_all(colour->r, colour->g, colour->b);
}