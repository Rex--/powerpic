/** @file thermometer.c
 * 
 * This mode implements a thermometer that uses the internal temperature sensor.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
// #include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
// #include "lib/buzzer.h"
// #include "lib/backlight.h"
#include "lib/temperature.h"

#include "lib/logging.h"

#include "modes/thermometer.h"

#undef  LOG_TAG
#define LOG_TAG "mode.thermometer"

void
thermometer_init (void)
{

}

void
thermometer_start (void)
{
    tick_rate_set_ms(1000);
}

signed char
thermometer_run (unsigned int event)
{
    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        temperature_read();
    break;

    case KEYPAD_EVENT_PRESS:

    break;

    case KEYPAD_EVENT_RELEASE:

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
thermometer_stop (void)
{

}




// EOF //
