

#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_KEY_PRESS_EVENT      0x0C
#define KEYPAD_KEY_RELEASE_EVENT    0x1C

#define KEYPAD_KEYMAP_DEFAULT 0
#define KEYPAD_KEYMAP_DIR 1
#define KEYPAD_KEYMAP_SEL 2
#define KEYPAD_KEYMAP_CODE 3
#define KEYPAD_KEYMAP_T9 9


void keypad_enable (void);

void keypad_keymap_set (int keymap);

#endif

/* EOF */
