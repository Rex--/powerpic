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


static unsigned char clock_fmt = 0;

static datetime_t now;

// Draw the time in the 'now' datetime_t object
void clock_draw_now (void);

void clock_edit_start (void);
void clock_draw_edit (void);

void
clock_init (void)
{

}

void
clock_start (void)
{
    // Get current time
    datetime_now(&now);

    // Clear display
    display_primary_clear(0);

    // Draw time
    clock_draw_now();

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

        // Draw time
        clock_draw_now();

        // We update the date every hour on the hour
        if (0 == (now.time.minute | now.time.second))
        {
            datetime_today(&now.date);
        }
    }

    if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        if (EVENT_DATA(event) == '/')
        {
            // Divide key shows full date
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

void
clock_edit_start (void)
{
    edit_blink = 1;
    edit_position = 1;

    // Get the current time
    datetime_now(&edit_now);

    // Set our edit tickrate to 1/2 a sec for blinking the current character.
    tick_rate_set_ms(500);

    // Draw time
    clock_draw_edit();
}

signed char
clock_edit (unsigned int event)
{
    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        if (edit_blink)
        {
            // We are currently blinking the selected character position.
            display_primary_character((signed char)edit_position, '_');
            if (7 == edit_position)
            {
                // Blink both second characters
                display_primary_character(8, '_');
            }

            // Update seconds next tick
            edit_blink = 0;
        }
        else
        {
            // Draw edited time
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
            keypress -= 48;

            // Set alarm digit based on edit_position
            switch (edit_position)
            {
                case 1:     // Hour Tens
                    edit_now.time.hour = (unsigned char)((edit_now.time.hour & 0x0F) | (keypress << 4));
                    edit_position = 2;
                break;

                case 2:     // Hour Ones
                    edit_now.time.hour = ((edit_now.time.hour & 0xF0) | (keypress));
                    edit_position = 4;
                break;

                case 4:     // Minute Tens
                    edit_now.time.minute = (unsigned char)((edit_now.time.minute & 0x0F) | (keypress << 4));
                    edit_position = 5;
                break;

                case 5:     // Minute Ones
                    edit_now.time.minute = ((edit_now.time.minute & 0xF0) | (keypress));
                    edit_position = 7;
                break;

                case 7:     // Seconds
                    if (0 == keypress)
                    {
                        // Clear seconds to 00 if 0 is pressed.
                        datetime_time_now(&now.time);
                        now.time.second = 0;
                        datetime_time_set(&now.time);
                    }
                    // Don't advance position
                break;
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
            switch (edit_position)
            {
                case 1:     // Hour Tens
                    edit_position = 2;
                break;

                case 2:     // Hour Ones
                    edit_position = 4;
                break;

                case 4:     // Minute Tens
                    edit_position = 5;
                break;

                case 5:     // Minute Ones
                    edit_position = 7;
                break;

                case 7:     // Seconds
                    edit_position = 1;
                break;
            }
        }

        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Press of the adj button saves the time and returns to the clock.

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


void
clock_draw_now (void)
{
    // Draw second
    display_primary_character(8, (now.time.second & 0x0F));
    display_primary_character(7, (now.time.second >> 4));

    // Draw minute
    display_primary_character(5, (now.time.minute & 0x0F));
    display_primary_character(4, (now.time.minute >> 4));

    // Colon
    display_period(DISPLAY_PERIOD_COLON);

    // Draw hour
    if (clock_fmt)
    {
        // 12-hour format

        if (0x00 == now.time.hour)
        {
            // Midnght - 12:00 AM
            display_primary_character(1, 1);
            display_primary_character(2, 2);
            display_misc(DISPLAY_MISC_AM);
            display_misc_clear(DISPLAY_MISC_PM);
        }
        else if (0x12 == now.time.hour)
        {
            // Noon - 12:00 PM
            display_primary_character(1, 1);
            display_primary_character(2, 2);
            display_misc(DISPLAY_MISC_PM);
            display_misc_clear(DISPLAY_MISC_AM);
        }
        else if (0x12 < now.time.hour)
        {
            // PM
            if ((now.time.hour >> 4) - 1)
            {
                // Only display 1's
                display_primary_character(1, ((now.time.hour >> 4) - 1));
            }
            display_primary_character(2, ((now.time.hour & 0x0F) - 2));
            display_misc(DISPLAY_MISC_PM);
            display_misc_clear(DISPLAY_MISC_AM);
        }
        else
        {
            // AM
            if ((now.time.hour >> 4))
            {
                // Only display digits > 0
                display_primary_character(1, (now.time.hour >> 4));
            }
            else
            {
                // Clear digit instead of displaying 0
                display_primary_clear(1);
            }
            display_primary_character(2, (now.time.hour & 0x0F));
            display_misc(DISPLAY_MISC_AM);
            display_misc_clear(DISPLAY_MISC_PM);
        }
    }
    else
    {
        // 24-hour format
        display_primary_character(2, (now.time.hour & 0x0F));
        display_primary_character(1, (now.time.hour >> 4));

        // Clear AM/PM
        display_misc_clear(DISPLAY_MISC_AM);
        display_misc_clear(DISPLAY_MISC_PM);
    }
}


void
clock_draw_edit (void)
{
    // Draw second
    display_primary_character(8, (SECONDS & 0x0F));
    display_primary_character(7, (SECONDS >> 4));

    // Draw minute
    display_primary_character(5, (edit_now.time.minute & 0x0F));
    display_primary_character(4, (edit_now.time.minute >> 4));

    // Colon
    display_period(DISPLAY_PERIOD_COLON);

    // 24-hour format
    display_primary_character(2, (edit_now.time.hour & 0x0F));
    display_primary_character(1, (edit_now.time.hour >> 4));

    // Clear AM/PM
    display_misc_clear(DISPLAY_MISC_AM);
    display_misc_clear(DISPLAY_MISC_PM);
}


// EOF //