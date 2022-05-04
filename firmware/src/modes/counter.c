/** @file
 * This Counter Mode is for testing and development.
 * Displays a count of how many tims the main thread has been ran since reset.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/display.h"

#include "modes/counter.h"

static long number_count = 0;

void
counter_init (mode_config_t *cfg)
{
    cfg->tickrate = -1;

    number_count = 0;
}

void
counter_start (mode_config_t *cfg)
{
    display_primary_clear(0);
    display_primary_number(0, number_count);
}

void
counter_thread (mode_config_t *cfg, unsigned int event)
{
    display_primary_number(0, ++number_count);
}
