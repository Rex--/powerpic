/** @file main.c
 *
 * Main program for the PowerPIC firmware.
 *
 * TODO: Write a nice description about the PowerPIC firmware.
 * 
*/

#include <xc.h>

#include "post.h"

#include "lib/system.h"
#include "lib/isr.h"
#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/datetime.h"
#include "lib/alarm.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/backlight.h"
#include "lib/buzzer.h"
#include "lib/display.h"
#include "lib/battery.h"

#define LOG_TAG "main"
#include "lib/logging.h"


/**
 * Setup of hardware and libs on POR.
 * This function gets called exactly once at every reset event.
*/
void
setup (void)
{
    // Initialize the base system.
    // - Oscillator (xtal)
    // - Default pin states (pins)
    //
    system_init();

    // Init logging lib so it is available for use by libs.
    logging_init();

    // Initialize peripheral libraries:
    // - Timers     (tick.h)
    // - RTCC       (datetime.h)
    //   - Alarm    (alarm.h)
    // - LCD        (display.h)
    // - Keypad     (keypad.h)
    // - Buttons    (buttons.h)
    // - Buzzer     (buzzer.h)
    // - Backlight  (backlight.h)
    // - ADC        (battery.h)
    //
    tick_init();
    datetime_init();
    alarm_init();
    display_init();
    keypad_init();
    buttons_init();
    buzzer_init();
    backlight_init();
    battery_init();
}

/**
 * Entrypoint of firmware.
 * This contains the main loop.
*/
void
main (void)
{
    setup();

    post_bios();

    // Set a default datetime.
    // (Eventually we should load this from EEPROM or something)
    datetime_t fourtwenty = {
        .date = {0x22, 0x04, 0x20, 0x00},
        .time = {0x16, 0x19, 0x00},
    };
    datetime_set(&fourtwenty);

    // Initialize our modes. This calls each mode's init() function. It then
    // starts the default mode (whatever is at mode_list index 0) and configures
    // the system according to the mode's config.
    mode_init();

    // Update display in case started mode drew something
    display_update();

    // Tickrate was configured above, so we can enable ticks.
    // This could be moved to the init() call above.
    tick_enable();

    LOG_INFO("Starting main loop...");
    for( ;; )
    {
        if (event_check())
        {
            // If we have an event in the queue, we call the mode thread to
            // handle it.
            mode_thread();
            
            // Update the display after running the mode.
            display_update();
        }

        // Go to sleep. An interrupt will wake us up when something happens.
        SLEEP();

        // The instruction after sleep is always executed first when waking up.
        // This NOP makes sure we don't do anything weird.
        NOP();
    }
}


// EOF //
