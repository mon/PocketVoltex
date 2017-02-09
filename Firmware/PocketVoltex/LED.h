#ifndef LED_h_
#define LED_h_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#define LED_COUNT 8
#define LED_PHYSICAL_COUNT (LED_COUNT*3)
// BT + FX
#define LED_VIRTUAL_COUNT 6
#define LED_TOTAL_COUNT (LED_PHYSICAL_COUNT + LED_VIRTUAL_COUNT)
#define BRIGHTNESS_LEVELS 64

typedef struct {
    uint8_t r, g, b;
} RGB_t;

// Arranged left to right, top to bottom
// LED order is BGR...BGR
volatile uint8_t leds[LED_PHYSICAL_COUNT];

// Maps BT/FX keys to their associated LED
// Order is BT-1-4, FX-L, FX-R
static const uint8_t ledMap[] = {2, 0, 1, 3, 6, 7};

// map LEDs to run in a circle
static const uint8_t ledCircleMap[] = {0, 2, 4, 6, 7, 5, 3, 1};

void led_init(void);
void led_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b);
void led_set_max(uint8_t num, uint8_t r, uint8_t g, uint8_t b);
void led_set_all(uint8_t r, uint8_t g, uint8_t b);
void led_set_indiv(uint8_t num, uint8_t val);

PROGMEM const static uint8_t ledLogCurve[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 2, 2,
    3, 3, 4, 5, 6, 7, 8, 9,
    11, 13, 15, 17, 20, 23, 25, 28,
    31, 34, 37, 40, 43, 45, 48, 50,
    52, 53, 55, 56, 57, 58, 59, 60,
    60, 61, 61, 61, 62, 62, 62, 62,
    63, 63, 63, 63, 63, 63, 63, 63, 63};

#endif
