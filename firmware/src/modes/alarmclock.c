/** @file alarmclock.c
 * 
 * This mode implements a configurable alarm and an hourly chime.
 * 
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/alarm.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buttons.h"

#include "modes/alarmclock.h"

// Time object that holds our daily alarm. If the seconds is 0, alarm
// is disabled. If the seconds is 1, alarm is enabled.
static time_t daily_alarm = {
    .hour   = 0x12,
    .minute = 0x05,
    .second = 0x00
};

// Time object to hold the next hourly alarm. If the seconds is 0, hourly chime
// is disabled. If the seconds is 1, hourly chime is enabled.
static time_t hourly_alarm = {0};

// Draw the daily alarm time to the display.
void daily_alarm_draw (void);
void daily_alarm_edit_draw (void);
void alarmclock_edit_start (void);

void
alarmclock_init (void)
{
    // Initialize some default values for the hourly alarm
}

void
alarmclock_start (void)
{
    // Draw 'AL' in corner
    display_secondary_string(1, "AL");

    // Draw the daily alarm
    daily_alarm_draw();

    if (daily_alarm.second)
    {
        // If the daily alarm is enabled, draw the wave
        display_misc(DISPLAY_MISC_WAVE);
    }

    if (hourly_alarm.second)
    {
        // If the hourly chime is enabled, draw the bell
        display_misc(DISPLAY_MISC_BELL);
    }
}

void
alarmclock_run (unsigned int event)
{

    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        // We don't want any tick events
    }

    if (EVENT_TYPE(event) == EVENT_ALARM)
    {
        // We don't do anything about alarm events.
        // Need an alarm clock daemon
    }
    
    if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        if (EVENT_DATA(event) == '*')
        {
            // Multiply key is used to toggle hourly chime
            if (hourly_alarm.second)
            {
                hourly_alarm.second = 0;
                display_misc_clear(DISPLAY_MISC_BELL);
            }
            else
            {
                hourly_alarm.second = 1;
                display_misc(DISPLAY_MISC_BELL);
            }
        }

        if (EVENT_DATA(event) == '4')
        {
            // 4 key  is used to toggle daily alarm
            if (daily_alarm.second)
            {
                daily_alarm.second = 0;
                display_misc_clear(DISPLAY_MISC_WAVE);
            }
            else
            {
                daily_alarm.second = 1;
                display_misc(DISPLAY_MISC_WAVE);
            }
        }
    }

    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button is used to edit the daily alarm.

            // Set the run thread to be our edit thread
            alarmclock_mode.run = &alarmclock_edit;

            // Set tickrate to our blink rate
            alarmclock_config.tickrate = 500;

            alarmclock_edit_start();
        }
    }
}


void
alarmclock_stop (void)
{

}


static unsigned char edit_blink = 0;

static unsigned char edit_position = 0;

static unsigned char edit_tod = 0;

void
alarmclock_edit_start (void)
{
    edit_blink = 1;
    edit_position = 1;

    // We temporarily convert the time to 12 hour format

    if (0x00 == daily_alarm.hour)
    {
        // Midnght - 12:00 AM
        daily_alarm.hour = 0x12;
        edit_tod = 0;
    }
    else if (0x12 == daily_alarm.hour)
    {
        // Noon - 12:00 PM
        edit_tod = 1;
    }
    else if (0x12 < daily_alarm.hour)
    {
        // PM
        daily_alarm.hour -= 0x12;
        edit_tod = 1;
    }
    else
    {
        // AM
        edit_tod = 0;
    }
}

void
alarmclock_edit (unsigned int event)
{
    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        // Draw our alarm time
        daily_alarm_edit_draw();

        // Blink character that is currently being edited
        // and am/pm sign
        if (edit_blink)
        {
            // We are blinking meaning we are drawing an underscore in place 
            // of the current place being edited
            display_primary_character((signed char)edit_position, '_');

            // And clearing the AM/PM
            display_misc_clear(DISPLAY_MISC_AM);
            display_misc_clear(DISPLAY_MISC_PM);

            edit_blink = 0;
        }
        else
        {
            edit_blink = 1;
        }
    }

    if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        unsigned char keypress = EVENT_DATA(event);

        if ((keypress >= '0') && (keypress <= '9'))
        {
            keypress -= 48;

            // Set alarm digit based on edit_position
            switch (edit_position)
            {
                case 1:     // Hour Tens
                    if (2 > keypress)
                    {
                        // Only acknowledge buttons 0-1
                        daily_alarm.hour =
                            (unsigned char)(
                                (daily_alarm.hour & 0x0F) | (keypress << 4)
                            );
                        edit_position = 2;
                    }
                break;

                case 2:     // Hour Ones
                    // TODO: Some error checking to make sure this is a valid value
                    daily_alarm.hour = ((daily_alarm.hour & 0xF0) | (keypress));
                    edit_position = 4;
                break;

                case 4:     // Minute Tens
                    if (6 > keypress)
                    {
                        // Only acknowledge buttons 0-5
                        daily_alarm.minute =
                            (unsigned char)(
                                (daily_alarm.minute & 0x0F) | (keypress << 4)
                            );
                        edit_position = 5;
                    }
                break;

                case 5:     // Minute Ones
                    daily_alarm.minute = ((daily_alarm.minute & 0xF0) | (keypress));
                    edit_position = 1;
                break;
            }

            daily_alarm_edit_draw();
        }

        if (keypress == '.')
        {
            // switch AM/PM

            if (edit_tod)
            {
                display_misc_clear(DISPLAY_MISC_PM);
                display_misc(DISPLAY_MISC_AM);
                edit_tod = 0;
            }
            else
            {
                display_misc_clear(DISPLAY_MISC_AM);
                display_misc(DISPLAY_MISC_PM);
                edit_tod = 1;
            }
        }
    }

    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button is used to confirm alarm and return to display mode

            if (edit_tod)
            {
                // Convert back to 24 hour format
                daily_alarm.hour += 0x12;
            }
            
            // Set the run thread to be our main run thread
            alarmclock_mode.run = &alarmclock_run;

            alarmclock_config.tickrate = 65535000;

            display_primary_clear(0);
            daily_alarm_draw();
        }
    }
}

void
daily_alarm_draw (void)
{
    // Draw alarm time
    //

    // (Pos 1-2) Display hour
    if (0x00 == daily_alarm.hour)
    {
        // Midnght - 12:00 AM
        display_primary_character(1, 1);
        display_primary_character(2, 2);
        display_misc(DISPLAY_MISC_AM);
    }
    else if (0x12 == daily_alarm.hour)
    {
        // Noon - 12:00 PM
        display_primary_character(1, 1);
        display_primary_character(2, 2);
        display_misc(DISPLAY_MISC_PM);
    }
    else if (0x12 < daily_alarm.hour)
    {
        // PM
        display_primary_character(1, ((daily_alarm.hour >> 4) - 1));
        display_primary_character(2, ((daily_alarm.hour & 0x0F) - 2));
        display_misc(DISPLAY_MISC_PM);
    }
    else
    {
        // AM
        display_primary_character(1, (daily_alarm.hour >> 4));
        display_primary_character(2, (daily_alarm.hour & 0x0F));
        display_misc(DISPLAY_MISC_AM);
    }

    // (3) Display colon
    display_period(DISPLAY_PERIOD_COLON);

    // (4-5) Display minute
    display_primary_character(4, (daily_alarm.minute >> 4));
    display_primary_character(5, (daily_alarm.minute & 0x0F));
}

void
daily_alarm_edit_draw (void)
{
    // (Pos 1-2) Display hour
    display_primary_character(1, (daily_alarm.hour >> 4));
    display_primary_character(2, (daily_alarm.hour & 0x0F));

    // (3) Display colon
    display_period(DISPLAY_PERIOD_COLON);

    // (4-5) Display minute
    display_primary_character(4, (daily_alarm.minute >> 4));
    display_primary_character(5, (daily_alarm.minute & 0x0F));

    if (edit_tod)
    {
        // PM time of day
        display_misc(DISPLAY_MISC_PM);
    }
    else
    {
        // AM time of day
        display_misc(DISPLAY_MISC_AM);
    }
}

// EOF //