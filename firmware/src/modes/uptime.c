/** @file uptime.c
 * 
 * This mode implements an uptime counter.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/settings.h"
#include "lib/display.h"
#include "lib/events.h"
// #include "lib/tick.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
// #include "lib/buzzer.h"
// #include "lib/backlight.h"
#include "lib/datetime.h"
#include "lib/alarm.h"

#define LOG_TAG "mode.uptime"
#include "lib/logging.h"

#include "modes/mode_settings.h"
#include "modes/uptime.h"


/* The current battery uptime in days */
static unsigned int current_uptime = 0;

// Time repr of 12:00 noon
// static time_t noon = {0x12, 0x00, 0x00};

// Time repr of 12:00 midnight
static time_t midnight = {0x00, 0x00, 0x00};


void
uptime_init (void)
{
    // Load saved uptime from EEPROM
    // current_uptime = settings_get_int(SETTING_UPTIME_H, SETTING_UPTIME_L);
    // current_uptime = settings_get(SETTING_UPTIME_H);
    // current_uptime |= settings_get(SETTING_UPTIME_L);

    // Register alarm for day counter
    alarm_set_time(&midnight, 0xAE);
}

void
uptime_start (void)
{
    // Display tag
    display_secondary_string(1, "UP");

    // Load saved uptime from EEPROM
    current_uptime = settings_get_int(SETTING_UPTIME_H, SETTING_UPTIME_L);

    // Display current uptime
    display_primary_string(1, "day");
    // display_primary_string(-1, "----");
    display_primary_number(8, (long)current_uptime);

    // display_update();
}


signed char
uptime_run (unsigned int event)
{

    switch (EVENT_TYPE(event))
    {
    case EVENT_TICK:
    break;

    case KEYPAD_EVENT_PRESS:
    break;

    case KEYPAD_EVENT_RELEASE:
    break;

    case EVENT_BUTTON:
        switch (EVENT_DATA(event))
        {
        case BUTTON_MODE_PRESS:
            return 1;
        break;
            
        case BUTTON_MODE_RELEASE:
        break;

        case BUTTON_ADJ_PRESS:
            // Reset uptime counter
            current_uptime = 0;
            settings_set_int(SETTING_UPTIME_H, SETTING_UPTIME_L, current_uptime);
            display_primary_number(8, (long)current_uptime);

            // Re-register uptime alarm
            alarm_del_event(UPTIME_ALARM_EVENT);
            alarm_set_time(&midnight);
        break;

        case BUTTON_ADJ_RELEASE:
        break;
        }
    break;

    case EVENT_ALARM:
        switch (EVENT_DATA(event))
        {
            case UPTIME_ALARM_EVENT:
                current_uptime = settings_get_int(SETTING_UPTIME_H, SETTING_UPTIME_L);
                display_primary_number(8, (long)current_uptime);
            break;
        }
    break;

    default:
    break;
    }

    return 0;
}


void
uptime_stop (void)
{
}

void
uptimed (unsigned int event)
{
    if (event == EVENT_ID(EVENT_ALARM, UPTIME_ALARM_EVENT))
    {
        // Increment day counter at midnight
        settings_set_int(SETTING_UPTIME_H, SETTING_UPTIME_L, ++current_uptime);

        // Register another alarm for next time
        alarm_set_time(&midnight, 0xAE);
    }
}

// EOF //
