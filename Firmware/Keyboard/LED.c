#include "LED.h"
//#include <util/delay.h>

#define BRIGHTNESS_LEVELS 64
#define GND_COUNT 4
// RGB * 2
#define LED_PINS 6

// LED gnd 0-3 are on PC7-4
#define GND_PORT PORTC
#define GND_DDR  DDRC
#define GND_MASK 0xF0
#define GND_OFFSET 4 // in bits

// LED power BGR BGR PB2-7
#define LED_PORT PORTB
#define LED_DDR  DDRB
#define LED_MASK (0b111111 << 2)

#define R 2
#define G 1
#define B 0

//typedef struct {
//    uint8_t gnd;
//    uint8_t brightness;
//} matrix_t;

// Arranged left to right, bottom to top
static volatile uint8_t leds[LED_COUNT][3];
//volatile matrix_t current;
volatile uint8_t currentGnd;
// This saves us doing a costly dynamic _BV()
volatile uint8_t currentGndMask;
volatile uint8_t currentBright;

void led_init() {
    // all GNDs low level for high impedence or gnd
    GND_PORT &= ~GND_MASK;
    // all GNDs input
    GND_DDR &= ~GND_MASK;
    
    // Because we roll over on each loop and want to start at 0
    currentBright = BRIGHTNESS_LEVELS - 1;
    currentGnd = GND_COUNT - 1;
    currentGndMask = _BV(GND_OFFSET);
    
    // all LEDs off
    LED_PORT &= ~LED_MASK;
    // all LEDs output
    LED_DDR |= LED_MASK;
    
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        leds[i][R] = 0;
        leds[i][G] = 0;
        leds[i][B] = 0;
    }
    
    // 64 light levels * 60Hz update * 4 different GND pins = 15360Hz
    // 520 clock cycles for our interrupt handler
    // CTC mode
    TCCR0A = _BV(WGM01);
    // clk/8 prescaler
    TCCR0B = _BV(CS01);
    // 0.16% error on above calculation
    OCR0A = 128;//64; DEBUG SLOWER
    // Enable interrupt on OCR0A
    TIMSK0 = _BV(OCIE0A);
    // Clear interrupt
    TIFR0 = _BV(OCF0A);
    
    /*debug shit
    sei();
    while(1) {
        led_set(0, 32, 0, 0);
        _delay_ms(500);
        led_set(0, 0, 0, 0);
        _delay_ms(500);
        
        currentGnd++;
        currentGnd %= GND_COUNT;
        GND_ENABLE(currentGnd);
        LED_PORT &= ~LED_MASK;
        LED_PORT |= 0b100010 << 2;
        _delay_ms(500);
        LED_PORT &= ~LED_MASK;
        LED_PORT |= 0b010001 << 2;
        _delay_ms(500);
        LED_PORT &= ~LED_MASK;
        LED_PORT |= 0b001100 << 2;
        _delay_ms(500);
        GND_DISABLE(currentGnd);
    }*/
}

void led_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
    leds[num][R] = r;
    leds[num][G] = g;
    leds[num][B] = b;
}
//if(leds[(currentGnd << 1) + side][colour] <= currentBright)
#define LED_SET(side, colour, outPin) \
    if(leds[offset + side][colour] > currentBright) \
        out |= _BV(outPin)

// This function takes about 279 clock cycles. Could be improved :(
// Optimised GND accesses got it to 157
ISR(TIMER0_COMPA_vect) {
    uint8_t out = 0;
    
    currentGnd++;
    currentGndMask >>= 1;
    if(currentGnd >= GND_COUNT) {
        currentGnd = 0;
        currentGndMask = _BV(7);
        ++currentBright;
        currentBright %= BRIGHTNESS_LEVELS;
    }
    
    uint8_t offset = currentGnd * 2;
    // Faster than loops
    LED_SET(0, 0, 2);
    LED_SET(0, 1, 3);
    LED_SET(0, 2, 4);
    LED_SET(1, 0, 5);
    LED_SET(1, 1, 6);
    LED_SET(1, 2, 7);
    
    // Turn off before switch
    LED_PORT &= ~LED_MASK;
    GND_DDR &= ~GND_MASK;
    // Enable new ground
    GND_DDR |= currentGndMask;
    LED_PORT |= out;
}