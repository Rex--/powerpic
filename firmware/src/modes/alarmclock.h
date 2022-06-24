/** @file alarmclock.h
 * 
 * This mode implements a configurable alarm and an hourly chime.
 * 
*/

#ifndef _alarmclock_h_
#define _alarmclock_h_

#define ALARMCLOCK_EVENT_CHIME          0x0C
#define ALARMCLOCK_EVENT_DAILY          0x0D

void            alarmclock_init  (void);
void            alarmclock_start (void);
signed char     alarmclock_run   (unsigned int event);
signed char     alarmclock_edit  (unsigned int event);
void            alarmclock_stop  (void);
void            alarmclockd      (unsigned int event);

mode_config_t alarmclock_config = {
        .tickrate = 65535000,
        .keymap   = KEYMAP_CASIO,
};

mode_app_t alarmclock_mode = {
        "alarmclock",
        &alarmclock_config,
        &alarmclock_init,
        &alarmclock_start,
        &alarmclock_run,
        &alarmclock_stop,
        &alarmclockd
};

#endif

// EOF //
