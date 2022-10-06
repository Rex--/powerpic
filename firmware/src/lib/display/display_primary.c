/** @file display_primary.c
 * 
 * Display library for PowerOS
 * 
 * Functions related to the primary display.
*/

#include "drivers/lcd.h"
#include "lib/display.h"
#include "lib/display/display_priv.h"


 //////////////////////////////////////////////////////////////////////////////
// Primary

void
display_primary_segments (signed char position, unsigned char segments)
{
    position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

    for (char bit = 0; bit < LCD_PRIMARY_CHARACTER_SEGMENTS; bit++)
    {
        if ((1U << bit) & segments)
        {
            lcd_primary_draw((unsigned char)position, bit);
        }
        else
        {
            lcd_primary_clear((unsigned char)position, bit);
        }
    }

    display_needs_update = 1;
}

void
display_primary_character (signed char position, unsigned char character)
{
    // Position will get normalized in the call to display_primary_segments
    // position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

    character = character_normalize(character);

    unsigned char segments = display_font[character];

    if (position)
    {
        display_primary_segments(position, segments);
    }
    else
    {
        // If position is zero we set all characters
        for (position = 1; position <= LCD_PRIMARY_CHARACTERS; position++)
        {
            display_primary_segments(position, segments);
        }
    }
}

void
display_primary_string (signed char position, const char *string)
{
    // LOG_DEBUG("String: %.8s", string);

    // Get the length of the string.
    //
    signed char string_len = 0;
    while (string[string_len])
    {
        string_len++;
    }

    // Right justify negative positions.
    //
    if (0 > position)
    {
        position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

        // Set pointer to the last char in the string
        //
        string += string_len - 1;

        // When right justifying, position is also the number of characters to
        // the right of the justification.
        //
        if (string_len > position)
        {
            string_len = 0;
        }
        else
        {
            string_len = position - string_len;
        }

        while (position > string_len)
        {
            display_primary_character(position--, *string--);
        }
    }

    // Left justify positive positions.
    //
    else if (0 < position)
    {
        while (position <= LCD_PRIMARY_CHARACTERS && *string)
        {
            display_primary_character(position++, *string++);
        }
    }

    // Center position 0
    else
    {
        position = ( (LCD_PRIMARY_CHARACTERS / 2) + 1 ) - (string_len / 2);

        if (1 < position)
        {
            if (string_len % 2)
            {
                position--;
            }
        }
        else
        {
            string += (string_len - LCD_PRIMARY_CHARACTERS) / 2;
            position = 1;
        }

        while (position <= LCD_PRIMARY_CHARACTERS && *string)
        {
            display_primary_character(position++, *string++);
        }
    }
}


/**
 * Display a number on the primary display.
 * 
 * This function displays any number that will fit in the 8 digits available on
 * the primary display. This gives a range of -9,999,999 to 99,999,999. If the
 * given number is outside of this range, 'Er' will be displayed at position.
 * The given position will always be the ones place of the number. Negative
 * positions map from the right, positive from the left; both starting with 1.
 * 
 * @param[in]   position    The postion of the start of the number.
 * @param[in]   number      The number to display.
*/
void
display_primary_number (signed char position, long number)
{

    // LOG_DEBUG("Number: %li", number);

    if (0 == number)
    {
        // Special case for 0
        display_primary_character(position, 0);
    }
    else if (-9999999 > number || number > 99999999)
    {
        // Number is outside of displayable range.
        // Display error for now.
        display_primary_string(position, "Er");
    }
    else
    {
        position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

        // Convert negative to positive numbers, and indicate that it is a
        // negative. We use this to draw the '-' sign in front.
        unsigned char negative_number = 0;
        if (0 > number)
        {
            negative_number = 1;
            number = -number;
        }

        // Draw the absolute value of the number to the display.
        unsigned char number_char = 0;
        while (0 < number && 0 < position)
        {
            number_char = number % 10;

            display_primary_character(position--, number_char);

            number /= 10;
        }

        // Draw our '-' sign if necessary.
        if (negative_number)
        {
            // We should always have a postion leftover for the negative sign.
            display_primary_character(position, '-');
        }
    }
}


/**
 * Display a hexadecimal number.
 * 
 * @param[in]   position    Position of least significant digit.
 * @param[in]   number      Number to display.
*/
void
display_primary_hex (signed char position, unsigned long number)
{
    if (0 == number)
    {
        // Special case for 0
        display_primary_character(position, 0);
    }
    else
    {
        position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

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

            display_primary_character(position--, number_char);

            number /= 16;
        }
    }
}

void
display_primary_clear (signed char position)
{
    // char seg = 0;

    // position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

    // Clear character
    if (position)
    {
        display_primary_segments(position, 0);
        // for (seg = 0; seg < LCD_PRIMARY_CHARACTER_SEGMENTS; seg++)
        // {
        //     lcd_primary_clear((unsigned char)position, seg);
        // }
    }

    // 0 clears whole primary display.
    else
    {
        for (position = 1; position <= LCD_PRIMARY_CHARACTERS; position++)
        {
            display_primary_segments(position, 0);
            // for (seg = 0; seg < LCD_PRIMARY_CHARACTER_SEGMENTS; seg++)
            // {
            //     lcd_primary_clear((unsigned char)position, seg);
            // }
        }
    }
    // display_needs_update = 1;
}

// EOF //
