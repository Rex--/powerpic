/** @file mode.c
 * 
 * This library implements the types and procedures used by Mode applications.
*/

#include <xc.h>

#include "lib/events.h"
#include "lib/tick.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/display.h"

#include "modes/mode_config.h"

#include "lib/mode.h"

#define LOG_TAG "lib.mode"
#include "lib/logging.h"

/** This value holds the currently selected mode. */
static unsigned char mode_selected = 0;

/**
 * Reset various libs to defaults.
 * This reset libraries that are configurable by mode applications including:
 * - tickrate
 * - keypad keymap
*/
static void
mode_config_defaults (void);


void
mode_init (void)
{
    LOG_INFO("Initializing %i modes", MODE_MAX_MODES);

    // Loop over all the modes in the mode_list
    //
    for (int i = 0; i < MODE_MAX_MODES; i++)
    {
        if (mode_list[i]->init != NULL)
        {
            LOG_DEBUG("Initializing mode: %s", mode_list[i]->id);

            // Call the mode's init function.
            mode_list[i]->init();
        }
    }

    // Reset libs n' things to a default state
    mode_config_defaults();

    // Start our default mode (index 0)
    mode_list[0]->start();
}

void
mode_next (void)
{
    // Disable to prevent tick events.
    tick_disable();

    // Get the next mode's index in the list
    //
    unsigned char mode_selected_next = mode_selected + 1;
    if (MODE_MAX_MODES == mode_selected_next)
    {
        mode_selected_next = 0;
    }

    LOG_INFO(
        "Switching modes [%u]%s -> [%u]%s",
        mode_selected,
        mode_list[mode_selected]->id,
        mode_selected_next,
        mode_list[mode_selected_next]->id
    );

    // Stop the mode that is currently selected.
    if (mode_list[mode_selected]->stop != NULL)
    {
        mode_list[mode_selected]->stop();
    }

    // Reset libs n' things to a default state
    mode_config_defaults();

    // Start the next mode.
    if (mode_list[mode_selected_next]->start != NULL)
    {
        mode_list[mode_selected_next]->start();
    }

    // Set new selected mode.
    mode_selected = mode_selected_next;
}

void
mode_thread (void)
{
    unsigned int event = event_get();

    // Check if an event is in the queue.
    while (event)
    {
        LOG_DEBUG("Handling event: x%.4x", event);

        // Call mode's run function with the event.
        if (mode_list[mode_selected]->run(event))
        {
            // If the mode returns 1, we switch to the next mode
            mode_next();
        }

        // Pass the event to every registered daemon
        for (int i = 0; i < MODE_MAX_MODES; i++)
        {
            if (mode_list[i]->daemon)
            {
                LOG_DEBUG("Running daemon: %i", i);
                mode_list[i]->daemon(event);
            }
        }

        event = event_get();
    }
}


static void
mode_config_defaults (void)
{
    // Default tick settings: Disabled
    tick_disable();
    tick_counter_reset();
    
    // Default keypad keymap: Casio
    keypad_keymap_set(KEYMAP_CASIO);

    // Default Display: Blank primary, secondary, and punctuations displays.
    // The sign & misc displays are not touched so they can be 'notifications'.
    display_primary_clear(0);
    display_secondary_clear(0);
    display_period_clear(0);
}

// EOF //
