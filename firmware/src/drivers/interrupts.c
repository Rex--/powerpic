/** @file interrupts.c
 * 
 * Interrupt driver for PIC16LF1919x devices.
 * 
 * This Driver controls the flow of interrupts. Each peripherial driver should
 * enable the needed interupts by setting the corresponding PIE bit. Drivers
 * should only manipulate the specific bits needed by means of direct bit access 
 * via bitfield structs e.g. `INTCONbits.GIE` or bitwise OR operations ` |= `.
 * 
*/

#include <xc.h>

#include "drivers/interrupts.h"

/*** EOF ***/