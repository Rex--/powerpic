/** @file battery.h
 * 
 * This library implements functionality relating to the battery in the watch.
 * 
 * The goal is to determine the current battery level. We do this currently by
 * just sampling the voltage with the built-in ADC.
 * 
*/

#ifndef _battery_h_
#define _battery_h_


/**
 * Initialize the battery library.
*/
void
battery_init (void);

/**
 * Gets an approximate value for the battery voltage.
 *
 * We return a float here, but maybe an int representation would be better.
*/
float
battery_read_voltage (void);

#endif

// EOF //
