/** @file uart.h
 *
 * UART Driver library for PIC16LF1919x devices.
 *
 * This driver implements an 8-byte buffer for both receiving and transmitting.
 * Function are provided that interface with the standard library `stdio.h`.
 * After setup of the driver, you are encouraged to use the `printf()` and
 * `scanf()` functions from the standard library.
 *
 * This library also includes functions for interfacing directly with the
 * UART driver. These should only be used under special circumstances.
*/

#ifndef UART_H
#define UART_H

int    uart_init (void);

// These functions implement functions relating to the Transmitter.
//
void    uart_write (uint8_t data);
void    uart_tx_isr (void);


// These functions implement functions relating to the Receiver.
//
uint8_t uart_read (void);
void    uart_rx_isr (void);


// These function implement stub functions for stdio.h to hook into and provide
// support for printf() and scanf().
//
void    putch (uint8_t data);
int     getch (void);

#endif /* UART_H */

/*** EOF ***/
