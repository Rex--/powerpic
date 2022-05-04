/** @file buttons.c
 * Button library for CA-53W watches.
 * 
 * This library enables the use of the two side buttons "Mode/C" and "ADJ".
 * Each button press and release emits an event that gets eventually passed to
 * a mode for handling. Currently the "Mode" button events are interected by the
 * mode manager for switching modes and will not be passed on to any mode.
*/

#include <xc.h>
#include <stdio.h>

#include "drivers/ioc.h"
#include "drivers/pins.h"

#include "lib/event.h"
#include "lib/isr.h"

#include "lib/buttons.h"


static unsigned char button_mode_state = 0;
static unsigned char button_adj_state = 0;


static void buttons_isr (void);

/**
 * Enable the buttons.
 * After enabling, each button press and release will start to generate events.
*/
void
buttons_enable (void)
{
    button_mode_state = 0;
    button_adj_state = 0;

    // Configure pins for side buttons
    
    // Mode button: RC5
    //
    // TRISCbits.TRISC5 = 1;
    pin_mode_input2(PORTC, PIN5);
    WPUCbits.WPUC5 = 1;

    // Adj button: RC4
    //
    // TRISCbits.TRISC4 = 1;
    pin_mode_input2(PORTC, PIN4);
    WPUCbits.WPUC4 = 1;

    // Disable IOC interrupts while configuring pins.
    //
    IOC_DISABLE;

    // Enable interrupt for falling and rising edges of Mode and Adj buttons.
    //
    ioc_pin_enable(IOC_PORTC, 5, IOC_EDGE_BOTH);
    ioc_pin_enable(IOC_PORTC, 4, IOC_EDGE_BOTH);

    isr_register(0, _PIR0_IOCIF_MASK, &buttons_isr);

    IOC_ENABLE;
}


/**
 * Button Interrupt Service Routine.
 * This services the IOC interrupt and determines if a button pin was the
 * trigger, emitting an event if so.
*/
static void
buttons_isr (void)
{
    // Mode button
    if (IOCCFbits.IOCCF5)
    {
        if (PORTCbits.RC5)
        {
            if (1 == button_mode_state)
            {
                // Release event
                //
                event_add(BUTTON_EVENT, BUTTON_MODE_RELEASE);
                button_mode_state = 0;
            }
        }
        else
        {
            if (0 == button_mode_state)
            {
                // Press event
                //
                event_add(BUTTON_EVENT, BUTTON_MODE_PRESS);
                button_mode_state = 1;
            }
        }
        IOCCFbits.IOCCF5 = 0;
    }

    // Adj Button
    //
    if (IOCCFbits.IOCCF4)
    {
        if (PORTCbits.RC4)
        {
            if (1 == button_adj_state)
            {
                event_add(BUTTON_EVENT, BUTTON_ADJ_RELEASE);
                button_adj_state = 0;
            }
        }
        else
        {
            if (0 == button_adj_state)
            {
                event_add(BUTTON_EVENT, BUTTON_ADJ_PRESS);
                button_adj_state = 1;
            }
        }
        IOCCFbits.IOCCF4 = 0;
    }
}
