#ifndef Encoder_h_
#define Encoder_h_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void encoder_init(void);
int8_t encoder_get(uint8_t num);
void encoder_set(uint8_t num, int8_t val);

#endif
