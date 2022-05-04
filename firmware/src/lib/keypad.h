

#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_KEYMAP_DEFAULT 0
#define KEYPAD_KEYMAP_DIR 1
#define KEYPAD_KEYMAP_SEL 2
#define KEYPAD_KEYMAP_CODE 3
#define KEYPAD_KEYMAP_T9 9

void keypad_enable (void);

void keypad_keymap_set (int keymap);

int keypad_pressed (void);

void keypad_scan (void);

int keypad_check (void);

#endif
