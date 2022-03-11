@page drivers Device Drivers
[TOC]

Drivers interface directly with the PIC hardware control registers to provide
some simple functions to manage the peripherial. Drivers typically will not be
accessed directly from a Mode application. Checkout the [library](#library)
page for the higher level interface for usage by Modes.

Several Device drivers are available for use:

![](driver-components.svg)


@section drivers_uart UART Driver

```c
#include <stdio.h>
#include "drivers/uart.h"
    
```

This UART driver library provides some functions that enable standard library
`stdio.h` functionality. After setting up the module, standard `printf` and
`scanf` functions can be used. \n
This driver makes the assumption that the system
clock is running at 1MHz and the desired baudrate is 9600 bps.

To use this driver the main application must:
1. Configure clock speed to 1MHz.
2. Initialize the driver.
3. Enable Peripherial and Global interrupts.
4. Call both transmit and receive interrupt handlers from the main ISR.


@subsection drivers_uart_init Initialization and Configuration

To initialize this driver, all one must do is call `uart_init()`. There are no
configuration options at this time. Here's an example of initializing the UART
Driver and enabling the interrupts:
```c
int
main (void)
{
    uart_init();            // Initialize UART driver
    INTCONbits.PEIE = 1;    // Enable Peripherial Interrupts
    INTCONbits.GIE  = 1;    // Enable Global Interrupts
}   /* main() */
```


@subsection drivers_uart_isr Interrupt Service Routines

Two interrupt handlers are need for the driver to function properly:
- `uart_tx_isr()` - Transmit Interrupt service routine
and 
- `uart_rx_isr()` - Receive Interrupt service routine

These should be called from the main Interrupt Service routine if either of
their corresponding interrupt enable bits are set.\n
**Example:**
```c
void __interrupt()
main_isr (void)
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
}   /* main_isr() */
```

The Interrupt Service Routines provided function as follows:

![](driver-uart-rx-isr.svg)![](driver-uart-tx-isr.svg)

@subsection drivers_uart_rw Read and Write

Some functions are exposed to interact directly with the UART Driver. While
these *can* be used, it is preferred for Mode applications to use the standard
i/o functions provided by `stdio.h`. This provides a common interface layer so
backend drivers can be easily exchanged, say for e.g. debugging. These functions
are eventually called through wrapper functions `getch()` and `putch()`

![](driver-uart-read.svg)![](driver-uart-write.svg)


<!-- @section drivers_ioc Interrupt On Change Driver

`#include drivers/ioc.h`

The IOC driver provides functions to enable and disable peripherial interrupts.
This module simply controls the flow of interrupts and doesn't act on them in
any way. -->