/** @file settings.h
 * 
 * This mode provides a way to configure watch settings.
 * 
 * Settings include:
 *  1. rtc - RTCC Drift calibration. -128 to 127. Positive value for slow
 *          clock, negative value for fast.
*/

#ifndef _settings_mode_h_
#define _settings_mode_h_

void            settings_init (void);
void            settings_start (void);
signed char     settings_run   (unsigned int event);

mode_app_t settings_mode = {
    .id="settings",
    .init=&settings_init,
    .start=&settings_start,
    .run=&settings_run,
};

#endif

// EOF //
