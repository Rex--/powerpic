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
    }
}   /* ioc_pin_enable() */

/**
 * Enables interrupts for a PORT.
 * In the v1.1 board only PORTC is used.
 * In v2.0 PORTC and PORTB are used.
 * 
 * @param   port    PORTx of the pin to enable.
 * @param   mask    Mask of pins to enable.
 * @param   edge    Edge of change to enable.
 * 
 * Use the included macros to define a port and edge.
*/
void
ioc_mask_enable (unsigned char port, unsigned char mask, signed char edge)
{
    switch (port)
    {
        case IOC_PORTB:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCBP |= mask;
                IOCBN |= mask;
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCBN |= mask;
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCBP |= mask;
            }
        break;

        case IOC_PORTC:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCCP |= mask;
                IOCCN |= mask;
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCCN |= mask;
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCCP |= mask;
            }
        break;

        case IOC_PORTE:
            if (IOC_EDGE_BOTH == edge)
            {
                IOCEP |= mask;
                IOCEN |= mask;
            }
            else if (IOC_EDGE_FALLING == edge)
            {
                IOCEN |= mask;
            }
            else if (IOC_EDGE_RISING == edge)
            {
                IOCEP |= mask;
            }
        break;
    }
}   /* ioc_mask_enable() */