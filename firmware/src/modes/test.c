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

        // Also give a short beep
        buzzer_tone(3500, 50, 15);
    break;

    case KEYPAD_EVENT_RELEASE:
        // Key release just clears the '-'
        display_secondary_clear(2);
    break;


    case EVENT_BUTTON:

        LOG_INFO("Button: 0x%.2x", EVENT_DATA(event));
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Return 1 to signal to switch modes
            return 1;
        }
        else if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            // Adj button toggles the backlight
            backlight_set(BACKLIGHT_TOGGLE);
        }
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
