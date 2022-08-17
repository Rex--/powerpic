/** @file temperature.h
 * 
 * This library implements reading the internal temperature sensor and
 * converting it to degrees.
 * 
 * NOTE: The ADC must be initialized before using this lib. Currently this is
 * done by the battery_init() function so no initialization procedure is needed
 * as long as the battery library is initialized at startup.
*/

#ifndef _temperature_h_
#define _temperature_h_

float
temperature_read (void);

#endif

// EOF //