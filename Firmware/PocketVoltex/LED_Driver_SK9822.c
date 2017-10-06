#define R 0
#define G 1
#define B 2

#define SK9822_BRIGHTNESS 1

static const uint8_t SK9822_map[] = {0, 2, 4, 6, 7, 5, 3, 1};

void led_init(void) {
    // SCLK/MOSI PB1/2
    DDRB |= _BV(1)|_BV(2);
    
    // SPI enabled, master mode, CLK/4 speed
    SPCR = _BV(SPE)|_BV(MSTR);
    
    // normal speed mode
    SPSR = 0;
}

// busy wait
void SPI_write(uint8_t val) {
    cli();
	SPDR = val;
	while(!(SPSR & _BV(SPIF)))
        ;
    SPSR = _BV(SPIF); // clear flag
    sei();
}

void led_commit(void) {
  SPI_write(0x00);  // Start Frame 
  SPI_write(0x00);
  SPI_write(0x00);
  SPI_write(0x00);
 
  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    SPI_write(0xe0|SK9822_BRIGHTNESS);  // Maximum global brightness
    uint8_t offset = SK9822_map[i] * 3;
    SPI_write(leds[offset+B]);
    SPI_write(leds[offset+G]);
    SPI_write(leds[offset+R]);
  }

  // Reset frame - Only needed for SK9822, has no effect on APA102
  SPI_write(0x00);  
  SPI_write(0x00);
  SPI_write(0x00);
  SPI_write(0x00);
  // End frame - 1 for every 16 LEDs
  SPI_write(0x00);
}