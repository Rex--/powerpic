/** @file timers.c
 * Driver for the various timers on the PIC16F1919x.
 * 
 * The timers currently in use:
 * - Timer 0 is used for the main tick timer. This tick drives the main loop to
 *      call a mode at the set tick rate.
 * - Timer 1 is used for button debouncing.
 * - Timer 4 is used for PWM to drive the buzzer.
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

#   else // (2 == PCB_REV)
    // clock src SOSC
    T0CON1bits.T0CS = 0b110;
#   endif
}

void
timer1_init (void)
{
    T1CONbits.CKPS = 0b11;  // 1:8 prescaler
    // T1CONbits.nSYNC = 0; // Synchronized with Fosc
    T1CLKbits.CS = 0b0010; // Set clock source to Fosc
}

void
timer4_init (void)
{
    T4CLKCONbits.CS = 0b0001;         // clock - Fosc/4
    T4HLTbits.PSYNC = 1;            // Synced with Fosc
}

// EOF //
