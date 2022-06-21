/** @file interrupts.h
 * Interrupt Driver.
*/

#ifndef _interrupts_h_
#define _interrupts_h_


/** Enable Global interrupts.
 * This function-like macro enables all interrupts. The corresponding interrupt
 * enable flag must be set before it will start generating an interrupt.
*/
#define interrupts_global_enable() (INTCONbits.GIE = 1)

/**
 * Disable Global interrupts.
*/
#define interrupts_global_disable() (INTCONbits.GIE = 0)


/** Enable Peripherial interrupts.
 * This function-like macro enables peripherials to generate interrupts. The
 * corresponding peripherial's interrupt must be enabled seperatly.
*/
#define interrupts_peripherial_enable() (INTCONbits.PEIE = 1)

/**
 * Disable Peripherial interrupts.
*/
#define interrupts_peripherial_disable() (INTCONbits.PEIE = 0)


#endif

// EOF //
