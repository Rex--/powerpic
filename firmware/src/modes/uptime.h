/** @file uptime.h
 * 
 * This mode implements an uptime counter. It attempts to give an idea of the
 * lifetime of a battery by giving a number of days since last battery change.
*/

#ifndef _uptime_h_
#define _uptime_h_

#define UPTIME_ALARM_EVENT 0xAE

void            uptime_init  (void);
void            uptime_start (void);
signed char     uptime_run   (unsigned int event);
void            uptime_stop  (void);
void            uptimed (unsigned int event);

mode_app_t uptime_mode = {
    "uptime",
    &uptime_init,
    &uptime_start,
    &uptime_run,
    &uptime_stop,
    &uptimed
};

#endif

// EOF //
