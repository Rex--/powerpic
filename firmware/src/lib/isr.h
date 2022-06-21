/** @file isr.h
 * ISR Library for PowerOS.
 * This library handles all interrupts. For each interrupt generated,
 * the registered 'int_flag's are checked against it's 'int_mask' and the
 * corresponding function is called. It is up to the registered functions to
 * clear their interrupt flags.
 * 
 * This library assumes that exactly one isr will be registered for each
 * interrupt generated.
 * 
 * Macros are provided to enable/disable global and peripherial interrupts.
*/

#ifndef _isr_h_
#define _isr_h_

////////////////////////////////////////
// Lib Config //

#define MAX_SERVICE_ROUTINES    8

////////////////////////////////////////



// Include macros to enable/disable interrupts.
//
#include "drivers/interrupts.h"


/**
 * Register a function to be run as an isr.
 * The function should return as quickly as possible.
 * 
 * @param[in] int_flag The PIRx register of the interrupt.
 * @param[in] int_mask The bitmask of the interrupt flag(s).
 * @param[in] int_func The function to be run for the isr.
 * 
 * @returns The index of the isr function in the callstack. -1 if error.
*/
signed char
isr_register (
    unsigned char int_flag,
    unsigned char int_mask,
    void (*int_func)(void)
);

/**
 * Unregisters an isr.
 * This removes your place in the callstack, re-registering will place the
 * function at the end.
 * 
 * @param[in] isr_index The index of the isr function in the callstack.
*/
void    isr_unregister (signed char isr_index);

#endif

// EOF //
