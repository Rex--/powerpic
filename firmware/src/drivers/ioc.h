/** @file ioc.h
 * Interrupt on Change Driver.
 * This driver is used to configure the IOC module for the 4 keypad columns and
 * the 2 side buttons (Mode and Adj).
 * 
 * In v1 boards only PORTC is used.
 * In v2 PORTC and PORTB are used.
*/

#ifndef _ioc_h_

#define IOC_PORTB           1
#define IOC_PORTC           2
#define IOC_PORTE           4

#define IOC_EDGE_BOTH      -1
#define IOC_EDGE_FALLING    0
#define IOC_EDGE_RISING     1

/**
 * Enable IOC interrupts.
 * After enabling the wanted pin change interrupt, the IOC interrupt must be
 * enabled to generate interrupts.
*/
#define ioc_interrupt_enable()          PIE0bits.IOCIE = 1

/**
 * Disable IOC interrupts.
*/
#define ioc_interrupt_disable()         PIE0bits.IOCIE = 0


/**
 * Enable edge triggered interrupts for multiple pins in a port.

 * 
 * @param   port    PORTx of the pin to enable.
 * @param   mask    Mask of pins to enable.
 * @param   edge    Edge of change to enable.
 * 
 * Use the included macros to define a port and edge.
*/
void ioc_mask_enable         (unsigned char port, unsigned char mask, signed char edge);

/**
 * Enable edge triggered interrupts for a single pin.
 * 
 * @param   port    PORTx of the pin to enable.
 * @param   pin     Number of the pin to enable.
 * @param   edge    Edge of change to enable.
 * 
 * Use the included macros to define a port and edge.
*/
#define ioc_pin_enable(port, pin, edge)    ioc_mask_enable(port, (1 << pin), edge)

#endif

// EOF //
