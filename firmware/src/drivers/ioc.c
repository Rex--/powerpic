/** @file ioc.c
 * Interrupt on Change driver library for PIC16LF1919x Devices.
 * 
 * This driver provides a couple handy function to configure the IOC module.
 * Each needed pin should be enabled and then a call to IOC_ENABLE will enable
 * interrupts.
*/

#include <xc.h>

#include "drivers/ioc.h"

/**
 * Initialize the interrupt on change driver.
 * 
 * This should be called before use of the IOC module.
*/
void
ioc_init (void) {
    return;
}   /* ioc_init() */

/**
 * Enables interrupts for a specific pin.
 * In the current v1.1 board only PORTC is used.
 * 
 * @param   port    PORTx of the pin to enable.
 * @param   pin     Number of the pin to enable.
 * @param   edge    Edge of change to enable.
 * 
 * Use the included macros to define a port and edge.
*/
void
ioc_pin_enable (unsigned char port, unsigned char pin, signed char edge)
{
    switch (port)
    {
#       ifdef IOC_PORTB        
        case IOC_PORTB:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCBP |= (1U << pin);
                IOCBN |= (1U << pin);
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCBN |= (1U << pin);
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCBP |= (1U << pin);
            }
        break;
#       endif

#       ifdef IOC_PORTC
        case IOC_PORTC:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCCP |= (1U << pin);
                IOCCN |= (1U << pin);
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCCN |= (1U << pin);
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCCP |= (1U << pin);
            }
        break;
#       endif        

#       ifdef IOC_PORTE
        case IOC_PORTE:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCEP |= (1U << pin);
                IOCEN |= (1U << pin);
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCEN |= (1U << pin);
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCEP |= (1U << pin);
            }
        break;
#       endif

    }
}   /* ioc_pin_enable() */
