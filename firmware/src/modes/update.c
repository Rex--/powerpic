/** @file update.c
 * 
 * This mode provides an easy way for the user to get into the bootloader.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"

#define LOG_TAG "mode.update"
#include "lib/logging.h"

#include "modes/update.h"

static unsigned char update_reboot = 0;

void
update_init (void)
{
    update_reboot = 0;
}

void
update_start (void)
{
    display_secondary_string(1, "UP");
    display_primary_string(2, "UPdAtE");
}

signed char
update_run (unsigned int event)
{
    unsigned int timerDur;
    switch (EVENT_TYPE(event))
    {
    case EVENT_BUTTON:
        switch (EVENT_DATA(event))
        {
        case BUTTON_ADJ_PRESS:
            if (update_reboot) {
                update_reboot = 0;
                display_primary_string(2, "UPdAtE");
            } else {
                update_reboot = 1;
                display_primary_string(2, "rEboot");
            }
        break;
        case BUTTON_MODE_PRESS:
            if (update_reboot) {    // Confirm update
                RESET();    // Reset device
            } else {
                return 1; // Cycle to next mode
            }
        break;
        }
    break;

    case KEYPAD_EVENT_PRESS:
        if (update_reboot) {
            RESET();
        }
    break;
    
    default:
    break;
    }
    return 0;
}

void
update_stop (void)
{

}

// EOF //
