/** @file eusart.h
 * Enhanced Universal Synchronous Asynchronous Receiver Transmitter C Driver.
*/

#include <xc.h>

#include "drivers/pins.h"

#include "drivers/eusart.h"


void
eusart1_init (unsigned long baudrate)
{
    // Configure pins
#   if (1 == PCB_REV)
    pin_set_output(TRISC, 3);   // Set RC3(TX) as output
    RC3PPS = 0x0D;              // x0D is the value for TX for USART1
#   endif

#   if (2 == PCB_REV)
    pin_set_output(TRISC, 4);   // Set RC4(TX) as output
    RC4PPS = 0x0D;              // x0D is the value for TX for USART1
#   endif

    // High speed baudrate generator
    TX1STAbits.BRGH = 1;

    // 16-bit baudrate generator
    BAUD1CONbits.BRG16 = 1;

    eusart1_baudrate_set(baudrate);

    // Enable TX
    TX1STAbits.TXEN = 1;

    // Enable EUSART1
    RC1STAbits.SPEN = 1;
}


// EOF //
