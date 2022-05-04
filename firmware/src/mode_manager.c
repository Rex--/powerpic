/** @file mode_manager.c
 * 
 * Manages switching between Mode Applications.
*/

#include <xc.h>
#include <stdio.h>

#include "lib/event.h"
#include "lib/mode.h"
#include "lib/isr.h"
#include "lib/buttons.h"    // TODO: remove

#include "modes/test_mode.h"
#include "modes/power.h"
#include "modes/clock.h"
#include "modes/counter.h"

#include "mode_manager.h"

#define MAX_MODES    8

static int added_modes = 0;

static int current_mode = 0;

static int next_mode = 0;

static int adj_mode = 0;


mode_app_t       managed_modes[MAX_MODES];
mode_config_t    managed_configs[MAX_MODES];


int mode_manager_add (char *id, init_t init, start_t start, run_t run, stop_t stop);

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
    current_mode = -1;
    next_mode = 0;

    // Add modes to be managed
    //
    mode_manager_add(CLOCK_MODE);
    mode_manager_add(TEST_MODE);
    mode_manager_add(POWER_MODE);
    mode_manager_add(COUNTER_MODE);

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
    mode_start(&managed_modes[next_mode], &managed_configs[next_mode]);
    current_mode = next_mode;

    return;
}   /* mode_manager_init() */


const mode_config_t mode_config_default = {
    .keymap =   0,
    .tickrate = 0,
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

            mode_start(&managed_modes[next_mode], &managed_configs[next_mode]);

            current_mode = next_mode;
        }
    }
    else
    {
        // Pass event to mode app
        mode_run(&managed_modes[current_mode], &managed_configs[current_mode], event);
    }
}


int
mode_manager_config_update (void)
{
    return 1;
}

int
mode_manager_get_tickrate (void)
{
    return managed_configs[current_mode].tickrate;
}

int
mode_manager_get_keymap (void)
{
    return managed_configs[current_mode].keymap;
}

/*** EOF ***/
