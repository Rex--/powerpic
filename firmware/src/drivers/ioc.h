/** @file ioc.h
 * Interrupt on Change driver for PIC16LF1919x Devices.
*/

#ifndef IOC_H
#define IOC_H

#define IOC_PORTC           0

#define IOC_EDGE_BOTH      -1
#define IOC_EDGE_FALLING    0
#define IOC_EDGE_RISING     1

#define IOC_ENABLE          (PIE0bits.IOCIE = 1)
#define IOC_DISABLE         (PIE0bits.IOCIE = 0)

void ioc_init               (void);

void ioc_pin_enable         (unsigned char port, unsigned char pin, signed char edge);

#endif
