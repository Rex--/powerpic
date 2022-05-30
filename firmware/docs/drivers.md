@page drivers Device Drivers
[TOC]

Drivers interface directly with the PIC hardware control registers to provide
some simple functions to manage the peripherial. Drivers typically will not be
accessed directly from a Mode application. Checkout the [library](#library)
page for the higher level interface for usage by Modes.

Several Device drivers are available for use:

![](driver-components.svg)

@section drivers_manager Driver Manager

The Driver Manager is responsible for enabling and configuring the needed device
drivers. It should be configured by the Mode Manager prior to using any of the
supported driver peripherials. Each driver provides an init() function that gets
called from the Driver Manager.


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
- `uart_tx_isr()` - Transmit Interrupt Service Routine

and 

- `uart_rx_isr()` - Receive Interrupt Service Routine

These are called by the driver's main ISR:
- `uart_isr()` - Driver Interrupt Service Rountine

The Interrupt Service Routine provided function as follows:

![](driver-uart-isr.svg)\n

@subsection drivers_uart_rw Read and Write

Some functions are exposed to interact directly with the UART Driver. While
these *can* be used, it is preferred for Mode applications to use the standard
i/o functions provided by `stdio.h`. This provides a common interface layer so
backend drivers can be easily exchanged, say for e.g. debugging. These functions
are eventually called through wrapper functions `getch()` and `putch()`

![](driver-uart-read.svg)![](driver-uart-write.svg)

@section drivers_fvr FVR Driver

```c
// FVR Driver Library
//
#include "drivers/fvr.h"
```

The FVR driver provides an interface to interact with the fixed voltage
reference availblee for use by peripherials. To enable the FVR call the macro
`FVR_ENABLE`. This will set the FVREN bit of the FVRCON register. A stub
function is also provided to keep a common interface amoung drivers:
`fvr_init()` which just calls the enable macro.

@subsection drivers_fvr_adc Output to ADC

The FVR has 2 outputs available to the ADC. They are configured as follows:
- `fvr_adc_set()` - Set adc buffer gain. Available as Buffer 1 on the ADC.
- `fvr_cda_set()` - Set cda buffer gain. Available as Buffer 2 on the ADC.
- `fvr_xxx_set(FVR_BUFFER_OFF);` - Disable 'xxx' buffer.

@subsection drivers_fvr_temperature Temperature Sensor

The FVRCON register controls the built-in temperature sensor. It can be
configured between high and low range. High range provides better resolution
over the range, but requires a higher Vdd.\n
The temperature sensor can be configured as follows:
- `fvr_temperature_sensor_set(uint8_t temperature_settings)`

`temperature_settings` being on of:
- `FVR_TEMPERATURE_OFF` - Disable the sensor.
- `FVR_TEMPERATURE_LOW` - Enable the sensor in low mode.
- `FVR_TEMPERATURE_HIGH` - Enable the sensor in high mode.