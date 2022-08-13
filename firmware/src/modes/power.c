/** @file power.c
 * 
 * This mode application displays the current battery voltage. It has buttons
 * to that turn on the backlight when pressed (+) and a button to play a tone
 * when pressed (/)[TODO].
 * 
 * Voltage displayed seems to be within -+0.02V of actual voltage.
*/

#include <xc.h>
#include <math.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/battery.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/backlight.h"
// #include "lib/buzzer.h"

#include "lib/logging.h"

#include "modes/power.h"


// Undefine LOG_TAG before defining our own. This will get preprended to
// each log message.
#undef  LOG_TAG
#define LOG_TAG "mode.power"

// This variable holds the battery voltage as a float.
static float battery_voltage = 0.0;

void
power_start (void)
{
    // Draw something immediately.
    display_primary_string(1, "bat v---");
    display_update();

    // Get inital battery reading
    battery_voltage = battery_read_voltage();

    // Draw it to the display
    display_primary_number(8, lroundf(battery_voltage*100));
    display_period(6);

    LOG_INFO("Battery: %0.2f", battery_voltage);

    // Set tick rate at 2 minutes
    tick_rate_set_sec(120);
}

signed char
power_run (unsigned int event)
{
switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
        // Take a new battery measurement every tick event and average it with
        // the previous value.
        battery_voltage += battery_read_voltage();
        battery_voltage /= 2;
        display_primary_number(8, lroundf(battery_voltage*100));

        LOG_INFO("Battery: %0.2f", battery_voltage);
    break;

    case KEYPAD_EVENT_PRESS:
        if (EVENT_DATA(event) == '/')
        {
            // Divide key turns on backlight.
            backlight_on();

            // Set tick rate to 1 sec so we can watch the voltage fall.
            tick_rate_set_sec(1);

            // Get initial voltage with the backlight on.
            battery_voltage = battery_read_voltage();
        }
        else if (EVENT_DATA(event) == '+')
        {
            // Plus key plays a 5 sec tone.
            // buzzer_tone2(3000, 100, 5000)

            // Sets the tick rate to 1 sec while button is down.
            // tick_rate_set_sec(1);
        }

    break;

    case KEYPAD_EVENT_RELEASE:
        if (EVENT_DATA(event) == '/')
        {
            // Turn off backlight.
            backlight_off();

            // Set tick rate back to 2 minutes.
            tick_rate_set_sec(120);

            // Get battery voltage without backlight on, the next update will
            // be in 2 minutes.
            battery_voltage = battery_read_voltage();
        }
        else if (EVENT_DATA(event) == '+')
        {
            // Set the tick rate back to 2 minutes.
            // tick_rate_set_sec(120);
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
            // Adj button resets the rolling average.
            battery_voltage = battery_read_voltage();
            display_primary_number(8, lroundf(battery_voltage*100));
        }
    break;
    
    default:
    break;
    }

    return 0;
}

void
power_stop (void)
{
    display_period_clear(6);
}

// EOF //
