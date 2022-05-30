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

    // Disable IOC interrupts while configuring pins.
    //
    IOC_DISABLE;

#   if (1 == PCB_REV)
#   define BUTTON_MODE_MASK     0b00100000
#   define BUTTON_MODE          PORTCbits.RC5
#   define BUTTON_MODE_INT      IOCCFbits.IOCCF5
    // Mode button: RC5
    //
    // pin_mode_input2(PORTC, PIN5);
    TRISCbits.TRISC5 = 1;
    WPUCbits.WPUC5 = 1;
    ANSELCbits.ANSC5 = 0;

#   define BUTTON_ADJ_MASK     0b00010000
#   define BUTTON_ADJ          PORTCbits.RC4
#   define BUTTON_ADJ_INT      IOCCFbits.IOCCF4
    // Adj button: RC4
    //
    // pin_mode_input2(PORTC, PIN4);
    TRISCbits.TRISC4 = 1;
    WPUCbits.WPUC4 = 1;
    ANSELCbits.ANSC4 = 0;

    // Enable interrupt for falling and rising edges of Mode and Adj buttons.
    //
    ioc_pin_enable(IOC_PORTC, 5, IOC_EDGE_BOTH);
    ioc_pin_enable(IOC_PORTC, 4, IOC_EDGE_BOTH);
#   endif

#   if (2 == PCB_REV)
#   define BUTTON_MODE          PORTBbits.RB6
#   define BUTTON_MODE_INT      IOCBFbits.IOCBF6
#   define BUTTON_MODE_MASK     0b01000000
    // Mode button: RB6
    //
    TRISBbits.TRISB6 = 1;
    WPUBbits.WPUB6 = 1;
    ANSELBbits.ANSB6 = 0;

#   define BUTTON_ADJ           PORTBbits.RB7
#   define BUTTON_ADJ_INT       IOCBFbits.IOCBF7
#   define BUTTON_ADJ_MASK      0b10000000
    // Adj button: RB7
    //
    TRISBbits.TRISB7 = 1;
    WPUBbits.WPUB7 = 1;
    ANSELBbits.ANSB7 = 0;

    // Enable interrupt for falling and rising edges of Mode and Adj buttons.
    //
    ioc_pin_enable(IOC_PORTB, 6, IOC_EDGE_BOTH);
    ioc_pin_enable(IOC_PORTB, 7, IOC_EDGE_BOTH);
#endif

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
#   if DEV_BUILD
    TXREG1 = 'b';
#   endif

    // Mode button
    if (BUTTON_MODE_INT)
    {
        if (BUTTON_MODE)
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
        BUTTON_MODE_INT = 0;
    }

    // Adj Button
    //
    if (BUTTON_ADJ_INT)
    {
        if (BUTTON_ADJ)
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
        BUTTON_ADJ_INT = 0;
    }
}
