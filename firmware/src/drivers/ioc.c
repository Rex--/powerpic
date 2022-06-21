/** @file ioc.c
 * Interrupt on Change Driver.
*/

#include <xc.h>

#include "drivers/ioc.h"



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
}


// EOF //