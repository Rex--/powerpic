

#ifndef KEYPAD_KEYMAPS_H
#define KEYPAD_KEYMAPS_H

// Some useful keymaps that can be overlayed over the default keycodes.
//
#define KEYPAD_KEYMAP_CODE 0
#define KEYPAD_KEYMAP_CASIO 1
#define KEYPAD_KEYMAP_DIR 2
#define KEYPAD_KEYMAP_SEL 3


/** Casio key mapping.
 * This is the keymapping that is labeled on the watch.
*/
const char keypad_keymap_casio[16] = {
    '7', '8', '9', '/',
    '4', '5', '6', '*',
    '1', '2', '3', '-',
    '0', '.', '=', '+',
};

/** Directional key mapping.
 * This separates the keypad into 4 arrow keys.
*/
const char keypad_keymap_directional[16] = {
    '<', '^', '^', '>',
    '<', '^', '^', '>',
    '<', 'v', 'v', '>',
    '<', 'v', 'v', '>',
};

/** Selection key mapping.
 * This separates the keypad into 4 arrow keys and a center button.
 * Currently the corner buttons are + and -.
*/
const char keypad_keymap_selection[16] = {
    '+', '^', '^', '+',
    '<', '=', '=', '>',
    '<', '=', '=', '>',
    '-', 'v', 'v', '-',
};



const char *keypad_keymaps[] = {
    NULL,                       // Placeholder for emitting raw keycodes
    keypad_keymap_casio,        // Casio keymap, this is printed on the keypad
    keypad_keymap_directional,  // 4 Arrow Keys
    keypad_keymap_selection,    // 4 Arrow Keys, center select, pg up/dn corners
};

#endif