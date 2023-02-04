/** @file timer.h
 *
 * This mode implements various timers.
*/

#ifndef _timer_h_
#define _timer_h_

void            timer_init  (void);
void            timer_start (void);
signed char     timer_run   (unsigned int event);
void            timer_stop  (void);

mode_app_t timer_mode = {
    "timer",
    &timer_init,
    &timer_start,
    &timer_run,
    &timer_stop,
};

#endif

// EOF //
