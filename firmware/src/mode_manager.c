/** @file mode_manager.c
 * 
 * Manages switching between Mode Applications.
*/

#include <xc.h>
#include <stdio.h>

#include "lib/event.h"
#include "lib/mode.h"
#include "lib/isr.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"

#include "modes/test_mode.h"
#include "modes/power.h"
#include "modes/clock.h"
#include "modes/thermometer.h"
#include "modes/settings.h"

#include "mode_manager.h"

#define MAX_MODES    8

static int added_modes = 0;

static int current_mode = 0;

static int next_mode = 0;

static int adj_mode = 0;



mode_app_t       managed_modes[MAX_MODES];
mode_config_t    managed_configs[MAX_MODES];
static mode_config_t    current_mode_config;


int mode_manager_add (char *id, init_t init, start_t start, run_t run, stop_t stop);
void mode_manager_update_tickrate (void);
void mode_manager_update_keymap (void);
void mode_manager_update_font (void);

/**
 * Initializes the Mode Manager for Managing Modes.
 * 
 * TODO: Should this start a default mode or should that be left to a start function?
 * 
*/
void
mode_manager_init (void)
{
    // Init default state:
    // No added modes
    // Current mode is something not equal to next mode.
    // Next mode to start is 0
    //
    added_modes = 0;
    current_mode = 0;
    next_mode = 0;

    // Add modes to be managed
    //
    mode_manager_add(CLOCK_MODE);
    mode_manager_add(THERMOMETER_MODE);
    mode_manager_add(POWER_MODE);
    mode_manager_add(TEST_MODE);
    mode_manager_add(SETTINGS_MODE);

    // Init modes
    //
    printf("Initializing %i modes:\n\r", added_modes);
    for (int imode = 0; imode < added_modes; imode++)
    {
        printf("[%i] ", imode);
        mode_init(&managed_modes[imode], &managed_configs[imode]);
    }

    // Start default mode
    //
    // current_mode_config = managed_configs[next_mode];
    mode_manager_update_tickrate();
    mode_manager_update_keymap();
    mode_manager_update_font();
    mode_start(&managed_modes[next_mode], &managed_configs[next_mode]);

    return;
}   /* mode_manager_init() */


const mode_config_t mode_config_default = {
    .keymap =   0,
    .tickrate = 0,
    .font = 0,
};

int
mode_manager_add (char *id, init_t init, start_t start, run_t run, stop_t stop)
{
    int new_mode_index = 0;

    if (MAX_MODES > added_modes)
    {
        new_mode_index = added_modes++;
        mode_app_t new_mode = {
            .id = id,
            .init = init,
            .start = start,
            .run = run,
            .stop = stop,
        };
        managed_modes[new_mode_index] = new_mode;
        managed_configs[new_mode_index] = mode_config_default;
    }
    else
    {
        // Error: Max modes have been added
        //
        new_mode_index = -1;
    }
    return new_mode_index;
}


void
mode_manager_thread (unsigned int event)
{
    if (event == ((BUTTON_MODE_PRESS << 8) | BUTTON_EVENT) )
    {
        printf("[MM]Button Event: (%x)\n\r", event);

        // Figure out next mode
        //
        next_mode = current_mode + 1;
        if (next_mode >= added_modes)
        {
            next_mode = 0;
        }

        if (current_mode != next_mode)
        {
            printf("[MM]Switching Modes: [%i] -> [%i]\n\r", current_mode, next_mode);

            mode_stop(&managed_modes[current_mode], &managed_configs[current_mode]);

            display_segments_zero();

            // current_mode_config = managed_configs[next_mode];

            mode_manager_update_tickrate();
            mode_manager_update_keymap();
            mode_manager_update_font();

            mode_start(&managed_modes[next_mode], &managed_configs[next_mode]);

            current_mode = next_mode;
        }
    }
    else
    {
        // Pass event to mode app
        mode_run(&managed_modes[current_mode], &managed_configs[current_mode], event);

        // Check if mode changed config
        //
        if (managed_configs[current_mode].tickrate != current_mode_config.tickrate)
        {
            mode_manager_update_tickrate();
        }
        
        if (managed_configs[current_mode].keymap != current_mode_config.keymap)
        {
            mode_manager_update_keymap();
        }
        
        if (managed_configs[current_mode].font != current_mode_config.font)
        {
            mode_manager_update_font();
        }

        // current_mode_config = managed_configs[current_mode];
    }
}

void
mode_manager_update_tickrate (void)
{
    printf("Updating tick to %u from %u\n\r", managed_configs[next_mode].tickrate, current_mode_config.tickrate);
    current_mode_config.tickrate = managed_configs[next_mode].tickrate;
    if (0 < current_mode_config.tickrate)
    {
        printf("Setting new tickrate\n\r");
        tick_stop();
        tick_set_ms(current_mode_config.tickrate);
        tick_enable_interrupts();
        tick_reset();
        tick_start();
    }
    else
    {
        printf("Disabling tickrate\n\r");
        tick_stop();
        tick_disable_interrupts();
    }
}

void
mode_manager_update_keymap (void)
{
    current_mode_config.keymap = managed_configs[next_mode].keymap;
    keypad_keymap_set(current_mode_config.keymap);
}

void
mode_manager_update_font (void)
{
    current_mode_config.font = managed_configs[next_mode].font;
    display_set_font(current_mode_config.font);
}


// int
// mode_manager_config_update (void)
// {

//     return mode_config_needs_update;
// }

// int
// mode_manager_get_tickrate (void)
// {
//     return managed_configs[current_mode].tickrate;
// }

// int
// mode_manager_get_keymap (void)
// {
//     return managed_configs[current_mode].keymap;
// }

// unsigned char
// mode_manager_get_font (void)
// {
//     return managed_configs[current_mode].font;
// }

/*** EOF ***/
