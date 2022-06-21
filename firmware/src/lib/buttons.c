/** @file buttons.c
 * 
 * This library implements the functionality of the side buttons.
*/

#include <xc.h>

#include "drivers/pins.h"
#include "drivers/ioc.h"

#include "lib/isr.h"
#include "lib/events.h"

#include "lib/buttons.h"


static unsigned char button_mode = 0;
static unsigned char button_adj  = 0;

static void     buttons_isr (void);

void
buttons_init (void)
{
    button_mode = 0;
    button_adj = 0;

    // Disable IOC interrupts while configuring pins.
    //
    ioc_interrupt_disable();

#   if (1 == PCB_REV)
#   define BUTTON_MODE_MASK     0b00100000
#   define BUTTON_MODE          PORTCbits.RC5
#   define BUTTON_MODE_INT      IOCCFbits.IOCCF5
    // Mode button: RC5
    //
    pin_set_input(TRISC, 5);
    pin_enable_pullup(WPUC, 5);
    pin_disable_analog(ANSELC, 5);

#   define BUTTON_ADJ_MASK     0b00010000
#   define BUTTON_ADJ          PORTCbits.RC4
#   define BUTTON_ADJ_INT      IOCCFbits.IOCCF4
    // Adj button: RC4
    //
    pin_set_input(TRISC, 4);
    pin_enable_pullup(WPUC, 4);
    pin_disable_analog(ANSELC, 4);

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
    pin_set_input(TRISB, 6);
    pin_enable_pullup(WPUB, 6);
    pin_disable_analog(ANSELB, 6);

#   define BUTTON_ADJ           PORTBbits.RB7
#   define BUTTON_ADJ_INT       IOCBFbits.IOCBF7
#   define BUTTON_ADJ_MASK      0b10000000
    // Adj button: RB7
    //
    pin_set_input(TRISB, 7);
    pin_enable_pullup(WPUB, 7);
    pin_disable_analog(ANSELB, 7);

    // Enable interrupt for falling and rising edges of Mode and Adj buttons.
    //
    ioc_pin_enable(IOC_PORTB, 6, IOC_EDGE_BOTH);
    ioc_pin_enable(IOC_PORTB, 7, IOC_EDGE_BOTH);
#endif

    isr_register(0, _PIR0_IOCIF_MASK, &buttons_isr);

    ioc_interrupt_enable();
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
    if (BUTTON_MODE_INT)
    {
        if (BUTTON_MODE)
        {
            if (1 == button_mode)
            {
                // Release event
                //
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_MODE_RELEASE));
                button_mode = 0;
            }
        }
        else
        {
            if (0 == button_mode)
            {
                // Press event
                //
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_MODE_PRESS));
                button_mode = 1;
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
            if (1 == button_adj)
            {
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_ADJ_RELEASE));
                button_adj = 0;
            }
        }
        else
        {
            if (0 == button_adj)
            {
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_ADJ_PRESS));
                button_adj = 1;
            }
        }
        BUTTON_ADJ_INT = 0;
    }
}

// EOF //
