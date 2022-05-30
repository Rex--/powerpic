/** @file uart.c
 *
 * UART driver for PIC16LF1919x devices using Hardware EUSART1.
 *
 * This driver implements an 8-byte buffer for both receiving and transmitting.
 * Function are provided that interface with the standard library `stdio.h`.
 * After setup of the driver, you are encouraged to use the `printf()` and
 * `scanf()` functions from the standard library.
*/

#include <stdint.h>
#include <xc.h>

#include "drivers/eusart.h"

#include "lib/isr.h"

#include "lib/uart.h"

#define UART_TX_BUFFER_SIZE    8    /**< Length of UART transmit buffer in bytes */
#define UART_RX_BUFFER_SIZE    8    /**< Length of UART receive buffer in bytes */


static volatile uint8_t uart_tx_buffer_head = 0;
static volatile uint8_t uart_tx_buffer_tail = 0;
static volatile uint8_t uart_tx_buffer_remaining = 0;
static volatile uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE] = {0};

static volatile uint8_t uart_rx_buffer_head = 0;
static volatile uint8_t uart_rx_buffer_tail = 0;
static volatile uint8_t uart_rx_buffer_remaining = 0;
static volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE] = {0};


static void uart_tx_isr (void);
static void uart_rx_isr (void);



/**
 * Initialize the UART driver with default settings.
 * Currently supports no configuration options. Assumes a 32MHz clock speed and
 * 115200 baud serial connection. After this is called standard i/o functions
 * from stdio.h can be used such as printf().
 * 
*/
void
uart_init (void)
{
    // Enable EUSART module transmit
    //
    eusart1_tx_init();

    // Set up the default TX state.
    //
    uart_tx_buffer_head = 0;
    uart_tx_buffer_tail = 0;
    uart_tx_buffer_remaining = sizeof(uart_tx_buffer);

    // Register UART isr
    //
    isr_register(3, _PIE3_TX1IE_MASK, &uart_tx_isr);


#   ifdef UART_RX
    // Set up the default TX state.
    //
    uart_rx_buffer_head = 0;
    uart_rx_buffer_tail = 0;
    uart_rx_buffer_remaining = 0;

    // Register UART isr
    //
    isr_register(3, _PIE3_RC1IE_MASK, &uart_rx_isr);

    // Enable Receive interrupt
    //
    PIE3bits.RC1IE = 1;
#   endif

    // TODO: Add some error checking to make sure USART Module has actually been
    // enabled and is ready for use.
    //
    return;
}   /* uart_init() */


/**
 * Write a byte of data to the currently configured UART.
 * 
 * This driver provides an 8-byte receive buffer in addition to the hardware
 * module's 1-byte buffer.
 * 
 * @param[in] data The data byte to be written.
*/
void
uart_write (uint8_t data)
{
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
        while (0 == uart_tx_buffer_remaining)
        {
            // Wait until there is room in the buffer. This should take max the
            // amount of time to transmit a single byte.
        }

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

    return;
}   /* uart_write() */


#ifdef UART_RX
/**
 * Read a byte of data from the currently configured UART.
 * 
 * This driver provides an 8-byte receive buffer in addition to the hardware
 * module's 2-byte buffer.
 * 
 * @returns The byte of data that was read.
*/
uint8_t
uart_read (void)
{
    uint8_t data = 0;

    while (0 == uart_rx_buffer_remaining)
    {
        // Block and wait if we don't have any data in the rx_buffer
    }

    data = uart_rx_buffer[uart_rx_buffer_tail];

    // Increment rx_buffer_tail, wrapping if neccessary.
    //
    uart_rx_buffer_tail++;
    if (sizeof(uart_rx_buffer) <= uart_rx_buffer_tail)
    {
        uart_rx_buffer_tail = 0;
    }

    // Disable interrupts while we change the amount remaining.
    //
    PIE3bits.RC1IE = 0;
    uart_rx_buffer_remaining--;
    PIE3bits.RC1IE = 1;

    return data;
}
#endif


/**
 * The Interrupt Service Handler for the Transmit Interrupt Flag.
 * 
 * This handles writing of data to TX1REG from the uart_tx_buffer. It should be
 * called by the main ISR when TX1IE is enabled and TX1IF is triggered.
*/
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

    return;
}   /* uart_tx_isr() */


#ifdef UART_RX
/**
 * The Interrupt Service Handler for the Receive Interrupt Flag.
 * 
 * This handles reading of the RC1REG register into rx_buffer. It should be
 * called by the main ISR when RC1IE is enabled and RC1IF is triggered.
*/
static void
uart_rx_isr (void)
{
    // Read a byte of data from the receive register
    //
    uart_rx_buffer[uart_rx_buffer_head] = RC1REG;

    // Increment rx_buffer_head
    //
    uart_rx_buffer_head++;
    if (sizeof(uart_tx_buffer) <= uart_rx_buffer_head)
    {
        uart_rx_buffer_head = 0;
    }
    uart_rx_buffer_remaining++;
}   /* uart_rx_isr() */
#endif


/**
 * This implements an interface for the standard library stdio.h to hook into.
 * 
 * Stub wrapper for uart_write().
 * 
 * @param[in] data Data byte to be written.
*/
void
putch (uint8_t data)
{
    uart_write(data);
    return;
}   /* putch() */


#ifdef UART_RX
/**
 * This implements an interface for the standard library stdio.h to hook into.
 * 
 * Stub wrapper for uart_read().
 * 
 * @returns The byte of data read. Blocks if no data available.
*/
int
getch (void)
{
    return abs(uart_read());
}   /* getch() */
#endif

/*** EOF ***/
