/** @file timers.c
 * Driver for the various timers on the PIC16F1919x.
*/

#include <xc.h>

#include "drivers/timers.h"


void
timer0_init (void)
{
    // 16-bit mode
    T0CON0bits.T016BIT = 1;

    // default postscaler 1:1
    T0CON0bits.T0OUTPS = 0b0000;
    
    // default prescaler 1:32768
    T0CON1bits.T0CKPS = 0b1111;

    // not syncd with Fosc to allow for operation during sleep
    T0CON1bits.T0ASYNC = 1;

#   if (1 == PCB_REV)
    // clock src LFINTOSC
    T0CON1bits.T0CS = 0b100;

#   elif (2 == PCB_REV)
    // clock src SOSC
    T0CON1bits.T0CS = 0b110;
#   endif
}

// EOF //
