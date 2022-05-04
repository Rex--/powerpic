/** @file pins.c
 * I/O Pin Driver for PIC16LF1919x Devices.
 * 
*/

#include <xc.h>


/**
 * Initializes output pins to a default state.
 * 
 * This configures all pins as outputs and sets them low so we can start from
 * a known state.
 * 
 * TODO: Actually do that.
*/
void
pins_init (void)
{

}


/**
 * Set pin to act as an input.
 * 
 * Control I/O functionality.
 * 
 * @param[in]   port The PORTx of the pin to configure.
 * @param[in]   pin The pin number of the port.
*/
inline void
pin_mode_input (volatile unsigned char port, const unsigned char pin)
{
    port |= (1 << pin);
}

/**
 * Set pin to act as an output.
 * 
 * @param[in]   port The PORTx of the pin to configure.
 * @param[in]   pin The pin number of the port.
*/
inline void
pin_mode_output (volatile unsigned char port, const unsigned char pin)
{
    port &= ~(1 << pin);
}
