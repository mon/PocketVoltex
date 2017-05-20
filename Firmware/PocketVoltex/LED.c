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

#define BRIGHTNESS_INCREMENT (BRIGHTNESS_LEVELS / BRIGHTNESS_DOWNSCALE)

#define UPDATE_HZ 100
// prescaler is the div8
#define TIMER_COMPARE ((F_CPU / 8 / UPDATE_HZ / GND_COUNT / BRIGHTNESS_DOWNSCALE)-1)
#if TIMER_COMPARE > 255
    #error timer compare too large for timer register
#endif

#define R 2
#define G 1
#define B 0

uint8_t leds[LED_PHYSICAL_COUNT];
static volatile uint8_t leds_frontbuffer[LED_PHYSICAL_COUNT];

void led_init() {
    // all GNDs low level for high impedence or gnd
    GND_PORT &= ~GND_MASK;
    // all GNDs input
    GND_DDR &= ~GND_MASK;
    
    // all LEDs off
    LED_PORT &= ~LED_MASK;
    // all LEDs output
    LED_DDR |= LED_MASK;
    
    memset(leds, 0, LED_PHYSICAL_COUNT);
    memset((uint8_t*)leds_frontbuffer, 0, LED_PHYSICAL_COUNT);
    
    // 64 light levels * 60Hz update * 4 different GND pins = 15360Hz
    // 520 clock cycles for our interrupt handler
    // CTC mode
    TCCR0A = _BV(WGM01);
    // clk/8 prescaler
    TCCR0B = _BV(CS01);
    OCR0A = TIMER_COMPARE;
    // Enable interrupt on OCR0A
    TIMSK0 = _BV(OCIE0A);
    // Clear interrupt
    TIFR0 = _BV(OCF0A);
}

void led_commit(void) {
    memcpy((uint8_t*)leds_frontbuffer, leds, LED_PHYSICAL_COUNT);
}

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

/* Straight voodoo magic, consult the Inline Assembler Cookbook
   Equivalent to:
   if(*led++ > brightness)
       out |= _BV(outPin)
*/
#define LED_PIN_SET(led, outPin)                            \
    __asm__ volatile(                                       \
        "ld __tmp_reg__, %a["#led"]+                    \n\t\
         cp %[bright], __tmp_reg__                      \n\t\
         brcc skip%=                                    \n\t\
         ori %[out], (1 << "#outPin")                   \n\t\
         skip%=:"                                           \
        : [out] "+a" (out), [led] "+z" (led) /* outputs */  \
        : [bright] "r" (brightness)          /* inputs */ )

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
    static uint8_t brightness = BRIGHTNESS_LEVELS - BRIGHTNESS_INCREMENT;
    static volatile uint8_t* offset = &leds_frontbuffer[0];
    
    uint8_t out = 0;
    
    currentGnd++;
    currentGndMask >>= 1;
    if(currentGnd >= GND_COUNT) {
        currentGnd = 0;
        // Because we work backwards start at the high end and shift down
        currentGndMask = _BV(7);
        offset = &leds_frontbuffer[0];
        brightness += BRIGHTNESS_INCREMENT;
        // brightness rolls over cleanly due to being a multiple
        #if BRIGHTNESS_LEVELS != 256
        if(brightness > BRIGHTNESS_MAX)
            brightness = 0;
        #endif
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
    // Enable new ground
    GND_DDR = (GND_DDR & ~GND_MASK) | currentGndMask;
    LED_PORT |= out;
}