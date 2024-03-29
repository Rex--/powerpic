/** @file alarmclock.c
 * 
 * This mode implements a configurable alarm and an hourly chime.
 * 
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/alarm.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buttons.h"
#include "lib/buzzer.h"

#include "modes/alarmclock.h"

#define LOG_TAG "mode.alarmclock"
#include "lib/logging.h"


// Status of daily alarm. 0 is disabled
static unsigned char daily_alarm_enabled = 0;
// Time object that holds our daily alarm.
static time_t daily_alarm = {
    .hour   = 0x16,
    .minute = 0x20,
    .second = 0x00
};

// Status of daily alarm. 0 is disabled
static unsigned char hourly_alarm_enabled = 0;
// Time object to hold the next hourly alarm.
static time_t hourly_alarm;

// Draw the daily alarm time to the display.
void daily_alarm_draw (void);
void daily_alarm_edit_draw (void);
void alarmclock_edit_start (void);
void hourly_alarm_update (void);

void
alarmclock_init (void)
{
    // Initialize some default values for the hourly alarm
}

void
alarmclock_start (void)
{
    // Tickrate is disabled by default and that's what we want

    // Draw 'AL' in corner
    display_secondary_string(1, "AL");

    // Draw the daily alarm
    display_primary_clear(0);
    daily_alarm_draw();

    if (daily_alarm_enabled)
    {
        // If the daily alarm is enabled, draw the wave
        display_misc(DISPLAY_MISC_WAVE);
    }

    if (hourly_alarm_enabled)
    {
        // If the hourly chime is enabled, draw the bell
        display_misc(DISPLAY_MISC_BELL);
    }
}

signed char
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
            if (hourly_alarm_enabled)
            {
                hourly_alarm_enabled = 0;
                display_misc_clear(DISPLAY_MISC_BELL);
                alarm_del_event(ALARMCLOCK_EVENT_CHIME);
            }
            else
            {
                hourly_alarm_enabled = 1;
                display_misc(DISPLAY_MISC_BELL);
                hourly_alarm_update();
                alarm_set_time(&hourly_alarm, ALARMCLOCK_EVENT_CHIME);
            }
        }

        if (EVENT_DATA(event) == '4')
        {
            // 4 key  is used to toggle daily alarm
            if (daily_alarm_enabled)
            {
                daily_alarm_enabled = 0;
                display_misc_clear(DISPLAY_MISC_WAVE);

                alarm_del_event(ALARMCLOCK_EVENT_DAILY);
            }
            else
            {
                daily_alarm_enabled = 1;
                display_misc(DISPLAY_MISC_WAVE);

                alarm_set_time(&daily_alarm, ALARMCLOCK_EVENT_DAILY);
            }
        }
    }

    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Switch modes on mode press
            return 1;
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button is used to edit the daily alarm.

            // Set the run thread to be our edit thread
            alarmclock_mode.run = &alarmclock_edit;

            // Set tickrate to our blink rate
            tick_rate_set_ms(500);

            alarmclock_edit_start();
        }
    }
    return 0;
}


void
alarmclock_stop (void)
{
    // Clear AM/PM for next mode
    display_misc_clear(DISPLAY_MISC_AM);
    display_misc_clear(DISPLAY_MISC_PM);

    // Clear colon for next mode
    display_period_clear(DISPLAY_PERIOD_COLON);
}

static unsigned char daily_alarm_is_beeping = 0;

void
alarmclockd (unsigned int event)
{
    if (EVENT_TYPE(event) == EVENT_ALARM)
    {
        if (EVENT_DATA(event) == ALARMCLOCK_EVENT_CHIME)
        {
            LOG_DEBUG("Chime alarm event event");
            // Beep Block Beep
            buzzer_tone(4500, 100, 75);
            __delay_ms(100);
            buzzer_tone(4500, 100, 75);

            // Update hourly alarm to new time
            hourly_alarm_update();

            // Register alarm
            alarm_set_time(&hourly_alarm, ALARMCLOCK_EVENT_CHIME);
        }

        if (EVENT_DATA(event) == ALARMCLOCK_EVENT_DAILY)
        {
            LOG_DEBUG("Daily alarm event");
            // 20 sec long alarm
            // Disabled with any button press

            // NOTE: Since our buzzer and delays block right now, we just give
            // a few beeps. (This means you can't silence it)
            // daily_alarm_is_beeping = 1;
            buzzer_tone(3200, 100, 75);
            __delay_ms(50);
            buzzer_tone(3200, 100, 75);
            __delay_ms(50);
            buzzer_tone(3200, 100, 75);


            // Set another alarm for our daily time, this should be
            // automatically set for tomorrow since the time has passed.
            alarm_set_time(&daily_alarm, ALARMCLOCK_EVENT_DAILY);
        }
    }

    if (daily_alarm_is_beeping &&
        ((EVENT_TYPE(event) == EVENT_KEYPAD) || 
        (EVENT_TYPE(event) == EVENT_BUTTON)))
    {
        // Daily alarm is beeping, any button or keypress silences it.
        LOG_DEBUG("Silencing daily alarm");
        daily_alarm_is_beeping = 0;
    }
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

    // And clear the alarm enabled wave
    display_misc_clear(DISPLAY_MISC_WAVE);
}

signed char
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
                    // Only acknowledge buttons 0-1
                    if (0 == keypress)
                    {
                        daily_alarm.hour &= 0x0F;
                        edit_position = 2;
                    }
                    else if (1 == keypress)
                    {
                        // Clear hour ones place to zero if it's over 2
                        if ((daily_alarm.hour & 0x0F) > 2)
                        {
                            daily_alarm.hour = 0x10;
                        }
                        else
                        {
                            daily_alarm.hour =
                                (unsigned char)(
                                    (daily_alarm.hour & 0x0F) | (1 << 4)
                                );
                        }
                        edit_position = 2;
                    }
                break;

                case 2:     // Hour Ones
                    if (1 == (daily_alarm.hour >> 4))
                    {
                        // Only acknowledge buttons 0-2 if hour is 1
                        if (3 > keypress)
                        {
                            daily_alarm.hour =
                                ((daily_alarm.hour & 0xF0) | (keypress));
                            edit_position = 4;
                        }
                    }
                    else
                    {
                        // Acknowledge 1-9
                        daily_alarm.hour = ((daily_alarm.hour & 0xF0) | (keypress));
                        edit_position = 4;
                    }
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
                    daily_alarm.minute =
                        ((daily_alarm.minute & 0xF0) | (keypress));
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
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Mode button press advances edit position
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
                    edit_position = 1;
                break;
            }
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button is used to confirm alarm and return to display mode

            // Convert back to 24 hour format
            if (edit_tod)
            {
                // PM time

                // Handle 12:00 PM (Noon)
                if (0x12 != daily_alarm.hour)
                {
                    daily_alarm.hour += 0x12;
                }
            }
            else
            {
                // AM 

                // Handle 12:00 AM (Midnight)
                if (0x12 == daily_alarm.hour)
                {
                    daily_alarm.hour = 0x00;
                }
            }

            if (daily_alarm_enabled)
            {
                // Alarm is enabled, register alarm
                
                // Delete any alarms with the old time
                alarm_del_event(ALARMCLOCK_EVENT_DAILY);

                alarm_set_time(&daily_alarm, ALARMCLOCK_EVENT_DAILY);

                // Enable indicator
                display_misc(DISPLAY_MISC_WAVE);
            }
            
            // Set the run thread to be our main run thread
            alarmclock_mode.run = &alarmclock_run;

            // Disable ticks
            tick_disable();

            // There might be a 0 in the hour tens place if hour < 10
            display_primary_clear(1);
            daily_alarm_draw();
        }
    }
    return 0;
}


void
hourly_alarm_update (void)
{
    datetime_time_now(&hourly_alarm);

    if (0x23 == hourly_alarm.hour)
    {
        // Handle rollover
        hourly_alarm.hour = 0x00;
    }
    else if (0x09 == hourly_alarm.hour)
    {
        // Handle jump from 0x09 to 0x10
        hourly_alarm.hour = 0x10;
    }
    else if (0x19 == hourly_alarm.hour)
    {
        // handle jump from 0x19 to 0x20
        hourly_alarm.hour = 0x20;
    }
    else
    {
        hourly_alarm.hour += 1;
    }
    
    // Clear minutes
    hourly_alarm.minute = 0;

    // Clear seconds
    hourly_alarm.second = 0;
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
        if ((daily_alarm.hour >> 4) - 1)
        {
            // Only display 1's
            display_primary_character(1, ((daily_alarm.hour >> 4) - 1));
        }
        display_primary_character(2, ((daily_alarm.hour & 0x0F) - 2));
        display_misc(DISPLAY_MISC_PM);
    }
    else
    {
        // AM
        if ((daily_alarm.hour >> 4))
        {
            // Don't display 0's
            display_primary_character(1, (daily_alarm.hour >> 4));
        }
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
