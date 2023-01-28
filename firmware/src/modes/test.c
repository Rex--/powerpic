/** @file test.c
 * 
 * This mode application implements various tests.
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

#include "lib/logging.h"

#include "modes/test.h"


// Undefine LOG_TAG before defining our own. This will get preppended to
// each log message.
#undef  LOG_TAG
#define LOG_TAG "mode.test"


// Test string to scroll across the main display
static const char * test_display_string_marker;
static const char test_display_string[] = 
    "The quick brown fox jumps over the lazy dog 1234567890.";


void
test_init (void)
{
    LOG_INFO("Initializing World!");
}

void
test_start (void)
{
    LOG_INFO("Starting World!");

    // Set tickrate to scroll rate. (1/2 sec)
    tick_rate_set_ms(500);
}

signed char
test_run (unsigned int event)
{
    LOG_DEBUG("Running World!");

    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        // For every tick event we move our scroll string and display it.
        display_primary_string(1, test_display_string_marker++);
        if (*test_display_string_marker == '\0')
        {
            test_display_string_marker = &test_display_string[0];
        }
    break;

    case KEYPAD_EVENT_PRESS:
        LOG_INFO("Keypress: %c", EVENT_DATA(event));

        // Display keypress on secondary display
        display_secondary_character(1, EVENT_DATA(event));

        // Display a '-' when key is down
        display_secondary_character(2, '-');

        // Manually update the display for our keypress
        display_update();

        // Disable ticks while we play a song
        tick_disable();

        // Also play a song
        // static char song[] = "DKCountry:d=4,o=5,b=125:32p,8c,8c,a.,p,8f,8g,8f,d.,p,8d,8d,a#.,p,8g,8a,8g,e.,p,8e,8e,c.6,p,8a,8a#,8c6,d.6,p,8f,8g,a,f,8p,8e,8f,8g,d.";
        // static char song[] = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
        static char song[] = "Mario:d=4,o=5,b=115:32p,16e6,16e6,16p,16e6,16p,16c6,16e6,16p,16g6,8p,16p,16g,8p,32p,16c6,8p,16g,8p,16e,8p,16a,16p,16b,16p,16a#,16a,16p,16g,16e6,16g6,16a6,16p,16f6,16g6,16p,16e6,16p,16c6,16d6,16b,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,8d#6,16p,16d6,8p,8c6";
        buzzer_play_rtttl(song);

        // Enable ticks after we're done
        tick_enable();
    break;

    case KEYPAD_EVENT_RELEASE:
        // Key release just clears the '-'
        display_secondary_clear(2);
    break;


    case EVENT_BUTTON:

        LOG_INFO("Button: 0x%.2x", EVENT_DATA(event));
        switch (EVENT_DATA(event))
        {
        case BUTTON_MODE_PRESS:
            display_primary_string(0, "MODE DN");
            tick_disable();
        break;
        case BUTTON_MODE_RELEASE:
            display_primary_string(0, "MODE UP");
            tick_enable();
        break;

        case BUTTON_ADJ_PRESS:
            display_primary_string(0, "ADJ DN");
            tick_disable();
        break;
        case BUTTON_ADJ_RELEASE:
            display_primary_string(0, "ADJ UP");
            tick_enable();
        break;
        }

        // if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        // {
        //     display_primary_string(0, "MOdE");
        //     // Return 1 to signal to switch modes
        //     // return 1;
        // }
        // else if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        // {
        //     display_primary_string(0, "AdJ");
        //     // Adj button toggles the backlight
        //     // backlight_set(BACKLIGHT_TOGGLE);
        // }
    break;
    
    default:
        // Log every other event
        LOG_DEBUG("Untested event: 0x%x", event);
    break;
    }

    // Return 0 to indicate OK, were good, everything ran good, we want more good.
    return 0;
}

void
test_stop (void)
{
    // I at least have to warn people.
    LOG_WARNING("Stopping World!");
}


// EOF //
