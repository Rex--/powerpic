/** @file mode.c
 * 
 * This library implements the types and procedures used by Mode applications.
*/

#include <xc.h>

#include "lib/events.h"
#include "lib/tick.h"
#include "lib/buttons.h"
#include "lib/keypad.h"

#include "modes/mode_config.h"

#include "lib/mode.h"

#define LOG_TAG "lib.mode"
#include "lib/logging.h"

/** This value holds the currently selected mode. */
static unsigned char mode_selected = 0;

/** This value holds the current mode config that is applied to the system. */
static mode_config_t mode_config = {0};


/**
 * Update the config based on a given mode's index.
 * This function updates the configuration values based on the mode's config.
 * It then copies these configuration values to mode_config. It makes no
 * attempt to keep in sync with the current selected mode.
*/
static void
mode_config_update (unsigned char mode);


void
mode_init (void)
{
    LOG_INFO("Initializing %i modes", MODE_MAX_MODES);

    // Loop over all the modes in the mode_list
    //
    for (int i = 0; i < MODE_MAX_MODES; i++)
    {
        LOG_DEBUG("Initializing mode: %s", mode_list[i]->id);

        // Call the mode's init function.
        mode_list[i]->init();
    }

    // We don't need the tickrate or keymap set because no events will be
    // passed to the startup function.
    // NOTE: What if an event happens between init and now?
    // mode_config_update(0);

    // Configure and Start our default mode (index 0)
    //
    mode_list[0]->start();

    // We configure tickrate etc after we start the mode in case the
    // configuration gets changed in the startup routine.
    //
    mode_config_update(0);

}

void
mode_next (void)
{
    // Stop the tick timer to prevent interrupts.
    tick_disable();
    
    // Go ahead and reset the tick counter for the next mode.
    tick_counter_reset();

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
    mode_list[mode_selected]->stop();

    // Set config values based on next mode.
    mode_config_update(mode_selected_next);

    // Start the next mode.
    //
    mode_list[mode_selected_next]->start();

    // Set configuration values if they have changed.
    mode_config_update(mode_selected_next);

    // Set new selected mode.
    //
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
        else
        {
            // Update configuration after calls to run() in case they've changed.
            mode_config_update(mode_selected);
        }

        event = event_get();
    }
}



static void
mode_config_update (unsigned char mode)
{
    LOG_DEBUG("Updating Mode config values: %i", mode);
    // Set config values based on the given index
    // mode_config = *mode_list[mode]->config;

    // Set mode's desired tickrate.
    //
    if (mode_list[mode]->config->tickrate != tick_rate_get())
    {
        // Only set the tickrate if it's changed.
        //
        mode_config.tickrate = mode_list[mode]->config->tickrate;
        tick_rate_set(mode_config.tickrate);

        LOG_DEBUG("Updating tickrate: %li", mode_config.tickrate);
    }
    else
    {
        // We still reset the timer to get a full tick.
        //
        tick_reset();

        // LOG_DEBUG("Retting tick: %li", mode_config.tickrate);
        // LOG_DEBUG("Tickrate: %li", tick_rate_get());
    }

    // Set mode's desired keymap
    //
    if (mode_list[mode]->config->keymap != keypad_keymap_get())
    {
        // Only set keymap if it's changed
        //
        mode_config.keymap = mode_list[mode]->config->keymap;
        keypad_keymap_set(mode_config.keymap);

        LOG_DEBUG("Updating keymap: %i", mode_config.keymap);
    }
}

// EOF //
