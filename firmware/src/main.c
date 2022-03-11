/** @file main.c
 *
 * Main program for the PowerPIC firmware.
 *
 * @par
 * TODO: Write a nice description about the PowerPIC firmware.
 * 
*/

#include <stdio.h>
#include <xc.h>

#include "dev_config.h"
#include "drivers/uart.h"

/**
 * Main Interrupt Service Routine.
 * 
 * Calls other ISRs.
*/
void
__interrupt() main_isr (void)
{
    // Check if the Peripherial Interrupts are enabled.
    //
    if (1 == INTCONbits.PEIE) {

        // USART1 Transmit Interrupt flag
        //
        if (1 == PIE3bits.TX1IE && 1 == PIR3bits.TX1IF)
        {
            uart_tx_isr();
        }

        // USART1 Receive Interrupt Flag
        //
        else if (1 == PIE3bits.RC1IE && 1 == PIR3bits.RC1IF)
        {
            uart_rx_isr();
        }

        else
        {
            // Unhandled Interrupt
        }
    }
    else
    {
        // Unhandled Interrupt
    }

    return;
}   /* main_isr() */


/**
 * Helper function that prints a string followed by a newline and
 * carriage return.
 * 
 * NOTE: Does not support printf-style arguments.
 * 
 * @param[in] data String to be printed.
*/
void println(char *data)
{
    printf("%s\n\r", data);
}


/**
 * Entrypoint of main program.
 * 
 * This configures the device drivers and starts the Mode Manager.
*/
void
main (void)
{   
    // Init uart connection
    uart_init();

    // Enable interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    println("Hello World!");

    uint8_t data = 0;
    int bytes_read = 0;

    for ( ;; )
    {
        bytes_read = scanf("%c", &data);
        if (1 <= bytes_read)
        {
            printf("Received char: %c\n\r", data);
        }
    }
}

/*** EOF ***/
