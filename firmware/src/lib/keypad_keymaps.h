/** @file keypad_keymaps.h
 * 
 * This file contains some useful keymaps that can be overlayed on top of the
 * default keycodes.
 * Keymaps are configurable in mode applications by modifying the 'keymap'
 * attribute of a mode_config object.
*/

#ifndef KEYPAD_KEYMAPS_H
#define KEYPAD_KEYMAPS_H

/**
 * Defined keymaps.
 * This enum provides an easy way of specifying keymaps.
*/
enum keypad_keymaps_index {
    KEYMAP_CODES,
    KEYMAP_CASIO,
    KEYMAP_DIRECTIONAL,
    KEYMAP_SELECTION,
    KEYMAP_MAX_KEYMAPS
};

/**
 * Casio key mapping.
 * This is the keymapping that is labeled on the watch.
*/
const char keypad_keymap_casio[16] = {
    '7', '8', '9', '/',
    '4', '5', '6', '*',
    '1', '2', '3', '-',
    '0', '.', '=', '+',
};

/**
 * Directional key mapping.
 * This separates the keypad into 4 arrow keys.
*/
const char keypad_keymap_directional[16] = {
    '<', '^', '^', '>',
    '<', '^', '^', '>',
    '<', 'v', 'v', '>',
    '<', 'v', 'v', '>',
};

/**
 * Selection key mapping.
 * This separates the keypad into 4 arrow keys and a center button.
 * Currently the corner buttons are + and -.
*/
const char keypad_keymap_selection[16] = {
    '+', '^', '^', '+',
    '<', '=', '=', '>',
    '<', '=', '=', '>',
    '-', 'v', 'v', '-',
};

/** This array holds a pointer to every implemented keymap. */
const char *keypad_keymaps[KEYMAP_MAX_KEYMAPS] = {
    NULL,                       // Placeholder for emitting raw keycodes
    keypad_keymap_casio,        // Casio keymap, this is printed on the keypad
    keypad_keymap_directional,  // 4 Arrow Keys
    keypad_keymap_selection,    // 4 Arrow Keys, center select, pg up/dn corners
};

#endif