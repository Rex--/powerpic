
#ifndef KEYPAD_H
#define KEYPAD_H
#include "lib/keypad_keymaps.h"


#define KEYPAD_KEY_PRESS_EVENT      0x0C
#define KEYPAD_KEY_RELEASE_EVENT    0x1C

void keypad_enable (void);

void keypad_keymap_set (unsigned char keymap);

#endif

/* EOF */
