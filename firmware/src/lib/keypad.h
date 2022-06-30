/** @file keypad.h
 * 
 * This library implements the 4x4 16-key keypad on the watch.
 * 
 * Each key generates events on press and release.
 * The keycode each key generates is configurable via the 'keymap' mode_config
 * option.
 * 
 * TODO: No debouncing is implemented for the button presses.
*/

#ifndef _keypad_h_
#define _keypad_h_

// Include keymaps header for keymap index enum
#include "lib/keypad_keymaps.h"

// Specific keypad events
//
#define KEYPAD_EVENT_PRESS      0x0C
#define KEYPAD_EVENT_RELEASE    0x1C

/**
 * Initialize the keypad.
 * Events will be generated for every press and release event.
*/
void
keypad_init (void);

/**
 * Set the keymap.
 * This configures the keycodes that are emitted by the keypad on each button
 * event.
*/
void
keypad_keymap_set (unsigned char keymap);

/**
 * Get the current keymap.
 * Returns the keymap that is currently being used.
*/
unsigned char
keypad_keymap_get (void);

#endif

// EOF //
