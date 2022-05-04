/** @file isr.h
 * Interrupt library for PowerOS.
*/

#ifndef ISR_H
#define ISR_H

#define ISR_MAX_SERVICE_ROUTINES     8

typedef void (*isr_func_t)(void);

int isr_register (unsigned char int_flag, unsigned char int_mask, isr_func_t int_func);

void isr_unregister (int isr_index);

void isr_execute (void);

#endif

/*** EOF ***/
