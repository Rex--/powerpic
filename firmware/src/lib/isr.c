/** @file isr.c
 * Interrupt library for PowerOS.
*/

#include <xc.h>
#include <stdio.h>

#include "lib/isr.h"


typedef struct
{
    int index;
    unsigned char flag;
    unsigned char mask;
    isr_func_t isr_func;
} isr_t;

static volatile unsigned char * pir_registers = &PIR0;
static volatile unsigned char * pie_registers = &PIE0;

static volatile isr_t interrupts_service_routines[ISR_MAX_SERVICE_ROUTINES];
static volatile int interrupts_registered = 0;


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
int isr_register (unsigned char int_flag, unsigned char int_mask, isr_func_t int_func)
{
    isr_t isr_to_register;

    isr_to_register.flag = int_flag;
    isr_to_register.mask = int_mask;
    isr_to_register.isr_func = int_func;

    int isr_index = -1;
    if (ISR_MAX_SERVICE_ROUTINES >= interrupts_registered)
    {
        isr_index = interrupts_registered++;
        isr_to_register.index = isr_index;
        interrupts_service_routines[isr_index] = isr_to_register;
    }
    else
    {
        // Unhandled error: Not registered because limit reached.
        printf("[ISR] ERROR: Max ISRs added already!\n\r");
    }
    return isr_index;
}

/**
 * Unregisters an isr.
 * 
 * @param[in] isr_index The index of the isr function in the callstack.
*/
void
isr_unregister (int isr_index)
{
    di();
    for (; isr_index < interrupts_registered; isr_index++)
    {
        interrupts_service_routines[isr_index] = \
            interrupts_service_routines[isr_index+1];
    }
    interrupts_registered--;
    ei();
}

/**
 * XC8 specific way of defining an Interrupt Service Routine.
*/
static void __interrupt() isr_thread (void);

/**
 * Main Interrupt Service Routine.
 * 
 * Calls other ISRs as needed by the firmware.
*/
static void
isr_thread (void)
{
    isr_t isr_to_call;

    for (int isr = 0; isr < interrupts_registered; isr++)
    {
        isr_to_call = interrupts_service_routines[isr];
        if (isr_to_call.flag == 254)
        {
            isr_to_call.isr_func();
        }
        else
        {
            if ((pie_registers[isr_to_call.flag] & isr_to_call.mask) && \
                (pir_registers[isr_to_call.flag] & isr_to_call.mask))
            {
                isr_to_call.isr_func();
            }
        }
    }

    return;
}   /* _isr() */

/*** EOF ***/
