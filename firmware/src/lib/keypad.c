/** @file
 * Keypad library for casio databank watches.
*/

#include <xc.h>
#include <stdio.h>

#include "drivers/ioc.h"

#include "lib/isr.h"
#include "lib/event.h"

#include "lib/keypad_keymaps.h"
#include "lib/keypad.h"


#define KEYPAD_ROW_MASK          0x0F
#define KEYPAD_ROW_0             PORTAbits.RA0   // row 0 - RA0
#define KEYPAD_ROW_1             PORTABITS.RA1   // row 1 - RA1
#define KEYPAD_ROW_2             PORTABITS.RA2   // row 2 - RA2
#define KEYPAD_ROW_3             PORTABITS.RA3   // row 3 - RA3

#if (1 == PCB_REV)
#   define KEYPAD_COLUMN_MASK      0xC3
#   define KEYPAD_COLUMN_PORT      PORTC
#   define KEYPAD_COLUMN_0         PORTCbits.RC1   // col 0 - RC1
#   define KEYPAD_COLUMN_1         PORTCbits.RC0   // col 1 - RC0
#   define KEYPAD_COLUMN_2         PORTCbits.RC6   // col 2 - RC6
#   define KEYPAD_COLUMN_3         PORTCbits.RC7   // col 3 - RC7
#endif

#if (2 <= PCB_REV)
#   define KEYPAD_COLUMN_MASK      0xCC
#   define KEYPAD_COLUMN_PORT      PORTC    
#   define KEYPAD_COLUMN_0         PORTCbits.RC6   // col 0 - RC6
#   define KEYPAD_COLUMN_1         PORTCbits.RC7   // col 1 - RC7
#   define KEYPAD_COLUMN_2         PORTCbits.RC2   // col 2 - RC2
#   define KEYPAD_COLUMN_3         PORTCbits.RC3   // col 3 - RC3
#endif


static unsigned char keypad_lastkey = 0;

static unsigned char keypad_keystate = 0;

static char keypad_keymap = 0;
static const char keypad_keymaps_total = sizeof(keypad_keymaps);


void keypad_isr (void);


/**
 * Enable the keypad.
 * This configures the appropriate drivers and enables interrupts for the
 * column pins.
*/
void
keypad_enable (void)
{
    // Setup default state
    //
    keypad_lastkey = 0;
    keypad_keystate = 0;
    keypad_keymap = 1;

    // Disable IOC interrupts while configuring pins.
    //
    IOC_DISABLE;

    // Configure row pins:
    // Row pins will be used as outputs,
    // with a default LOW state
    //
    TRISA &= ~KEYPAD_ROW_MASK;
    LATA  &= ~KEYPAD_ROW_MASK;

    // Enable IOC driver for the columns
    //
    ioc_mask_enable(IOC_PORTC, KEYPAD_COLUMN_MASK, IOC_EDGE_BOTH);

    // Configure column pins:
    // Columns will be inputs with weak pull-ups enabled
    //
    TRISC |= KEYPAD_COLUMN_MASK;
    WPUC  |= KEYPAD_COLUMN_MASK;

    // Register keypad isr
    //
    isr_register(0, _PIR0_IOCIF_MASK, &keypad_isr);

    IOC_ENABLE;
}

void
keypad_keymap_set (unsigned char keymap)
{
    keypad_keymap = keymap;
}


void
keypad_keypress (unsigned char keycode)
{
    if (keypad_keymap)
    {
        keypad_lastkey = keypad_keymaps[keypad_keymap][keycode];
    }
    else
    {
        keypad_lastkey = keycode;
    }

    keypad_keystate = 1;
    event_add(KEYPAD_KEY_PRESS_EVENT, keypad_lastkey);
}

void
keypad_keyrelease (void)
{
    keypad_keystate = 0;
    event_add(KEYPAD_KEY_RELEASE_EVENT, keypad_lastkey);
}

void
keypad_isr (void)
{
    if (IOCCF & KEYPAD_COLUMN_MASK) {

// #       if DEV_BUILD
//         TXREG1 = 'k';
// #       endif

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
                LATA |= KEYPAD_ROW_MASK;

                // Set given row low
                //
                LATA &= ~(1 << row);


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
                        keypad_keypress((unsigned char) ((row * 4) + column));
                    }
                }
            }

            // Set rows to default LOW state
            //
            LATA &= ~KEYPAD_ROW_MASK;
        }
        
        // Clear column IOC flag
        //
        IOCCF &= ~(IOCCF & KEYPAD_COLUMN_MASK);
    }
}

/* EOF */
