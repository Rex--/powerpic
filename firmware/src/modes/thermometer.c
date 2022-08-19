/** @file thermometer.c
 * 
 * This mode implements a thermometer that uses the internal temperature sensor.
*/

#include <xc.h>
#include <math.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
// #include "lib/buzzer.h"
// #include "lib/backlight.h"
#include "lib/temperature.h"

#include "lib/logging.h"

#include "modes/thermometer.h"

#undef  LOG_TAG
#define LOG_TAG "mode.thermometer"


// Last taken temperature
static int last_temp = 0;

// Format of displayed temp. 0/1 for C/F
static unsigned char temp_fmt = 0;

// Display last_temp in temp_fmt
static void thermometer_display_temp (void);


void
thermometer_start (void)
{
    // Update rate of 30 seconds.
    tick_rate_set_sec(30);

    // Draw our logo and basic UI
    display_secondary_segments(1, 0b0010110001); // A better letter T
    display_secondary_character(2, 'E');
    display_primary_string(1, "CPU --* ");

    // Take an initial temp reading and display it
    last_temp = temperature_read();
    thermometer_display_temp();
}

signed char
thermometer_run (unsigned int event)
{
    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        last_temp = temperature_read();
        thermometer_display_temp();
    break;

    case KEYPAD_EVENT_PRESS:
        if ('+' == EVENT_DATA(event))
        {
            // Plus key toggles between C/F
            if (temp_fmt)
            {
                temp_fmt = 0;
            }
            else
            {
                temp_fmt = 1;
            }
            
            // Display temp in new format without updating it.
            thermometer_display_temp();
        }
    break;

    case EVENT_BUTTON:
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Return 1 to signal to switch modes
            return 1;
        }
    break;
    
    default:
    break;
    }

    return 0;
}


static void
thermometer_display_temp (void)
{
    if (temp_fmt)
    {
        // Display temp in fahrenheit
        int last_temp_f = (int)lroundf((last_temp * 1.8F) + 32);
        display_primary_number(-3, last_temp_f);
        display_primary_character(-1, 'F');
        
        LOG_DEBUG("Temp: %i*F", last_temp_f);
    }
    else
    {
        // Display temp in celsius
        display_primary_number(-3, last_temp);
        display_primary_character(-1, 'C');

        LOG_DEBUG("Temp: %i*C", last_temp);
    }
}


// EOF //
