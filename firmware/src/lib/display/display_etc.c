/** @file display_etc.c
 * 
 * Display library for CasiOS
 * 
 * Functions related to displays that don't contain characters.
*/

#include "drivers/lcd.h"
#include "lib/display.h"
#include "lib/display/display_priv.h"


 //////////////////////////////////////////////////////////////////////////////
// Punctuation

void
display_period (signed char position)
{
    if (0 > position)
    {
        position += LCD_PERIOD_SEGMENTS + 1;
    }

    if (0 < position)
    {
        lcd_period_draw((unsigned char)position);
    }
    else
    {
        // 0 - set all periods
        for (; position <= LCD_PERIOD_SEGMENTS; position++)
        {
            lcd_period_draw((unsigned char)position);
        }
    }
    display_needs_update = 1;
}

void
display_period_clear (signed char position)
{
    if (0 > position)
    {
        position += LCD_PERIOD_SEGMENTS + 1;
    }

    if (0 < position)
    {
        lcd_period_clear((unsigned char)position);
    }
    else
    {
        // 0 - clear all periods
        for (; position <= LCD_PERIOD_SEGMENTS; position++)
        {
            lcd_period_clear((unsigned char)position);
        }
    }
    display_needs_update = 1;
}


 //////////////////////////////////////////////////////////////////////////////
// Arithmetic signs

void    display_sign                    (signed char position)
{
    if (0 > position)
    {
        position += LCD_SIGN_SEGMENTS + 1;
    }
    
    if (0 < position)
    {
        lcd_sign_draw((unsigned char)position);
    }
    else
    {
        // 0 - set all sign segments
        for (; position <= LCD_SIGN_SEGMENTS; position++)
        {
            lcd_sign_draw((unsigned char)position);
        }
    }
    display_needs_update = 1;
}

void    display_sign_clear              (signed char position)
{
    if (0 > position)
    {
        position += LCD_SIGN_SEGMENTS + 1;
    }
    
    if (0 < position)
    {
        lcd_sign_clear((unsigned char)position);
    }
    else
    {
        // 0 - clear all sign segments
        for (; position <= LCD_SIGN_SEGMENTS; position++)
        {
            lcd_sign_clear((unsigned char)position);
        }
    }
    display_needs_update = 1;
}


 //////////////////////////////////////////////////////////////////////////////
// Miscellaneous segments (Alarm indicators, AM/PM indicators)

void    display_misc                    (signed char position)
{
    if (0 > position)
    {
        position += LCD_MISC_SEGMENTS + 1;
    }
    
    if (0 < position)
    {
        lcd_misc_draw((unsigned char)position);
    }
    else
    {
        // 0 - set all misc segments
        for (; position <= LCD_MISC_SEGMENTS; position++)
        {
            lcd_misc_draw((unsigned char)position);
        }
    }
    display_needs_update = 1;
}

void    display_misc_clear              (signed char position)
{
    if (0 > position)
    {
        position += LCD_MISC_SEGMENTS + 1;
    }
    
    if (0 < position)
    {
        lcd_misc_clear((unsigned char)position);
    }
    else
    {
        // 0 - clear all misc segments
        for (; position <= LCD_MISC_SEGMENTS; position++)
        {
            lcd_misc_clear((unsigned char)position);
        }
    }
    display_needs_update = 1;
}

// EOF //
