/** @file test.c
 * 
 * This mode application implements various tests.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buttons.h"

#include "modes/test.h"

// Define a LOG_TAG before importing logging lib. This will get preppended to
// each log message.
//
#define LOG_TAG "mode.test"
#include "lib/logging.h"


// String that contains every font character, except punctuation.
// (it looks bad scrolling)
//
static const char *test_str = 
"the quick brown fox jumps over the lazy dog"
" 1234567890 "
"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG ";
// Note the trailing space - so we dont have to clear the display.


static const char *scroll_ptr = NULL;

void
test_init (void)
{
    LOG_INFO("Initializing World!");
}

void
test_start (void)
{
    LOG_INFO("Starting World!");
    scroll_ptr = test_str;
}

void
test_run (unsigned int event)
{
    // This will cause problems @ 100ms tickrate
    // LOG_DEBUG("Running World!");

    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        display_secondary_string(1, scroll_ptr);
        display_primary_string(1, scroll_ptr);
        if (*++scroll_ptr == '\0')
        {
            scroll_ptr = test_str;
        }
    }

    else if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        LOG_INFO("Keypress: %c", EVENT_DATA(event));
    }

    else if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_RELEASE)
        {
            
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            LOG_INFO("Adj button press");
            test_config.tickrate = 500;
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_RELEASE)
        {

        }
    }
}

void
test_stop (void)
{
    // I at least have to warn people.
    LOG_WARNING("Stopping World!");
}
