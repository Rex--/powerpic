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

/**
 * Read the current cpu temperature.
 * 
 * This function returns a fairly inaccurate measurement.
 * 
 * @returns Temperature in degrees Celsius.
*/
int
temperature_read (void);

/**
 * Calibrate the temperature sensor for a more accurate result.
 * 
 * This function calibrates the temperature sensor given the current
 * temperature in degrees. This should give a more accurate result than just
 * using the factory 90C calibration value.
 * 
 * NOTE: This value is not persistent across resets.
 * 
 * @param[in]   degrees     The current temperature to calibrate to.
*/
void
temperature_calibrate (int degrees);

#endif

// EOF //
