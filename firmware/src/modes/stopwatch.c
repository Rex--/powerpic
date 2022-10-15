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

static unsigned char stopwatch_active = 0;

static long hexnumber = 0;

void
stopwatch_init (void)
{
    stopwatch_active = 0;
}

void
stopwatch_start (void)
{
    // Secondary display tag
    display_secondary_character(1, 'S');
    display_secondary_segments(2,0b0010110001);

    // Display stopwatch time
    // display_primary_string(1, "00 00 00");
    // display_period(DISPLAY_PERIOD_COLON);
    tick_rate_set_ms(500);
    hexnumber = 0;
    display_primary_hex(-1, hexnumber);
}

signed char
stopwatch_run (unsigned int event)
{
    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        display_primary_hex(-1, hexnumber++);
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


    case EVENT_BUTTON:
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Return 1 to signal to switch modes
            return 1;
        }
        else if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
        }
    break;
    
    default:
    break;
    }

    return 0;
}

void
stopwatch_stop (void)
{

}

// EOF //
