/** @file
 * EUSART Driver for PIC16LF1919x Devices.
*/

#include <xc.h>

#include "dev_config.h"

// It'd be real nice to calculate SP1BRG at compilation
//
//#define BAUDRATE      115200

/**
 * Initialize the EUSART1 transmitter.
 * 
*/
void
eusart1_tx_init (void)
{
    // Before configuring the USART module, the PORT pins need to be configured
    // as well as the Peripherial Pin Select (PPS) module.
    // Currently the pins used aren't configurable, they are:
    //
#   if (1 == PCB_REV)
    TRISCbits.TRISC3 = 0;   // Set RC3(TX) as output
    RC3PPS = 0x0D;          // x0D is the value for TX for USART1
#   endif

#   if (2 == PCB_REV)
    TRISCbits.TRISC4 = 0;   // Set RC4(TX) as output
    RC4PPS = 0x0D;          // x0D is the value for TX for USART1
#   endif

    // The following registers control the baudrate generation for the EUSART 1
    // module:
    // BAUD1CON - Baud rate control register
    //   ABDOVF:0 | RCIDL:1:r | 0 | SCKP:0 | BRG16:0 | 0 | WUE:0 | ABDEN: 0
    //     BRG16  - 16-bit Baud Rate Generator
    // SP1BRG - Baud Rate Generator value
    //   SP1BRGH | SP1BRGL
    //     SP1BRGL  - Lower 8-bits of the Baud Rate Generator
    //     SP1BRGH  - Upper 8-bits of the Baud Rate Generator
    // 
    // We want to use the 16-bit generator and set SP1BRG to 12 to achieve our
    // desired baud of 115230 @ 32MHz clock speed- which is close enough to 115200.
    //
    BAUD1CON = 0b01001000;
    SP1BRGL = 68;

    // The following registers configure the Transmit and Receive modules:
    // 
    // TX1STA - Transmit status and control register
    //   0 | TX9:0 | TXEN:0 | SYNC:0 | 0 | BRGH:0 | TRMT:1:r | TX9D:0
    //     TXEN - Transmit Enable Bit
    //     BRGH - High Baud Rate Select Bit
    // RC1STA - Receive status
    //  and control register
    //   SPEN:0 | RX9:0 | 0 | CREN:0 | ADDEN:0 | FERR:0:r | OERR:0:r | RX9D:0:r
    //     SPEN - Serial Port Enable Bit
    //     CREN - Continuous Receive Enable Bit (RX not implemented)
    //
    TX1STA = 0b00100100;
    RC1STA = 0b10000000;
}

/*** EOF ***/
