/** @file timer.c
 *
 * This mode implements various timers.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/buzzer.h"
#include "lib/backlight.h"
#include "lib/datetime.h"

#define LOG_TAG "mode.timer"
#include "lib/logging.h"

#include "modes/timer.h"

enum timer_types {
    TIMER_STOPWATCH,
    TIMER_COUNTDOWN,
    TIMER_MAX_TIMERS
};

typedef void (*timer_start_t) (void);

void timer_stopwatch_start (void);
void timer_countdown_start (void);

static timer_start_t timer_start_funcs[TIMER_MAX_TIMERS] = {
    &timer_stopwatch_start,
    &timer_countdown_start,
};


typedef void (*timer_run_t) (unsigned int event);

void timer_stopwatch_run (unsigned int event);
void timer_countdown_run (unsigned int event);

static timer_run_t timer_run_funcs[TIMER_MAX_TIMERS] = {
    &timer_stopwatch_run,
    &timer_countdown_run,
};


static unsigned char timer_type = 0;



//// Variables for countdown timer ////

// Any number above 0 signifies that the timer is active
static unsigned char timer_countdown_active = 0;

// Any number above 0 signifies that we are editing the timer
static unsigned char timer_countdown_edit = 0;

// Any number above 0 signifies that the timer should repeat when finished
static unsigned char timer_countdown_repeat = 0;

// This is the time that the current timer will reach 0
static datetime_t timer_countdown_alarm = {0,0,0,0,0,0,0};

// This is the time of the current countdown timer
static time_t timer_countdown_time = {0,0,0};

// This is the time that the countdown timer was originally set to.
static time_t timer_countdown_reset = {0,0,0};



//// Helper functions ////
void timer_next (void);
static void timer_display_time (time_t *time);


void
timer_init (void)
{
    timer_type = TIMER_STOPWATCH;

    timer_countdown_active = 0;
    timer_countdown_edit = 0;
    timer_countdown_repeat = 0;
    timer_countdown_reset.minute = 15; // Countdown timer defaults to 15 minutes
    timer_countdown_time.minute = 15;  // with no time having elapsed
}

void
timer_start (void)
{
    if (TIMER_MAX_TIMERS > timer_type)
    {
        // Call the corresponding timer's start function
        timer_start_funcs[timer_type]();
    }

    else
    {
        display_secondary_string(1, "Er");
    }
}


void
timer_stopwatch_start (void)
{
    // Secondary display tag
    display_secondary_character(1, 'S');
    display_secondary_segments(2,0b0010110001);
    // the letter 'T', but we use additional segments

    // Display 00:00 00 on screen
    display_primary_string(1, "00 00 00");
    display_period(DISPLAY_PERIOD_COLON);
}


void
timer_countdown_start (void)
{
    display_secondary_string(1, "Cd");

    // Display 00:00 00 on screen
    display_primary_string(1, "00 00 00");
    display_period(DISPLAY_PERIOD_COLON);

    // Then display actual timer on top of that (keeps the zeros)
    timer_display_time(&timer_countdown_time);

    // Display multiplication sign if timer is set to repeat
    if (timer_countdown_repeat)
    {
        display_sign(DISPLAY_SIGN_MULTIPLY);
    }

    if (timer_countdown_active)
    {
        // If the timer is active set tick rate to 1 second
        tick_rate_set_sec(1);
    }
}


signed char
timer_run (unsigned int event)
{
    // We catch all button presses and respond to them the same way:
    // * MODE - A press of the mode button behaves like it does all the time,
    //      by cycling to the next mode by returning 1.
    // * ADJ - A press of the adj button will cycle to the next type of timer.
    //      Currently, it just switches between stopwatch and countdown.
    if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Return 1 to signal to switch modes
            return 1;
        }
        else if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button cycles to the next timer
            timer_next();
            return 0;
        }
    }

    if (TIMER_MAX_TIMERS > timer_type)
    {
        timer_run_funcs[timer_type](event);
    }
    else
    {
        // Cycle to next mode if we somehow get an invalid timer type.
        // Also reset timer type to 0 so mode won't get stuck in invalid type.
        timer_type = 0;
        return 1;
    }

    return 0;
}

void
timer_stopwatch_run (unsigned int event)
{
    // display_primary_string(2, "00 00 00");
    // display_period(DISPLAY_PERIOD_COLON);

    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        // if (stopwatch_active)
        // {

        // }
    break;

    case KEYPAD_EVENT_PRESS:
        if (EVENT_DATA(event) == '+')
        {
            // Plus key starts/stops stopwatch
        }
        if (EVENT_DATA(event) == '0')
        {
            // 0 is Lap key
        }
        if (EVENT_DATA(event) == '/')
        {
            // Divide key shows hours
        }
    break;
    
    default:
    break;
    }
}

void
timer_countdown_run (unsigned int event)
{
    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        if (timer_countdown_active)
        {
            display_primary_string(1, "00 00 00");
            timer_display_time(&timer_countdown_time);
            if (timer_countdown_time.second > 0)
            {
                timer_countdown_time.second = (unsigned char)DEC2BCD(BCD2DEC(timer_countdown_time.second) - 1);
            }
            else // Minute rollover
            {
                timer_countdown_time.second = DEC2BCD(59);

                if (timer_countdown_time.minute > 0)
                {
                    timer_countdown_time.minute = (unsigned char)DEC2BCD(BCD2DEC(timer_countdown_time.minute) - 1);
                }
                else // Hour rollover
                {
                    timer_countdown_time.minute = DEC2BCD(59);

                    if (timer_countdown_time.hour > 0)
                    {
                        timer_countdown_time.hour = (unsigned char)DEC2BCD(BCD2DEC(timer_countdown_time.hour) - 1);
                    }
                    else // Timer is finished
                    {
                        if (timer_countdown_repeat)
                        {
                            // Repeat timer
                            timer_countdown_time = timer_countdown_reset;
                            timer_display_time(&timer_countdown_time);
                        }
                        else
                        {
                            // Do not repeat timer, disable ticks and set timer inactive
                            tick_disable();
                            timer_countdown_active = 0;
                        }
                    }
                }
            }
        }
    break;

    case KEYPAD_EVENT_PRESS:
        switch (EVENT_DATA(event))
        {
            case '+':   // Plus key starts/pauses countdown
                if (timer_countdown_active) // Pause timer
                {
                    timer_countdown_active = 0; // Deactivate countdown
                    tick_disable(); // Disable ticks
                }

                else
                {
                    if (timer_countdown_edit)
                    {
                        // If were in edit mode, reset the timer to the new value and leave edit mode
                        timer_countdown_time = timer_countdown_reset;
                        timer_countdown_edit = 0;
                        display_sign_clear(DISPLAY_SIGN_ADD);
                    }
                    timer_countdown_active = 1; // Enable countdown
                    tick_rate_set_sec(1);  // Enable ticks
                }
            break;

            case '*':   // Multiply key toggles repeat of timer
                if (timer_countdown_repeat)
                {
                    timer_countdown_repeat = 0;
                    display_sign_clear(DISPLAY_SIGN_MULTIPLY);
                }
                else
                {
                    timer_countdown_repeat = 1;
                    display_sign(DISPLAY_SIGN_MULTIPLY);
                }
            break;

            case '=':   // Equals key edits the timer
                // Only enter edit mode when timer is not running
                if (!timer_countdown_active)
                {
                    if (timer_countdown_edit)
                    {
                        // If we're already in edit mode, save the value and leave
                        // edit mode
                        timer_countdown_time = timer_countdown_reset;
                        timer_countdown_edit = 0;
                        display_sign_clear(DISPLAY_SIGN_ADD);
                    }
                    else
                    {
                        // If we aren't in edit mode, then enter edit mode
                        timer_countdown_edit = 1;
                        // Reset timer value to 0
                        timer_countdown_reset.hour = 0;
                        timer_countdown_reset.minute = 0;
                        timer_countdown_reset.second = 0;
                        display_primary_string(1, "00 00 00");
                        display_sign(DISPLAY_SIGN_ADD);
                    }
                }
            break;

            case '0':   // 0 is the reset key when not in edit mode
                if (!timer_countdown_edit && !timer_countdown_active)
                {
                    timer_countdown_time = timer_countdown_reset;
                    timer_display_time(&timer_countdown_time);
                    break;
                }
            // case '0' will fall through to our number key handler when in edit mode
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (timer_countdown_edit)
                {
                    unsigned char new_number = (unsigned char)EVENT_DATA(event) - 48; // Event data contains the number in ascii, we subtract 48 to get the numerical value of the ascii representation
                    // We shift all the numbers one left when a new number is input
                    timer_countdown_reset.hour = (unsigned char)((timer_countdown_reset.hour << 4) | (timer_countdown_reset.minute >> 4));
                    timer_countdown_reset.minute = (unsigned char)((timer_countdown_reset.minute << 4) | (timer_countdown_reset.second >> 4));
                    timer_countdown_reset.second = (unsigned char)((timer_countdown_reset.second << 4) | (new_number));
                    // Update display with new time

                    timer_display_time(&timer_countdown_reset);
                }
            break;

            default: // Unused keys use the default case
            break;
        }
    break;
    
    default:
    break;
    }
}

void
timer_stop (void)
{
    display_sign_clear(DISPLAY_SIGN_MULTIPLY);
}

void
timer_next (void)
{
    display_primary_clear(0);
    display_period_clear(0);
    display_sign_clear(DISPLAY_SIGN_MULTIPLY);
    timer_type = (timer_type + 1) % TIMER_MAX_TIMERS;
    timer_start_funcs[timer_type]();
}

//// Helper functions ////

static void
timer_display_time (time_t *time)
{
    display_primary_string(1, "00 00 00");
    display_primary_number(2, BCD2DEC(time->hour));
    display_primary_number(5, BCD2DEC(time->minute));
    display_primary_number(8, BCD2DEC(time->second));
}

// EOF //
