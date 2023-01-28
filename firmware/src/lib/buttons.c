/** @file buttons.c
 * 
 * This library implements the functionality of the side buttons.
*/

#include <xc.h>

#include "drivers/pins.h"
#include "drivers/ioc.h"
#include "drivers/timers.h"

#include "lib/isr.h"
#include "lib/events.h"

#include "lib/buttons.h"

#define DEBOUNCE_PERIOD 10

static unsigned char button_mode_pressed = 0;
static unsigned char button_adj_pressed  = 0;

// Timer1 timestamp of last event
static unsigned int last_event_time = 0;

static void     buttons_isr (void);

void
buttons_init (void)
{
    button_mode_pressed = 0;
    button_adj_pressed = 0;
    last_event_time = 0;

    // Init timer
    timer1_init();

    // Start timer
    // TODO: We should only start the timer when a button (or keypad) is down
    timer1_start();
    

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

#   else   // (2 == PCB_REV)
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
 * 
 * Buttons are active low.
*/
static void
buttons_isr (void)
{
    // Mode button
    if (BUTTON_MODE_INT)
    {
        if (BUTTON_MODE)
        {
            if (1 == button_mode_pressed && (timer1_get() - last_event_time) > DEBOUNCE_PERIOD)
            {
                // Release event
                //
                last_event_time = timer1_get();
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_MODE_RELEASE));
                button_mode_pressed = 0;
            }
        }
        else
        {
            // Always detect push events if button is not already pressed
            if (0 == button_mode_pressed)
            {
                // Press event
                //
                last_event_time = timer1_get();
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_MODE_PRESS));
                button_mode_pressed = 1;
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
            if (1 == button_adj_pressed && (timer1_get() - last_event_time) > DEBOUNCE_PERIOD)
            {
                last_event_time = timer1_get();
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_ADJ_RELEASE));
                button_adj_pressed = 0;
            }
        }
        else
        {
            if (0 == button_adj_pressed)
            {
                last_event_time = timer1_get();
                event_isr(EVENT_ID(EVENT_BUTTON, BUTTON_ADJ_PRESS));
                button_adj_pressed = 1;
            }
        }
        BUTTON_ADJ_INT = 0;
    }
}

// EOF //
