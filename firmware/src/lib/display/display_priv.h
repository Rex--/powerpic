/** @file display_priv.h
 * 
 * Display library for CasiOS
 * 
 * Private functions for use internally by the display lib.
*/
#ifndef _display_priv_h_
#define _display_priv_h_

/** Flag to determine if the display has changed. */
extern volatile unsigned char display_needs_update;

/** Currently configured font to use for displaying characters.*/
extern const unsigned char * volatile display_font;

#if LOG_LVL >= LOG_LVL_INFO

// The debug display is enabled for every log level
#define DISPLAY_DEBUG

/** The debug display holds the content of the primary display as ascii characters. */
extern volatile unsigned char debug_display[8];

#endif

/**
 * Map negative positions to absolute.
 * This does not find the absolute value, it considers negatives to mean
 * "from the end". e.g. a -1 position with a display_len of 8 will return 8
 * because it is the first position from the end.
 * 
 * @param[in]   position        Position to normalize.
 * @param[in]   display_len     Length of display to normalize position on.
 * 
 * @returns     Normalized position
*/
signed char
position_normalize (signed char position, unsigned char display_len);

/**
 * Map supported characters to font characters.
 * This handles numbers 0-9, plus standard printable ascii characters.
 * Valid font character values are 0-9, and 32-126.
 * 
 * @param[in]   character       Character to map to font.
 * 
 * @returns     The index of the font representation of the character.
*/
unsigned char
character_normalize (unsigned char character);

#endif

// EOF //
