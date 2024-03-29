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
// #include "modes/test.h"
#include "modes/clock.h"
#include "modes/timer.h"
#include "modes/uptime.h"
#include "modes/power.h"
// #include "modes/thermometer.h"
#include "modes/alarmclock.h"
#include "modes/settings.h"


enum mode_app_index {
#ifdef _test_h_
    MODE_TEST,
#endif
#ifdef _clock_h_
    MODE_CLOCK,
#endif
#ifdef _timer_h_
    MODE_TIMER,
#endif
#ifdef _uptime_h_
    MODE_UPTIME,
#endif
#ifdef _power_h_
    MODE_POWER,
#endif
#ifdef _thermometer_h_
    MODE_THERMOMETER,
#endif
#ifdef _alarmclock_h_
    MODE_ALARMCLOCK,
#endif
#ifdef _settings_mode_h_
    MODE_SETTINGS,
#endif
    MODE_MAX_MODES,
};

mode_app_t *mode_list[MODE_MAX_MODES] = {
#ifdef _test_h_
    &test_mode,
#endif
#ifdef _clock_h_
    &clock_mode,
#endif
#ifdef _timer_h_
    &timer_mode,
#endif
#ifdef _uptime_h_
    &uptime_mode,
#endif
#ifdef _power_h_
    &power_mode,
#endif
#ifdef _thermometer_h_
    &thermometer_mode,
#endif
#ifdef _alarmclock_h_
    &alarmclock_mode,
#endif
#ifdef _settings_mode_h_
    &settings_mode,
#endif
};

#endif

// EOF //
