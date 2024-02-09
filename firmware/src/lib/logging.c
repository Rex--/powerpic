/** @file logging.c
 * Logging Library for PowerOS.
*/

#include <xc.h>

#include "drivers/eusart.h"

#include "lib/isr.h"
#include "lib/logging.h"

/** Length of UART transmit buffer in bytes */
#define UART_TX_BUFFER_SIZE    64

static volatile unsigned char uart_tx_buffer_head = 0;
static volatile unsigned char uart_tx_buffer_tail = 0;
static volatile unsigned char uart_tx_buffer_remaining = 0;
static volatile unsigned char uart_tx_buffer[UART_TX_BUFFER_SIZE] = {0};

static void uart_tx_isr (void);

void
logging_init (void)
{
    // Init eusart1 module.
    eusart1_init(LOGGING_UART_BAUDRATE);

    // Set up the default driver state.
    uart_tx_buffer_head = 0;
    uart_tx_buffer_tail = 0;
    uart_tx_buffer_remaining = sizeof(uart_tx_buffer);

    // Register UART isr
    //
    isr_register(3, _PIE3_TX1IE_MASK, &uart_tx_isr);
}

void
putch (char data)
{
    while (0 == uart_tx_buffer_remaining)
    {
        // Wait until there is room in the buffer. This should take max the
        // amount of time to transmit a single byte.
    }

    // If the TX1IE is not set transmission interrupts have been disabled.
    // They are only enabled when we have previously written to TX1REG, so if
    // they are disabled we can immediatly write this byte to the TX1REG.
    //
    if (0 == PIE3bits.TX1IE)
    {
        TX1REG = data;
    }

    // If the TX1IE bit is set it means we have some data in tx_buffer already
    // queued up. If there's room we add it tx_buffer, if not we block and wait.
    //
    else
    {

        // Disable the TX interrupt before adding our data to the buffer.
        //
        PIE3bits.TX1IE = 0;

        uart_tx_buffer[uart_tx_buffer_head] = data;

        // Next we increment the tx_buffer_head, wrapping around if neccessary,
        // and decrement tx_buffer_remaining.
        //
        uart_tx_buffer_head++;
        if (sizeof(uart_tx_buffer) <= uart_tx_buffer_head)
        {
            uart_tx_buffer_head = 0;
        }
        uart_tx_buffer_remaining--;
    }

    // Enable the transmission interrupt- meaning we have more data in the
    // buffer to send.
    //
    PIE3bits.TX1IE = 1;
}

unsigned char
logging_buffer_empty (void)
{
    if (sizeof(uart_tx_buffer) == uart_tx_buffer_remaining)
    {
        return 1;
    }

    return 0;
}


static void
uart_tx_isr (void)
{
    // Check if there is any data in the tx_buffer
    //
    if (sizeof(uart_tx_buffer) > uart_tx_buffer_remaining)
    {
        // Write the tail to TX1REG
        //
        TX1REG = uart_tx_buffer[uart_tx_buffer_tail];

        // Increment tx_buffer_tail, wrapping around if neccessary.
        //
        uart_tx_buffer_tail++;
        if (sizeof(uart_tx_buffer) <= uart_tx_buffer_tail)
        {
            uart_tx_buffer_tail = 0;
        }
        uart_tx_buffer_remaining++;
    }
    // If the buffer is empty, disable the Transmit Interrupt.
    //
    else
    {
        PIE3bits.TX1IE = 0;
    }
}


// EOF //
