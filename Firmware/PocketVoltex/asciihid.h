// https://github.com/SFE-Chris/UNO-HIDKeyboard-Library/blob/master/HIDKeyboard.h
#ifndef ASCIIHID_H_
#define ASCIIHID_H_

#include <avr/pgmspace.h>

// HID Values of Function Keys
#define F1 0x3a
#define F2 0x3b
#define F3 0x3c
#define F4 0x3d
#define F5 0x3e
#define F6 0x3f
#define F7 0x40
#define F8 0x41
#define F9 0x42
#define F10 0x43
#define F11 0x44
#define F12 0x45

// HID Values of Special Keys
#define ENTER 0x28
#define ESCAPE 0x29
#define BACKSPACE 0x2a
#define TAB 0x2b
#define SPACEBAR 0x2c
#define CAPSLOCK 0x39
#define PRINTSCREEN 0x46
#define SCROLLLOCK 0x47
#define PAUSE 0x48
#define INSERT 0x49
#define HOME 0x4a
#define PAGEUP 0x4b
#define DELETE 0x4c
#define END 0x4d
#define PAGEDOWN 0x4e
#define RIGHTARROW 0x4f
#define LEFTARROW 0x50 
#define DOWNARROW 0x51
#define UPARROW 0x52

// HID Values of Keypad Keys
#define NUMLOCK 0x53
#define KEYPADSLASH 0x54
#define KEYPADSTAR 0x55
#define KEYPADMINUS 0x56
#define KEYPADPLUS 0x57
#define KEYPADENTER 0x58
#define KEYPAD1 0x59
#define KEYPAD2 0x5a
#define KEYPAD3 0x5b
#define KEYPAD4 0x5c
#define KEYPAD5 0x5d
#define KEYPAD6 0x5e
#define KEYPAD7 0x5f
#define KEYPAD8 0x60
#define KEYPAD9 0x61
#define KEYPAD0 0x62
#define KEYPADPERIOD 0x63

// HID Values of System Keys
#define KEYBOARDAPPLICATION 0x65
#define KEYBOARDPOWER 0x66
#define VOLUMEMUTE 0x7f
#define VOLUMEUP 0x80
#define VOLUMEDOWN 0x81

// Common-use modifiers
#define CTRL 0x01
#define SHIFT 0x02
#define ALT 0x04
#define GUI 0x08

extern PROGMEM const uint8_t HIDTable[];
extern PROGMEM const uint8_t modifierTable[];
extern PROGMEM const uint8_t numpadTable[];

#endif