/** @file clock.c
 * 
 * This mode implements a clock face that displays the current time.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/datetime.h"

#include "modes/clock.h"


// Alternative mode functions for editing date/time
void clock_edit_start (void);
signed char clock_edit  (unsigned int event);

// Draw functions
void clock_draw_time (time_t *draw_time);
void clock_draw_date (date_t *draw_date);

// Edit helper functions
static void clock_draw_edit (void);
static void clock_edit_next (void);

// Needed variables
static unsigned char clock_fmt = 0; // 24/12 HR time format flag
static datetime_t now;              // Current date and time
static unsigned char date_looksie = 0;     // Flag is set when divde button is down

//
// Main Mode functions
//

void
clock_init (void)
{
    date_looksie = 0;
}

void
clock_start (void)
{
    // Get current time
    datetime_now(&now);

    // Clear display
    display_primary_clear(0);

    // Draw time
    clock_draw_time(&now.time);

    // Draw day of week
    display_secondary_string(1, datetime_weekday_str(now.date.weekday));

    // Set tickrate to 1 second
    tick_rate_set_sec(1);
}

signed char
clock_run (unsigned int event)
{
    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        // Get current time
        datetime_time_now(&now.time);

        // We update the date every hour on the hour
        if (0 == (now.time.minute | now.time.second))
        {
            datetime_today(&now.date);
        }

        // If the divide key is down, we draw the date
        if (date_looksie)
        {
            // Draw date
            clock_draw_date(&now.date);
        }
        else
        {
            // Draw time
            clock_draw_time(&now.time);
        }

    }

    if (EVENT_TYPE(event) == KEYPAD_EVENT_PRESS)
    {
        if (EVENT_DATA(event) == '/')
        {
            // Divide key shows full date
            clock_draw_date(&now.date);

            if (clock_fmt)
            {
                // Clear AM/PM sign if in 12-hour fmt
                display_misc_clear(DISPLAY_MISC_AM);
                display_misc_clear(DISPLAY_MISC_PM);
            }

            // Flag that divide key is down
            date_looksie = 1;
        }
    }
    else if (EVENT_TYPE(event) == KEYPAD_EVENT_RELEASE)
    {
        if (EVENT_DATA(event) == '/')
        {
            // Draw time on key release
            clock_draw_time(&now.time);

            // Divide key is up
            date_looksie = 0;
        }
    }

    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Switch to next mode
            return 1;
        }

        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Go into edit mode

            // Set the edit function to be our run function
            clock_mode.run = &clock_edit;

            // Start the edit procedure
            clock_edit_start();
        }
    }
    return 0;
}

void
clock_stop (void)
{
    // Clear AM/PM for next mode
    display_misc_clear(DISPLAY_MISC_AM);
    display_misc_clear(DISPLAY_MISC_PM);
}


static datetime_t edit_now;
static unsigned char edit_blink = 0;
static unsigned char edit_position = 0;

//
// Mode edit functions
//

enum {
    POS_TIME,
    // This holds the positions for every character on the time edit screen
    POS_HOUR_TENS,
    POS_HOUR_ONES,
    POS_COLON,
    POS_MIN_TENS,
    POS_MIN_ONES,
    POS_TSPACE,
    POS_SECONDS,     // Second tens and ones are 'edited'(cleared) in one block
    POS_SECONDS_ONES,

    POS_DATE,
    // This holds the positions for every character on the date edit screen
    POS_YEAR_TENS,
    POS_YEAR_ONES,
    POS_DSPACE,
    POS_MONTH_TENS,
    POS_MONTH_ONES,
    POS_DASH,
    POS_DAY_TENS,
    POS_DAY_ONES,

    POS_WEEKDAY // The weekday is represented by a string on the secondary

} EDIT_POSITIONS;



/** 
 * Start editing the clock.
*/
void
clock_edit_start (void)
{
    // Blink off the bat
    edit_blink = 1;

    // Always start editing the time seconds place
    edit_position = POS_SECONDS;

    // Get the current time
    datetime_now(&edit_now);

    // Set our edit tickrate to 1/2 a sec for blinking the current character.
    tick_rate_set_ms(500);

    // Draw time
    clock_draw_edit();
    
    // Clear seconday display of weekday
    display_secondary_clear(0);
}

/**
 * Main clock editing thread.
*/
signed char
clock_edit (unsigned int event)
{
    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        if (edit_blink)
        {
            // We are currently blinking the selected character position.

            if (edit_position == POS_WEEKDAY)
            {
                // When we are editing the weekday, flash the numerical value
                display_secondary_character(1, (edit_now.date.weekday >> 4));
                display_secondary_character(2, (edit_now.date.weekday & 0x0F));
            }
            else if (edit_position > POS_DATE)
            {
                // We need to subtract POS_DATE from the edit position to get
                // the actual display character position.
                display_primary_character((signed char)
                    (edit_position - POS_DATE), 
                    '_'
                );
            }
            else
            {
                // If we are editing time the positions work out correctly.
                display_primary_character((signed char)edit_position, '_');

                if (POS_SECONDS == edit_position)
                {
                    // Blink both second characters
                    display_primary_character(POS_SECONDS_ONES, '_');
                }
            }

            // Update display next tick
            edit_blink = 0;
        }
        else
        {
            // Update seconds
            edit_now.time.second = SECONDS;

            // Draw edited time or date
            clock_draw_edit();

            // Blink next tick
            edit_blink = 1;
        }
    }

    if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        unsigned char keypress = EVENT_DATA(event);

        if (keypress >= '0' && keypress <= '9')
        {
            // Subtracting 48 gives us a numerical value corresponding to the
            // ascii number character.
            keypress -= 48;

            // Set alarm digit based on edit_position
            switch (edit_position)
            {
                case POS_HOUR_TENS:     // Hour Tens
                    edit_now.time.hour = (unsigned char)((edit_now.time.hour & 0x0F) | (keypress << 4));
                break;
                case POS_HOUR_ONES:     // Hour Ones
                    edit_now.time.hour = ((edit_now.time.hour & 0xF0) | (keypress));
                break;

                case POS_MIN_TENS:     // Minute Tens
                    edit_now.time.minute = (unsigned char)((edit_now.time.minute & 0x0F) | (keypress << 4));
                break;
                case POS_MIN_ONES:     // Minute Ones
                    edit_now.time.minute = ((edit_now.time.minute & 0xF0) | (keypress));
                break;

                case POS_YEAR_TENS:
                    edit_now.date.year = (unsigned char)((edit_now.date.year & 0x0F) | (keypress << 4));
                break;
                case POS_YEAR_ONES:
                    edit_now.date.year = ((edit_now.date.year & 0xF0) | keypress);
                break;

                case POS_MONTH_TENS:
                    edit_now.date.month = (unsigned char)((edit_now.date.month & 0x0F) | (keypress << 4));
                break;
                case POS_MONTH_ONES:
                    edit_now.date.month = ((edit_now.date.month & 0xF0) | keypress);
                break;

                case POS_DAY_TENS:
                    edit_now.date.day = (unsigned char)((edit_now.date.day & 0x0F) | (keypress << 4));
                break;
                case POS_DAY_ONES:
                    edit_now.date.day = ((edit_now.date.day & 0xF0) | keypress);
                break;

                case POS_WEEKDAY:
                    edit_now.date.weekday = keypress;
                break;

                case POS_SECONDS:     // Seconds
                    if (0 == keypress)
                    {
                        // Clear seconds to 00 if 0 is pressed.
                        datetime_time_now(&now.time);
                        now.time.second = 0;
                        datetime_time_set(&now.time);
                    }
                break;
            }

            if (POS_SECONDS != edit_position)
            {
                // Advance to the next position,
                // except when the seconds are cleared
                clock_edit_next();
            }

            // Redraw time for every press
            clock_draw_edit();
        }

        if (keypress == '+')
        {
            // Plus sign toggles 12/24 hour fmt
            if (clock_fmt)
            {
                clock_fmt = 0;
            }
            else
            {
                clock_fmt = 1;
            }
        }
    }

    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Press of the mode button advances the edit position
            clock_edit_next();
        }

        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Press of the adj button saves the time and returns to the clock.

            // Set to 0 to signify we aren't editing any position.
            // We use this in the clock_draw_time function to determine whether
            // to draw a 0 in the tens place if the hour is < 10 AND we are in 12HR mode
            edit_position = 0;

            // Set new time with current seconds
            edit_now.time.second = SECONDS;
            datetime_set(&edit_now);

            // Set main run thread function
            clock_mode.run = &clock_run;

            // Start the clock
            clock_start();
        }
    }

    return 0;
}


//
// Draw functions
//

/**
 * Draw the given time to the display.
*/
void
clock_draw_time (time_t *draw_time)
{
    // Draw second
    display_primary_character(8, (draw_time->second & 0x0F));
    display_primary_character(7, (draw_time->second >> 4));

    // Clear space between minutes and seconds
    display_primary_clear(6);

    // Draw minute
    display_primary_character(5, (draw_time->minute & 0x0F));
    display_primary_character(4, (draw_time->minute >> 4));

    // Colon
    display_period(DISPLAY_PERIOD_COLON);

    // Draw hour
    if (clock_fmt)
    {
        // 12-hour format

        if (0x00 == draw_time->hour)
        {
            // Midnght - 12:00 AM
            display_primary_character(1, 1);
            display_primary_character(2, 2);
            display_misc(DISPLAY_MISC_AM);
            display_misc_clear(DISPLAY_MISC_PM);
        }
        else if (0x12 == draw_time->hour)
        {
            // Noon - 12:00 PM
            display_primary_character(1, 1);
            display_primary_character(2, 2);
            display_misc(DISPLAY_MISC_PM);
            display_misc_clear(DISPLAY_MISC_AM);
        }
        else if (0x12 < draw_time->hour)
        {
            // PM
            if ((draw_time->hour >> 4) - 1 || edit_position)
            {
                // Only display digits > 0 OR if we are editing a position
                display_primary_character(1, ((draw_time->hour >> 4) - 1));
            }
            else
            {
                // Clear digit instead of displaying 0
                display_primary_clear(1);
            }
            display_primary_character(2, ((draw_time->hour & 0x0F) - 2));
            display_misc(DISPLAY_MISC_PM);
            display_misc_clear(DISPLAY_MISC_AM);
        }
        else
        {
            // AM
            if ((draw_time->hour >> 4) || edit_position)
            {
                // Only display digits > 0 OR if we are editing a position
                display_primary_character(1, (draw_time->hour >> 4));
            }
            else
            {
                // Clear digit instead of displaying 0
                display_primary_clear(1);
            }
            display_primary_character(2, (draw_time->hour & 0x0F));
            display_misc(DISPLAY_MISC_AM);
            display_misc_clear(DISPLAY_MISC_PM);
        }
    }
    else
    {
        // 24-hour format
        display_primary_character(2, (draw_time->hour & 0x0F));
        display_primary_character(1, (draw_time->hour >> 4));

        // Clear AM/PM
        display_misc_clear(DISPLAY_MISC_AM);
        display_misc_clear(DISPLAY_MISC_PM);
    }
}


/**
 * Draw the given date to the display.
*/
void
clock_draw_date (date_t *draw_date)
{
    // Draw year (20XX)
    display_primary_character(1, (draw_date->year >> 4));
    display_primary_character(2, (draw_date->year & 0x0F));

    // Clear space between year and month
    display_primary_clear(3);
    // Also clear colon in case a time was being displayed
    display_period_clear(DISPLAY_PERIOD_COLON);

    // Draw month
    display_primary_character(4, (draw_date->month >> 4));
    display_primary_character(5, (draw_date->month & 0x0F));

    // Draw dash between month and day
    display_primary_character(6, '-');

    // Draw day
    display_primary_character(7, (draw_date->day >> 4));
    display_primary_character(8, (draw_date->day & 0x0F));

    // Draw weekday
    display_secondary_string(1, datetime_weekday_str(draw_date->weekday));
}


//
// Edit helper functions
//

/**
 * Draw the time or date that's being edited.
*/
static void
clock_draw_edit (void)
{
    if (edit_position > POS_DATE)
    {
        // We're editing the date.
        clock_draw_date(&edit_now.date);
    }
    else
    {
        // We're editing the time.
        clock_draw_time(&edit_now.time);
    }
}


/**
 * Advance the edit position to the next position
*/
static void
clock_edit_next (void)
{
    // Press of the mode button advances the edit position
    switch (edit_position)
    {
        // All tens positions are immediatly followed by a ones position
        // The weekday also immediatly follows the day ones.
        case POS_HOUR_TENS:
        case POS_MIN_TENS:
        case POS_YEAR_TENS:
        case POS_MONTH_TENS:
        case POS_DAY_TENS:
        case POS_DAY_ONES:
            edit_position++;
        break;

        // A subset of the ones places just need to skip a character position
        case POS_HOUR_ONES:     // Skip the colon position
        case POS_YEAR_ONES:     // Skip the space after the year
        case POS_MONTH_ONES:    // Skip the dash between month-day
            edit_position += 2;
        break;

        // Start editing the date after the minute
        case POS_MIN_ONES:
            edit_position = POS_YEAR_TENS;
        break;

        // Skip to seconds after the weekday
        case POS_WEEKDAY:
            edit_position = POS_SECONDS;

            // Clear weekday
            // TODO: This isnt really a good spot for this, but it works.
            display_secondary_clear(0);
        break;

        // Seconds is the last position and wraps back to the hour
        case POS_SECONDS:
            edit_position = POS_HOUR_TENS;
        break;
    }
}


// EOF //