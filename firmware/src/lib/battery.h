/** @file
 * Battery Library for the PowerPIC.
 * 
 * This library provides functions to determine the current battery status.
*/

#ifndef BATTERY_H
#define BATTERY_H

void        battery_enable (void);

void battery_disable(void);

unsigned int    battery_read_voltage (void);

#endif

/*** EOF ***/
