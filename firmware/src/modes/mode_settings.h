/** @file mode_settings.h
 * 
 * Global settings defined by modes.
 * 
 * To reserve a setting ID for a mode, add it to the enum contained in this
 * header. This will allow other modes to reference this setting, however
 * other modes may also change the value.
*/

#ifndef _mode_settings_h_
#define _mode_settings_h_

enum global_settings_mode {
    SETTING_CLOCK_FMT,      // 0/1 for 24/12 hour clock format
    SETTING_RTC_OFFSET,     // RTCCAL value
    SETTING_TEMP_CAL_1H,    // Temperature calibration reading at 25C
    SETTING_TEMP_CAL_1L,    // 12-bit value stored as 2 bytes H:L
    MODE_SETTINGS_MAX
};

#endif

// EOF //
