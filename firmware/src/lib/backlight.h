/** @file backlight.h
 * 
 * This library implements the functionality of the backlight on the powerpic.
 * The functionality includes:
 * - ON
 * - OFF
 * It's  a backlight, what more could you possibly want? Data transfer? ;)
*/

#ifndef _backlight_h_
#define _backlight_h_

// Predefined backlight states.
//
#define BACKLIGHT_ON        1
#define BACKLIGHT_OFF       0
#define BACKLIGHT_UNINIT    -1
#define BACKLIGHT_TOGGLE    -1

/**
 * Initialize the backlight for use.
 * This just configures the appropriate pin.
*/
void
backlight_init (void);

/**
 * Sets the backlight to a given state.
*/
void
backlight_set (signed char state);

/**
 * Gets the backlight's current state.
*/
signed char
backlight_get (void);

/**
 * Macro to turn the backlight on.
*/
#define backlight_on()     (backlight_set(BACKLIGHT_ON))

/**
 * Macro to turn the backlight off.
*/
#define backlight_off()     (backlight_set(BACKLIGHT_OFF))


#endif

// EOF //
