/** @file stopwatch.h
 *
 * This mode implements a stopwatch and various other timers.
*/

#ifndef _stopwatch_h_
#define _stopwatch_h_

void            stopwatch_init  (void);
void            stopwatch_start (void);
signed char     stopwatch_run   (unsigned int event);
void            stopwatch_stop  (void);

mode_app_t stopwatch_mode = {
    "stopwatch",
    &stopwatch_init,
    &stopwatch_start,
    &stopwatch_run,
    &stopwatch_stop,
};

#endif

// EOF //
