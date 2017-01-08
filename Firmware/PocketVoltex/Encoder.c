#include "Encoder.h"

// SDVX Controller - PD0-3
#define ENCODER_COUNT 2

#define ENCODER_PORT PORTD
#define ENCODER_PIN  PIND
#define ENCODER_DDR  DDRD

#define GET_ENCODER_0() (ENCODER_PIN & 0b11)
#define GET_ENCODER_1() ((ENCODER_PIN >> 2) & 0b11)

typedef struct {
    int8_t position;
    uint8_t state;
} encoder_t;

static volatile encoder_t encoders[ENCODER_COUNT] = {};

void encoder_init(void) {
    // inputs
    ENCODER_DDR &= ~0x0F;
    // pullups
    ENCODER_PORT |= 0x0F;
    
    encoders[0].state = GET_ENCODER_0();
    encoders[1].state = GET_ENCODER_1();
    
    // Edge interrupts on all INT pins
    EICRA = _BV(ISC30) | _BV(ISC20) | _BV(ISC10) | _BV(ISC00);
    // Enable the interrupts
    EIMSK = _BV(INT3) | _BV(INT2) | _BV(INT1) | _BV(INT0);
    // Clear interrupt flags
    EIFR  = _BV(INTF3) | _BV(INTF2) | _BV(INTF1) | _BV(INTF0);
}

int8_t encoder_get(uint8_t num) {
    return encoders[num].position;
}

void encoder_set(uint8_t num, int8_t val) {
    encoders[num].position = val;
}

// Adapted from the wonderful Encoder.h by PJRC
void update(uint8_t num, uint8_t newState) {
    int8_t position = encoders[num].position;
    uint8_t state = encoders[num].state | (newState << 2);
    encoders[num].state = newState;
    switch (state) {
        case 1: case 7: case 8: case 14:
            if(position < 127)
                encoders[num].position++;
            return;
        case 2: case 4: case 11: case 13:
            if(position > -128)
                encoders[num].position--;
            return;
        case 3: case 12:
            if(position < 125)
                encoders[num].position += 2;
            return;
        case 6: case 9:
            if(position > -126)
                encoders[num].position -= 2;
            return;
    }
    
}
    
ISR(INT0_vect) { update(0, GET_ENCODER_0()); }
ISR(INT1_vect) { update(0, GET_ENCODER_0()); }
ISR(INT2_vect) { update(1, GET_ENCODER_1()); }
ISR(INT3_vect) { update(1, GET_ENCODER_1()); }