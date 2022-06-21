/** @file keypad.c
 * 
 * This library implements the 4x4 16-key keypad on the watch.
*/

#include <xc.h>
#include <stdio.h>

#include "drivers/ioc.h"
#include "drivers/pins.h"

#include "lib/isr.h"
#include "lib/events.h"

#include "lib/keypad.h"


// Row Pins Configuration. These are the same on both PCB revs.
//
#define KEYPAD_ROW_MASK          0x0F
#define KEYPAD_ROW_LAT           LATA            // This could also be PORTA
#define KEYPAD_ROW_0             PORTAbits.RA0   // row 0 - RA0
#define KEYPAD_ROW_1             PORTABITS.RA1   // row 1 - RA1
#define KEYPAD_ROW_2             PORTABITS.RA2   // row 2 - RA2
#define KEYPAD_ROW_3             PORTABITS.RA3   // row 3 - RA3

// Column Pins Configuration.
#if (1 == PCB_REV)
#   define KEYPAD_COLUMN_MASK      0xC3
#   define KEYPAD_COLUMN_PORT      PORTC
#   define KEYPAD_COLUMN_0         PORTCbits.RC1   // col 0 - RC1
#   define KEYPAD_COLUMN_1         PORTCbits.RC0   // col 1 - RC0
#   define KEYPAD_COLUMN_2         PORTCbits.RC6   // col 2 - RC6
#   define KEYPAD_COLUMN_3         PORTCbits.RC7   // col 3 - RC7
#   define KEYPAD_COLUMN_INTF      IOCCF
#endif

#if (2 <= PCB_REV)
#   define KEYPAD_COLUMN_MASK      0xCC
#   define KEYPAD_COLUMN_PORT      PORTC    
#   define KEYPAD_COLUMN_0         PORTCbits.RC6   // col 0 - RC6
#   define KEYPAD_COLUMN_1         PORTCbits.RC7   // col 1 - RC7
#   define KEYPAD_COLUMN_2         PORTCbits.RC2   // col 2 - RC2
#   define KEYPAD_COLUMN_3         PORTCbits.RC3   // col 3 - RC3
#   define KEYPAD_COLUMN_INTF      IOCCF
#endif


// We only support one key at a time. So these variables are used to emit a key
// release event.
static unsigned char keypad_lastkey = 0;
static unsigned char keypad_keystate = 0;

/** Currently configured keymap. */
static char keypad_keymap = 0;

/** Total num of keymaps? */
static const char keypad_keymaps_total = sizeof(keypad_keymaps);


void keypad_isr (void);


void
keypad_init (void)
{
    // Setup default state
    //
    keypad_lastkey = 0;
    keypad_keystate = 0;
    keypad_keymap = 0;

    // Disable IOC interrupts while configuring pins.
    //
    ioc_interrupt_disable();

    // Configure row pins:
    // Row pins will be used as outputs, with a default LOW state
    //
    pin_mask_output(TRISA, KEYPAD_ROW_MASK);
    pin_mask_low(LATA, KEYPAD_ROW_MASK);

    // Configure column pins:
    // Columns will be inputs with weak pull-ups enabled
    //
    pin_mask_input(TRISC, KEYPAD_COLUMN_MASK);
    pin_mask_pullup(WPUC, KEYPAD_COLUMN_MASK);

    // Register keypad isr
    //
    isr_register(0, _PIR0_IOCIF_MASK, &keypad_isr);

    // Enable IOC driver for the columns
    //
    ioc_mask_enable(IOC_PORTC, KEYPAD_COLUMN_MASK, IOC_EDGE_BOTH);

    ioc_interrupt_enable();
}

void
keypad_keymap_set (unsigned char keymap)
{
    // Set the new keymap
    //
    keypad_keymap = keymap;
}

unsigned char
keypad_keymap_get (void)
{
    // Return the keymap
    //
    return keypad_keymap;
}


/**
 * Emit a key press event based on the given keycode.
 * 
 * This also sets a flag to later determine if a key is down.
*/
void
keypad_keypress (unsigned char keycode)
{
    // Get our mapped keycode from the current keymap.
    //
    if (keypad_keymap)
    {
        keypad_lastkey = keypad_keymaps[keypad_keymap][keycode];
    }

    // A keymap of 0 returns the raw keycodes so we don't have to get one from
    // a keymap.
    //
    else
    {
        keypad_lastkey = keycode;
    }

    // Key down flag
    keypad_keystate = 1;

    // Emit keypress event
    event_isr((unsigned int)EVENT_ID(KEYPAD_EVENT_PRESS, keypad_lastkey));
}

/**
 * Emit a key release event.
*/
void
keypad_keyrelease (void)
{
    keypad_keystate = 0;
    event_isr((unsigned int)EVENT_ID(KEYPAD_EVENT_RELEASE, keypad_lastkey));
}

void
keypad_isr (void)
{
    // Determine if any of the column pins triggered the interrupt.
    //
    if (KEYPAD_COLUMN_INTF & KEYPAD_COLUMN_MASK)
    {
        // Key release
        if (keypad_keystate)
        {
            keypad_keyrelease();
        }

        // Key press
        else
        {

            signed char column = -1;
            unsigned char row = 0;

            // Loop through rows and check the columns.
            //
            for (row = 0; row < 4; row++)
            {

                // Set all rows high
                //
                pin_mask_high(KEYPAD_ROW_LAT, KEYPAD_ROW_MASK);

                // Set given row low
                //
                pin_set_low(KEYPAD_ROW_LAT, row);

                // Check columns.
                //
                if (0 < (~KEYPAD_COLUMN_PORT & KEYPAD_COLUMN_MASK))
                {
                    if (0 == KEYPAD_COLUMN_0)
                    {
                        column = 0;
                    }
                    else if (0 == KEYPAD_COLUMN_1)
                    {
                        column = 1;
                    }
                    else if (0 == KEYPAD_COLUMN_2)
                    {
                        column = 2;
                    }
                    else if (0 == KEYPAD_COLUMN_3)
                    {
                        column = 3;
                    }

                    if (-1 < column)
                    {
                        // Keycodes are 0-15 starting from the top left going right
                        //
                        keypad_keypress((unsigned char)((row * 4) + column));
                    }
                }
            }

            // Set rows to default LOW state
            //
            pin_mask_low(KEYPAD_ROW_LAT, KEYPAD_ROW_MASK);
        }
        
        // Clear column IOC flag
        //
        KEYPAD_COLUMN_INTF &= ~(KEYPAD_COLUMN_MASK);
    }
}

/* EOF */
