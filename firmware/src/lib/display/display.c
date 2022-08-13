/** @file display.c
 * 
 * Display library for PowerOS.
 * 
 * This file holds generic display functions that apply to all displays.
*/

#include <xc.h>

#include "drivers/lcd.h"

#include "lib/display.h"
#include "lib/display/display_priv.h"
#include "lib/display/fonts.h"

#define LOG_TAG "lib.display"
#include "lib/logging.h"


volatile unsigned char display_needs_update = 0;

const unsigned char * volatile display_font;

/**
 * Initialize the display.
 * This initializes the LCD driver and configures the default state.
*/
void
display_init (void)
{
    lcd_init();

    display_needs_update = 0;

    // Configure our default font.
    display_font = default_font;
}

/**
 * Update the display if needed.
 * If the lcd segment buffers have been edited this calls to update the lcd.
*/
void
display_update (void)
{
    if (display_needs_update)
    {
        lcd_update();
        display_needs_update = 0;
    }
}

signed char
position_normalize (signed char position, unsigned char display_len)
{
    if (0 > position)
    {
        position += display_len + 1;
    }

    return position;
}

unsigned char
character_normalize (unsigned char character)
{
    // Handle numbers 0-9
    //
    if (character <= 9)
    {
        // Map values to their respective ascii representation.
        //
        character += ('0' - DISPLAY_CHARACTER_ASCII_OFFSET);
    }
    else
    {
        // Assume it's a displayable ascii character.
        //
        character -= DISPLAY_CHARACTER_ASCII_OFFSET;
    }
    return character;
}

void
display_segments_zero (void)
{
    lcd_segments_clear();
}

// EOF //
