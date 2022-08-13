/** @file power.h
 * 
 * This mode implements a battery voltage monitor.
*/

#ifndef _power_h_
#define _power_h_

void            power_start (void);
signed char     power_run   (unsigned int event);
void            power_stop  (void);

mode_app_t power_mode = {
        "power",
        NULL,
        &power_start,
        &power_run,
        &power_stop,
};

#endif

// EOF //