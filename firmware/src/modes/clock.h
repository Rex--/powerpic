/** @file clock.h
 * 
 * This mode implements a clock face that displays the current time.
*/

#ifndef _clock_h_
#define _clock_h_

void            clock_init  (void);
void            clock_start (void);
signed char     clock_run   (unsigned int event);
signed char     clock_edit  (unsigned int event);
void            clock_stop  (void);

mode_app_t clock_mode = {
        "clock",
        &clock_init,
        &clock_start,
        &clock_run,
        &clock_stop,
};

#endif

// EOF //
