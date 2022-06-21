/** @file isr.c
 * ISR Library for PowerOS.
*/

#include <xc.h>

#include "lib/isr.h"


#define LOG_TAG "ISR"
#include "lib/logging.h"


/** ISR function prototype. */
typedef void (*isr_func_t)(void);

/** Structure to hold information about each ISR.
 * 
*/
typedef struct
{
    unsigned char flag;
    unsigned char mask;
    isr_func_t isr_func;
} isr_t;

/** Pointer to the first of 8 peripherial interrupt flag bytes. */
static volatile unsigned char * pir_registers = &PIR0;

/** Pointer to the first of 8 peripherial interrupt enable bytes. */
static volatile unsigned char * pie_registers = &PIE0;

/** Array to hold currently registered ISRs. */
static volatile isr_t interrupts_service_routines[MAX_SERVICE_ROUTINES];

/** Number of ISRs currently registered. */
static volatile signed char interrupts_registered = 0;


signed char
isr_register (unsigned char int_flag, unsigned char int_mask, isr_func_t int_func)
{
    isr_t isr_to_register = {
        .flag = int_flag,
        .mask = int_mask,
        .isr_func = int_func
    };

    signed char isr_index = -1;
    if (MAX_SERVICE_ROUTINES >= interrupts_registered)
    {
        isr_index = interrupts_registered++;
        interrupts_service_routines[isr_index] = isr_to_register;
    }
    else
    {
        LOG_ERROR("Max ISRs added already!");
    }
    return isr_index;
}


void
isr_unregister (signed char isr_index)
{
    interrupts_global_disable();
    for (; isr_index < interrupts_registered; isr_index++)
    {
        interrupts_service_routines[isr_index] = \
            interrupts_service_routines[isr_index+1];
    }
    interrupts_registered--;
    interrupts_global_enable();
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
}

// EOF //
