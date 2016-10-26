#ifndef LED_h_
#define LED_h_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_COUNT 8

void led_init(void);
void led_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b);

#endif
