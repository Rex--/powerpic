/** @file stopwatch.c
 *
 * This mode implements a stopwatch and various other timers.
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

#define LOG_TAG "mode.stopwatch"
#include "lib/logging.h"

#include "modes/stopwatch.h"

enum stopwatch_types {
    TIMER_STOPWATCH,
    TIMER_COUNTDOWN,
    TIMER_MAX_TIMERS
};

static unsigned char stopwatch_type = 0;

static unsigned char stopwatch_active = 0;

void timer_stopwatch_run (unsigned int event);
void timer_countdown_run (unsigned int event);


void
stopwatch_init (void)
{
    stopwatch_type = TIMER_STOPWATCH;
    stopwatch_active = 0;
}

void
stopwatch_start (void)
{
    // Secondary display tag
    display_secondary_character(1, 'S');
    display_secondary_segments(2,0b0010110001);
    // the letter 'T', but we use additional segments

    // Display stopwatch time
    // display_primary_string(1, "00 00 00");
    // display_period(DISPLAY_PERIOD_COLON);
}


signed char
stopwatch_run (unsigned int event)
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
            stopwatch_type = (stopwatch_type + 1) % TIMER_MAX_TIMERS;
            return 0;
        }
    }

    switch (stopwatch_type)
    {
    case TIMER_STOPWATCH:
        timer_stopwatch_run(event);
    break;

    case TIMER_COUNTDOWN:
        timer_countdown_run(event);
    break;
    
    default:
        // Cycle to next mode if we somehow get a timer type we don't recognize.
        return 1;
    break;
    }

    return 0;
}

void
timer_stopwatch_run (unsigned int event)
{
    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        if (stopwatch_active)
        {
            // display_primary
        }
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
            // Divide key switches to countdown timer
        }
    break;



    break;
    
    default:
    break;
    }
}

void
timer_countdown_run (unsigned int event)
{
}

void
stopwatch_stop (void)
{

}

// EOF //
