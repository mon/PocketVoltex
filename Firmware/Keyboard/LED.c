#include "LED.h"

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

void led_init() {
    // all GNDs low level for high impedence or gnd
    GND_PORT &= ~GND_MASK;
    // all GNDs input
    GND_DDR &= ~GND_MASK;
    
    // all LEDs off
    LED_PORT &= ~LED_MASK;
    // all LEDs output
    LED_DDR |= LED_MASK;
    
    memset((uint8_t*)leds, 0, LED_RAW_COUNT);
    
    // 64 light levels * 60Hz update * 4 different GND pins = 15360Hz
    // 520 clock cycles for our interrupt handler
    // CTC mode
    TCCR0A = _BV(WGM01);
    // clk/8 prescaler
    TCCR0B = _BV(CS01);
    // 0.16% error on above calculation
    //OCR0A = 64; // 60Hz
    OCR0A = 77; // 50Hz
    //OCR0A = 48; // 80Hz
    // Enable interrupt on OCR0A
    TIMSK0 = _BV(OCIE0A);
    // Clear interrupt
    TIFR0 = _BV(OCF0A);
}

void led_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t offset = num * 3;
    leds[offset+R] = r;
    leds[offset+G] = g;
    leds[offset+B] = b;
}

void led_set_indiv(uint8_t num, uint8_t val) {
    leds[num] = val;
}

/* Straight voodoo magic, consult the Inline Assembler Cookbook
   Equivalent to:
   if(*led++ > brightness)
       out |= _BV(outPin)
*/
#define LED_PIN_SET(led, outPin)                                   \
    __asm__ volatile(                                              \
        "ld __tmp_reg__, %a["#led"]+                           \n\t\
         cp %[bright], __tmp_reg__                             \n\t\
         brcc skip%=                                           \n\t\
         ori %[out], (1 << "#outPin")                          \n\t\
         skip%=:"                                                  \
        : [out] "=r" (out), "=z" (led)              /* outputs */  \
        : "r" (out), [led] "z" (led), [bright] "r" (brightness) /* inputs */ )

// This function once took about 279 clock cycles.
// Optimised GND accesses got it to 157
// Optimised variables to static, got it to 100
// Made LED setter assembly, got it to 90
ISR(TIMER0_COMPA_vect) {
    /* Why are these static here instead of at the top of file?
       The compiler won't optimise 2 consecutive operations to use a register,
       and instead will perform a costly lds-sts every time. Making them
       static here will cache them in a local register.
    */
    // Because we roll over on each loop and want to start at 0 this starts at max
    static uint8_t currentGnd = GND_COUNT - 1;
    // This saves us doing a costly dynamic _BV()
    static uint8_t currentGndMask = 0;
    static uint8_t brightness = BRIGHTNESS_LEVELS - 1;
    static volatile uint8_t* offset = &leds[0];
    
    uint8_t out = 0;
    
    currentGnd++;
    currentGndMask >>= 1;
    if(currentGnd >= GND_COUNT) {
        currentGnd = 0;
        // Because we work backwards start at the high end and shift down
        currentGndMask = _BV(7);
        offset = &leds[0];
        ++brightness;
        brightness %= BRIGHTNESS_LEVELS;
    }

    // Faster than loops
    // NOTE: ASM MACRO INCREMENTS OFFSET
    LED_PIN_SET(offset, 2);
    LED_PIN_SET(offset, 3);
    LED_PIN_SET(offset, 4);
    LED_PIN_SET(offset, 5);
    LED_PIN_SET(offset, 6);
    LED_PIN_SET(offset, 7);
    
    // Turn off before switch
    LED_PORT &= ~LED_MASK;
    GND_DDR &= ~GND_MASK;
    // Enable new ground
    GND_DDR |= currentGndMask;
    LED_PORT |= out;
}