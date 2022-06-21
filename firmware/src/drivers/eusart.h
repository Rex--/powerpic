/** @file eusart.h
 * Enhanced Universal Synchronous Asynchronous Receiver Transmitter Driver.
 * The pic we are using has two eusarts available, eusart1 and eusart2, that
 * operate independently of one another.
 * Currently, only eusart1 is used for log output during development. This
 * means that receving is not implemented by this driver.
*/


#ifndef _eusart_h_
#define _eusart_h_

/** Initialize eusart1 at the specified baudrate.
 * Note: Typical baudrates are not supported and this driver makes no effort
 * to determine which one is meant. We can, however, get close enough for
 * several:
 *   9600 - 9615   (1, 4, 8 MHz)
 *   9600 - 9604   (32 MHz)
 * 
 *  19200 - 19230  (1, 4, 8 MHz)
 *  19200 - 19180  (32 MHz)
 * 
 * 115200 - 115900 (32 MHz)
*/
void    eusart1_init (unsigned long baudrate);

#define eusart1_baudrate_set(baudrate) \
    SP1BRG = (unsigned int)((_XTAL_FREQ / baudrate) / 4) - 1

#define eusart1_write(data) TX1REG = data

#endif

// EOF //
