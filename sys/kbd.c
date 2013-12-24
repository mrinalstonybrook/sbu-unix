#include <defs.h>
#include <stdio.h>
#include "sys/scancodes.h"
#include "sys/kbd.h"


static uint8_t char_shift_adjust(uint8_t c,int shift) {
	return c + (shift?('A'-'a'):0);
}

	uint8_t scancode_to_char(unsigned char scancode,int shift){
		if(scancode==KBD_SPACE)
			return ' ';

		if(scancode== KBD_RETURN)
			return '\n';

		if(scancode== KBD_FORWARDSLASH)
			return '/';

		if(scancode== KBD_NUMPAD_PERIOD)
			return '.';

		if((scancode >=  KBD_Q) && (scancode <=  KBD_P))
			return char_shift_adjust("qwertyuiop"[scancode- KBD_Q],shift);

		if((scancode >=  KBD_A) && (scancode <=  KBD_L))
			return char_shift_adjust("asdfghjkl"[scancode- KBD_A],shift);

		if((scancode >=  KBD_Z) && (scancode <=  KBD_M))
			return char_shift_adjust("zxcvbnm"[scancode- KBD_Z],shift);

		if((scancode >=  KBD_1) && (scancode <=  KBD_9)) {
			return '1' + scancode- KBD_1;
		}

		return 0;
	}
