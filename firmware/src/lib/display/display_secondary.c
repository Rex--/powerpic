/** @file display_secondary.c
 * 
 * Display library for CasiOS
 * 
 * Functions related to the secondary display.
*/

#include "drivers/lcd.h"
#include "lib/display.h"
#include "lib/display/display_priv.h"


 //////////////////////////////////////////////////////////////////////////////
// Secondary

void
display_secondary_segments (signed char position, unsigned int segments)
{
    position = position_normalize(position, LCD_SECONDARY_CHARACTERS);

    // Draw 10 secondary segments
    for (char bit = 0; bit < LCD_SECONDARY_CHARACTER_SEGMENTS; bit++)
    {
        if ((1U << bit) & segments)
        {
            lcd_secondary_draw((unsigned char)position, bit);
        }
        else
        {
            lcd_secondary_clear((unsigned char)position, bit);
        }
    }

    display_needs_update = 1;
}

void
display_secondary_character (signed char position, unsigned char character)
{
    character = character_normalize(character);
    unsigned char segments = display_font[character];

    if (position)
    {
        // We currently don't use the extra segments for displaying characters
        // so we cast our (char) character segments as ints.
        display_secondary_segments(position, (unsigned int)segments);
    }
    else
    {
        // If position is zero we set all characters
        for (position = 1; position <= LCD_SECONDARY_CHARACTERS; position++)
        {
            display_secondary_segments(position, (unsigned int)segments);
        }
    }
}

void
display_secondary_string (signed char position, const char *string)
{
    // Right justify negative positions.
    //
    if (0 > position)
    {
        position += LCD_SECONDARY_CHARACTERS + 1;

        // Set pointer to the last char in the string
        //
        signed char string_len = 0;
        while (*string++)
        {
            string_len++;
        }
        //Null byte
        string--;
        string_len--;

        if (string_len > position)
        {
            string_len = 1;
        }
        else
        {
            string_len = position - string_len;
        }

        while (position >= string_len && position > 0)
        {
            display_secondary_character(position--, *--string);
        }
    }

    // Left justify positive positions.
    //
    else if (0 < position)
    {
        while (position <= LCD_SECONDARY_CHARACTERS && *string)
        {
            display_secondary_character(position++, *string++);
        }
    }

    // TODO: 0
    else
    {

    }
}

void
display_secondary_number (signed char position, int number)
{
    position = position_normalize(position, LCD_SECONDARY_CHARACTERS);
    
    if (0 < number)
    {
        if (200 > number)
        {
            if (100 == number)
            {
                display_secondary_character(2, 0);
                display_secondary_character(1, 0);
                lcd_secondary_draw(1, 9);
            }
            else if (100 < number)
            {
                number -= 100;
                display_secondary_character(2, (unsigned char)(number % 10));
                display_secondary_character(1, (unsigned char)(number / 10));
                lcd_secondary_draw(1, 9);
            }
            else
            {
                display_secondary_character(2, number % 10);
                if (9 < number)
                {
                    display_secondary_character(1, (unsigned char)(number / 10));
                }
            }
        }
        else
        {
            display_secondary_character(1, 'E');
            display_secondary_character(2, 'r');
        }
    }
    else if (0 > number)
    {
        number = -number;
        if (10 > number)
        {
            display_secondary_character(1, '-');
            display_secondary_character(2, (unsigned char)number);
        }
        else
        {
            display_secondary_character(1, 'E');
            display_secondary_character(2, 'r');
        }
    }
    else
    {
        display_secondary_character(position, 0);
    }
}


/**
 * Display a hexadecimal number.
 * 
 * @param[in]   position    Position of least significant digit.
 * @param[in]   number      Number to display.
*/
void
display_secondary_hex (signed char position, unsigned char number)
{
    if (0 == number)
    {
        // Special case for 0
        display_secondary_character(position, 0);
    }
    else
    {
        position = position_normalize(position, LCD_SECONDARY_CHARACTERS);

        // Draw each digit as an ascii character.
        unsigned char number_char = 0;
        while (0 < number && 0 < position)
        {
            number_char = number % 16;

            if (number_char > 9)
            {
                switch (number_char)
                {
                    case 10:
                        number_char = 'A';
                    break;
                    case 11:
                        number_char = 'b';
                    break;
                    case 12:
                        number_char = 'C';
                    break;
                    case 13:
                        number_char = 'd';
                    break;
                    case 14:
                        number_char = 'E';
                    break;
                    case 15:
                        number_char = 'F';
                    break;

                    default:
                    break;
                }
            }

            display_secondary_character(position--, number_char);

            number /= 16;
        }
    }
}


void
display_secondary_clear (signed char position)
{
    // If the position is not zero, clear only that position.
    if (position)
    {
        display_secondary_segments(position, 0);
    }

    // If the position is 0, clear the entire display
    else
    {
        for (position = 1; position < 3; position++)
        {
            display_secondary_segments(position, 0);
        }
    }
}

// EOF //
