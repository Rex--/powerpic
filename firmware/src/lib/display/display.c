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

#ifdef DISPLAY_DEBUG

volatile unsigned char debug_display[8];

#endif

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

#   ifdef DISPLAY_DEBUG

    // Initialize the debug display to blank spaces
    for (char i = 0; i < 9; i++)
    {
        debug_display[i] = ' ';
    }

#   endif
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

#       ifdef DISPLAY_DEBUG
        
        LOG_INFO("%c%c%c%c%c%c%c%c",
            debug_display[0],
            debug_display[1],
            debug_display[2],
            debug_display[3],
            debug_display[4],
            debug_display[5],
            debug_display[6],
            debug_display[7]);
        
#       endif

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
