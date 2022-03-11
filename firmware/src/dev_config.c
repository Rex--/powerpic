/** @file dev_config.c
 * This file contains the configuration words.
*/

#include "dev_config.h"

/*** Configuration Word 1 ***/

// LCD Charge Pump Enable Bit.
//
#pragma config LCDPEN = OFF

// Power-up Oscillator mode - High freq internal oscillator at 1MHz
//
#pragma config RSTOSC = HFINT1

// External Oscillator mode - External oscillator not enabled
//
#pragma config FEXTOSC = OFF

/*** End of Config Word 1 ***/


/*** Configuration Word 3 ***/

// Disable WatchDog Timer
//
#pragma config WDTE = OFF

/*** End of Config Word 3 ***/

/*** EOF ***/
