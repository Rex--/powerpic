/** @file test.c
 * 
 * This mode application implements various tests.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/buttons.h"
#include "lib/keypad.h"

#include "modes/test.h"

// Define a LOG_TAG before importing logging lib. This will get preppended to
// each log message.
//
#define LOG_TAG "mode.test"
#include "lib/logging.h"

void
test_init (void)
{
    LOG_INFO("Initializing World!");
}

void
test_start (void)
{
    LOG_INFO("Starting World!");
}

signed char
test_run (unsigned int event)
{
    // This will cause problems @ 100ms tickrate
    LOG_DEBUG("Running World!");

    if (EVENT_TYPE(event) == EVENT_TICK)
    {
        datetime_t dt;
        datetime_now(&dt);  // Debug print current time
    }

    else if (EVENT_TYPE(event) == EVENT_ALARM)
    {

    }
    
    else if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        LOG_INFO("Keypress: %c", EVENT_DATA(event));
    }

    else if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Return 1 to signal to switch modes
            return 1;
        }
        if (EVENT_DATA(event) == BUTTON_MODE_RELEASE)
        {
            
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            LOG_INFO("Adj button press");
            event_add(0x0C0A);  // Send an hourly chime alarm event
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_RELEASE)
        {

        }
    }

    return 0;
}

void
test_stop (void)
{
    // I at least have to warn people.
    LOG_WARNING("Stopping World!");
}


// EOF //
