/** @file interrupts.h
 * 
 * Interrupt Driver Library for PIC16LF1919x devices.
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/**
 * Enable Peripherial interrupts.
 * 
 * This function-like macro enables peripherials to generate interrupts. The
 * corresponding peripherial's interrupt must be enabled seperatly, this
 * normally happens in the `<driver>_init()` function.
*/
#define INTERRUPTS_PERIPHERIAL_ENABLE (INTCONbits.PEIE = 1)

/**
 * Disable Peripherial interrupts.
 * 
 * This function-like macro disables peripherial interrupts.
*/
#define INTERRUPTS_PERIPHERIAL_DISABLE (INTCONbits.PEIE = 0)

/**
 * Enable Global interrupts.
 * 
 * This function-like macro enables all interrupts. The corresponding interrupt
 * enable flag must be set before it will start generating an interrupt.
*/
#define INTERRUPTS_GLOBAL_ENABLE (INTCONbits.GIE = 1)

/**
 * Disable Global interrupts.
 * 
 * This function-like macro disables global interrupts.
*/
#define INTERRUPTS_GLOBAL_DISABLE (INTCONbits.GIE = 0)


#endif  /* INTERRUPTS_H */

/*** EOF ***/