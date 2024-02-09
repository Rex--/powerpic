/** @file settings.h
 * 
 * This mode provides a way to configure watch settings.
 * Each setting can be edited as a 8-bit byte, with an ID of 0-255.
 * 
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
