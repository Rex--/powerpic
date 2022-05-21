/** @file display.c
*/

#include <xc.h>
#include <stdio.h>

#include "dev_config.h"

#include "drivers/lcd.h"
#include "lib/display_font.h"

#include "lib/display.h"



#if DEV_BUILD
#   define DISPLAY_DEBUG_PRINT
#endif

/**
 * Offset of ascii characters to our font array index.
 * The characters are in the same order as ascii, starting with 32 or a space (" ").
*/
#define DISPLAY_CHARACTER_ASCII_OFFSET      32


/** Flag to determine if the lcd segments have changed. */
static unsigned char lcd_needs_update = 0;


/** Currently installed font. */
static const unsigned char *current_font = NULL;

/**
 * Enables the display.
 * The LCD driver is initialized and enabled, default state initialized.
*/
void
display_enable (void)
{
    lcd_init();
    lcd_needs_update = 0;
    current_font = display_fonts[0];
}

void
display_update (void)
{
    if (lcd_needs_update)
    {
        lcd_update();
        lcd_needs_update = 0;
    }
}

void
display_set_font (unsigned char font)
{
    current_font = display_fonts[font];
}

// Map negative positions to absolute
inline static signed char
position_normalize (signed char position, unsigned char display_len)
{
    // Normalize negative positions to absolute positions.
    //
    if (0 > position)
    {
        position += display_len + 1;
    }

    return position;
}

// Map supported characters to font characters
inline static unsigned char
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

typedef void (*lcd_func_t) (unsigned char position, unsigned char segment);

inline static void
segments_draw (lcd_func_t draw_func, lcd_func_t clear_func, unsigned char position, unsigned char segments)
{
    for (char bit = 0; bit < LCD_PRIMARY_CHARACTER_SEGMENTS; bit++)
    {
        if ((1U << bit) & segments)
        {
            draw_func(position, bit);
        }
        else
        {
            clear_func(position, bit);
        }
    }
}



void
display_primary_character (signed char position, unsigned char character)
{
    position = position_normalize(position, LCD_PRIMARY_CHARACTERS);

    character = character_normalize(character);

    unsigned char segments = current_font[character];

    if (position)
    {
        segments_draw(&lcd_primary_draw, &lcd_primary_clear, (unsigned char)position, segments);
    }
    else
    {
        // If position is zero we set all characters
        for (position = 1; position <= LCD_PRIMARY_CHARACTERS; position++)
        {
            segments_draw(&lcd_primary_draw, &lcd_primary_clear, (unsigned char) position, segments);
        }
    }
    lcd_needs_update = 1;
}

void
display_primary_string (signed char position, const char *string)
{
#   ifdef DISPLAY_DEBUG_PRINT
    printf("Display: %s\n\r", string);
#   endif
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

        // When right justfying, position is also the number of characters to
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

void
display_primary_number (signed char position, long number)
{
    // Fuck you
    position = LCD_PRIMARY_CHARACTERS;

    if (0 == number)
    {
        display_primary_character(position, 0);
    }
    else
    {
        unsigned char number_char = 0;
        while (0 < number && 0 < position)
        {
            number_char = number % 10;

            display_primary_character(position--, number_char);

            number /= 10;
        }
    }
    lcd_needs_update = 1;
}

void
display_primary_clear (signed char position)
{
    char seg = 0;

    // Clear character
    if (0 < position)
    {
        for (seg = 0; seg < LCD_PRIMARY_CHARACTER_SEGMENTS; seg++)
        {
            lcd_primary_clear((unsigned char)position, seg);
        }
    }

    // 0 or less clears whole primary display.
    else
    {
        for (position = 1; position <= LCD_PRIMARY_CHARACTERS; position++)
        {
            for (seg = 0; seg < LCD_PRIMARY_CHARACTER_SEGMENTS; seg++)
            {
                lcd_primary_clear((unsigned char)position, seg);
            }
        }
    }
    lcd_needs_update = 1;
}


void
display_secondary_character (signed char position, unsigned char character)
{
    position = position_normalize(position, LCD_SECONDARY_CHARACTERS);

    character = character_normalize(character);

    unsigned char segments = current_font[character];

    if (position)
    {
        segments_draw(&lcd_secondary_draw, &lcd_secondary_clear, (unsigned char)position, segments);
    }
    else
    {
        // If position is zero we set all characters
        for (position = 1; position <= LCD_SECONDARY_CHARACTERS; position++)
        {
            segments_draw(&lcd_secondary_draw, &lcd_secondary_clear, (unsigned char)position, segments);
        }
    }
    lcd_needs_update = 1;
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
display_secondary_number        (signed char position, int number)
{
    // Fuck you
    position = LCD_SECONDARY_CHARACTERS;

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
    lcd_needs_update = 1;
}

void
display_secondary_clear (signed char position)
{
    char seg = 0;
    if (0 < position)
    {
        for (seg = 0; seg < LCD_SECONDARY_CHARACTER_SEGMENTS; seg++)
        {
            lcd_secondary_clear((unsigned char)position, seg);
        }
    }
    else
    {
        for (position = 0; position < 2; position++)
        {
            for (seg = 0; seg < LCD_SECONDARY_CHARACTER_SEGMENTS; seg++)
            {
                lcd_secondary_clear((unsigned char)position, seg);
            }
        }
    }
    lcd_needs_update = 1;
}

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
    lcd_needs_update = 1;
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
    lcd_needs_update = 1;
}

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
    lcd_needs_update = 1;
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
        // 0 - set all sign segments
        for (; position <= LCD_SIGN_SEGMENTS; position++)
        {
            lcd_sign_clear((unsigned char)position);
        }
    }
    lcd_needs_update = 1;
}

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
        // 0 - set all sign segments
        for (; position <= LCD_MISC_SEGMENTS; position++)
        {
            lcd_misc_draw((unsigned char)position);
        }
    }
    lcd_needs_update = 1;
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
        // 0 - set all sign segments
        for (; position <= LCD_MISC_SEGMENTS; position++)
        {
            lcd_misc_clear((unsigned char)position);
        }
    }
    lcd_needs_update = 1;
}


void
display_segments_zero (void)
{
    lcd_segments_clear();
}

/*** EOF ***/
