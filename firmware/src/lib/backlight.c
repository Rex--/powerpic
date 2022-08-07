/** @file backlight.c
 * 
 * This library implements the functionality of the backlight on the powerpic.
*/

#include <xc.h>

#include "drivers/pins.h"

#include "lib/backlight.h"

#define LOG_TAG "lib.backlight"
#include "lib/logging.h"


#if (PCB_REV == 2)
// Backlight pin is RG7
#   define BACKLIGHT_PORT      PORTG
#   define BACKLIGHT_TRIS      TRISG
#   define BACKLIGHT_LAT       LATG
#   define BACKLIGHT_ANSEL     ANSELG
#   define BACKLIGHT_PIN       7
#endif

signed char backlight_state = -1;

void
backlight_init (void)
{
    LOG_INFO("Initializing backlight...");

    // Configure backlight state to off.
    backlight_state = 0;

    // Set our backlight pin.
    // Only rev2 of the pcb has one so this should probably be in an ifdef
    pin_disable_analog(BACKLIGHT_ANSEL, BACKLIGHT_PIN);
    pin_set_low(BACKLIGHT_LAT, BACKLIGHT_PIN);
    pin_set_output(BACKLIGHT_TRIS, BACKLIGHT_PIN);
}

void
backlight_set (signed char state)
{
    LOG_DEBUG("Setting backlight to: %i", state);

    switch (state)
    {
        case BACKLIGHT_ON:
            pin_set_high(BACKLIGHT_LAT, BACKLIGHT_PIN);
            backlight_state = 1;
        break;

        case BACKLIGHT_OFF:
            pin_set_low(BACKLIGHT_LAT, BACKLIGHT_PIN);
            backlight_state = 0;
        break;

        case BACKLIGHT_TOGGLE:
            if (backlight_state)
            {
                // Backlight is currently on, turn off
                pin_set_low(BACKLIGHT_LAT, BACKLIGHT_PIN);
                backlight_state = 0;
            }
            else
            {
                // Backlight is currently off, turn on
                pin_set_high(BACKLIGHT_LAT, BACKLIGHT_PIN);
                backlight_state = 1;
            }
        break;
    }
}

signed char
backlight_get (void)
{
    LOG_DEBUG("Backlight value is: %i", backlight_state);

    return backlight_state;
}


// EOF //
