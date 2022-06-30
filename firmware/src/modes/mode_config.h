/** @file mode_config.h
 * 
 * Configuration of modes.
 * 
 * This files contains global configuration that applies to all modes,
 * including which modes to build into the firmware.
 * 
 * To add a mode:
 *  1. Place mode source files in modes/
 *  2. Include header file containing the mode_app
 *  3. Add mode index to mode_app_index
 *  4. Add mode application object to mode_list
*/

#ifndef _mode_config_h_
#define _mode_config_h_

#include "lib/mode.h"       // We need the mode typedefs
#include "modes/test.h"
#include "modes/alarmclock.h"
#include "modes/clock.h"


enum mode_app_index {
    MODE_CLOCK,
    MODE_ALARMCLOCK,
    MODE_TEST,
    MODE_MAX_MODES,
};

static mode_app_t *mode_list[MODE_MAX_MODES] = {
    &clock_mode,
    &alarmclock_mode,
    &test_mode,
};

#endif

// EOF //
