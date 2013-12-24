#ifndef SCANCODES_H
#define SCANCODES_H

#include <defs.h>

enum kbd {

        KBD_ESC       = 0x01,

        KBD_1         = 0x02,
        KBD_2         = 0x03,
        KBD_3         = 0x04,
        KBD_4         = 0x05,
        KBD_5         = 0x06,
        KBD_6         = 0x07,
        KBD_7         = 0x08,
        KBD_8         = 0x09,
        KBD_9         = 0x0a,
        KBD_0         = 0x0b,

        KBD_UNDERSCORE_MINUS          = 0x0c,
        KBD_EQUALS_PLUS               = 0x0d,
        KBD_BACKSPACE                 = 0x0e,
        KBD_TAB                       = 0x0f,

        KBD_Q         = 0x10,
        KBD_W         = 0x11,
        KBD_E         = 0x12,
        KBD_R         = 0x13,
        KBD_T         = 0x14,
        KBD_Y         = 0x15,
        KBD_U         = 0x16,
        KBD_I         = 0x17,
        KBD_O         = 0x18,
        KBD_P         = 0x19,

        KBD_OPEN_CURL  = 0x1a,
        KBD_CLOSE_CURL = 0x1b,

        KBD_RETURN = 0x1c,
        KBD_CTRL = 0x1d,

        KBD_A = 0x1e,
        KBD_S = 0x1f,
        KBD_D = 0x20,
        KBD_F = 0x21,
        KBD_G = 0x22,
        KBD_H = 0x23,
        KBD_J = 0x24,
        KBD_K = 0x25,
        KBD_L = 0x26,

        KBD_COLON = 0x27,
        KBD_AT        = 0x28,

        KBD_TILDA     = 0x29,

        KBD_LEFTSHIFT = 0x2a,
        KBD_BACKSLASH = 0x2b,

        KBD_Z = 0x2c,
        KBD_X = 0x2d,
        KBD_C = 0x2e,
        KBD_V = 0x2f,
        KBD_B = 0x30,
        KBD_N = 0x31,
        KBD_M = 0x32,

        KBD_LT = 0x33,
        KBD_GT = 0x34,
        KBD_FORWARDSLASH = 0x35,
        KBD_RIGHTSHIFT = 0x36,
        KBD_PRINTSCREEN = 0x37,
        KBD_ALT = 0x38,
        KBD_SPACE = 0x39,
        KBD_CAPSLOCK = 0x3a,
        KBD_F1 = 0x3b,
        KBD_F2 = 0x3c,
        KBD_F3 = 0x3d,
        KBD_F4 = 0x3e,
        KBD_F5 = 0x3f,
        KBD_F6 = 0x40,
        KBD_F7 = 0x41,
        KBD_F8 = 0x42,
        KBD_F9 = 0x43,
        KBD_F10= 0x44,

        KBD_NUMLOCK= 0x45,
        KBD_SCROLLLOCK= 0x46,
        KBD_HOME= 0x47,
        KBD_UP= 0x48,
        KBD_PGUP= 0x49,
        KBD_NUMPAD_MINUS = 0x4a,
        KBD_NUMPAD_4 = 0x4b,
        KBD_NUMPAD_5 = 0x4c,
        KBD_NUMPAD_6 = 0x4d,
        KBD_NUMPAD_PLUS = 0x4e,
        KBD_NUMPAD_1 = 0x4f,
        KBD_NUMPAD_2 = 0x50,
        KBD_NUMPAD_3 = 0x51,
        KBD_NUMPAD_0 = 0x52,
        KBD_NUMPAD_PERIOD = 0xB4,

        KBD_F11= 0x57,
        KBD_F12= 0x58,

        KBD_LEFTSHIFT_RELEASE = 0xAA,
        KBD_RIGHTSHIFT_RELEASE = 0xB6,


        KBD_MAX = 0x60,
};

uint8_t kbd_tochar(uint8_t b, uint8_t shift);

#endif
