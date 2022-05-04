/** @file
 * Keypad library for casio databank watches.
*/

#include <xc.h>
#include <stdio.h>

#include "drivers/ioc.h"

#include "lib/isr.h"
#include "lib/event.h"

#include "lib/keypad.h"


// Keypad Configuration
//

#define KEYPAD_USE_INTERRUPTS

#define KEYPAD_COLUMN_NUM       4
#define KEYPAD_COLUMN_MASK      0xC3
#define KEYPAD_COLUMN_0         PORTCbits.RC1
#define KEYPAD_COLUMN_1         PORTCbits.RC0
#define KEYPAD_COLUMN_2         PORTCbits.RC6
#define KEYPAD_COLUMN_3         PORTCbits.RC7

#define KEYPAD_ROW_NUM          4
#define KEYPAD_ROW_MASK         0x0F
#define KEYPAD_ROW_0            PORTAbits.RA0
#define KEYPAD_ROW_1            PORTAbits.RA1
#define KEYPAD_ROW_2            PORTAbits.RA2
#define KEYPAD_ROW_3            PORTAbits.RA3


// Some useful keymaps that can be overlayed over the default keycodes.
//

/** Default key mapping.
 * This is the keymapping that is labeled on the watch.
*/
const char keypad_keymap_default[16] = {
    '7', '8', '9', '/',
    '4', '5', '6', '*',
    '1', '2', '3', '-',
    '0', '.', '=', '+'
};

/** Directional key mapping.
 * This separates the keypad into 4 arrow keys.
*/
const char keypad_keymap_directional[16] = {
    '<', '^', '^', '>', // 0
    '<', '^', '^', '>', // 1
    '<', 'v', 'v', '>', // 2
    '<', 'v', 'v', '>'  // 3
  // 0 |  1 |  2 |  3 Col Row
};

/** Selection key mapping.
 * This separates the keypad into 4 arrow keys and a center button.
 * Currently the corner buttons are + and -.
*/
const char keypad_keymap_selection[16] = {
    '+', '^', '^', '+',
    '<', '=', '=', '>',
    '<', '=', '=', '>',
    '-', 'v', 'v', '-'
};

/** Telephone key mapping.
 * This resemebles a telephone keypad (t9), except the rows have been mirrored
 * to match the labels.
*/
const char keypad_keymap_telephone[16][4] = {                      
  {'p','q','r','s'},  {'t','u','v',0x0},  {'w','x','y','z'},  {0x0,0x0,0x0,0x0},
  {'g','h','i',0x0},  {'j','k','l',0x0},  {'m','n','o',0x0},  {0x0,0x0,0x0,0x0},
  {0x0,0x0,0x0,0x0},  {'a','b','c',0x0},  {'d','e','f',0x0},  {0x0,0x0,0x0,0x0},
  {0x0,0x0,0x0,0x0},  {0x0,0x0,0x0,0x0},  {0x0,0x0,0x0,0x0},  {0x0,0x0,0x0,0x0}
};

static int keypad_button = 0;

static int keypad_lastkey = 0;

static int trigger = 0;

static int keypad_keymap = 0;

static int keypad_events[20];

void keypad_isr (void);

/**
 * Enable the keypad.
 * This configures the appropriate drivers and enables interrupts for the
 * column pins.
*/
void
keypad_enable (void)
{
    // Setup default driver state
    //
    keypad_button = -1;
    keypad_lastkey = -1;
    keypad_keymap = 0;
    trigger = 0;

    // Configure row pins:
    // Row pins will be used as outputs,
    // with a default LOW state
    // row 0 - RA0
    // row 1 - RA1
    // row 2 - RA2
    // row 3 - RA3
    //
    TRISA &= 0b11110000;
    LATA  &= 0b11110000;

    // Configure column pins:
    // Columns will be inputs with weak pull-ups enabled
    // col 0 - RC1
    // col 1 - RC0
    // col 2 - RC6
    // col 3 - RC7
    //
    TRISC |= 0b11000011;
    WPUC  |= 0b11000011;

    // Disable IOC interrupts while configuring pins.
    //
    IOC_DISABLE;

    // Enable IOC driver for the columns
    //
    ioc_pin_enable(IOC_PORTC, 1, IOC_EDGE_FALLING);
    ioc_pin_enable(IOC_PORTC, 0, IOC_EDGE_FALLING);
    ioc_pin_enable(IOC_PORTC, 6, IOC_EDGE_FALLING);
    ioc_pin_enable(IOC_PORTC, 7, IOC_EDGE_FALLING);

    // Register keypad isr
    //
    isr_register(0, _PIR0_IOCIF_MASK, &keypad_isr);

    IOC_ENABLE;
}

void
keypad_keymap_set (int keymap)
{
    keypad_keymap = keymap;
}

/**
 * 
*/
int
keypad_pressed (void)
{
    if (keypad_button != keypad_lastkey)
    {
        keypad_button = keypad_lastkey;
        return keypad_keymap_default[keypad_button];
    }

    return -1;
}

int keypad_check (void)
{
    if (keypad_button != keypad_lastkey)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void
keypad_keypress (int keycode)
{
    unsigned char mapped_key = 0;

    switch (keypad_keymap)
    {
        case KEYPAD_KEYMAP_DEFAULT:
            mapped_key = keypad_keymap_default[keycode];
        break;

        case KEYPAD_KEYMAP_DIR:
            mapped_key = keypad_keymap_directional[keycode];
        break;

        case KEYPAD_KEYMAP_SEL:
            mapped_key = keypad_keymap_selection[keycode];
        break;

        case KEYPAD_KEYMAP_CODE:
            mapped_key = (unsigned char)keycode;
        break;

        case KEYPAD_KEYMAP_T9:
            mapped_key = keypad_keymap_telephone[keycode][0];
        break;
        
        default:
            mapped_key = (unsigned char)keycode;
        break;
    }   
    keypad_lastkey = mapped_key;
    event_add(0xCE, mapped_key);
}

/**
 * Check the keypad columns.
 * This loops through the column pins and checks for a low.
 * 
 * @returns The column that is low. -1 if none are low.
*/
inline int keypad_column_check (void)
{
    int column = -1;

    // Return quickly if no columns are low
    //
    if (0 < (~PORTC & KEYPAD_COLUMN_MASK))
    {
        if (0 == PORTCbits.RC1)
        {
            column = 0;
        }
        else if (0 == PORTCbits.RC0)
        {
            column = 1;
        }
        else if (0 == PORTCbits.RC6)
        {
            column = 2;
        }
        else if (0 == PORTCbits.RC7)
        {
            column = 3;
        }
    }

    return column;
}


/**
 * Select a keypad row.
 * Selecting a row consists of setting all rows high except the given row. The
 * columns can then be scanned to determine the pressed key. Deselecting all
 * rows will set them low.
 * 
 * @param[in] row The row to select. -1 to deselect all rows.
*/
void
keypad_row_select (int row)
{
    // Clear all rows if given row is -1
    //
    if (-1 == row)
    {
        LATA &= ~KEYPAD_ROW_MASK;
    }

    // Select row
    //
    else if (-1 < row)
    {
        // Set all rows high
        //
        LATA |= KEYPAD_ROW_MASK;

        // Set given row low
        //
        LATA &= ~(1U << row);
    }

    return;
}   /* keypad_row_select() */


/**
 * Scan keypad for any pressed keys.
 * This function adds any detected keys into the key buffer.
*/
inline void
keypad_scan (void)
{
    int column = 0;
    int row = 0;
    
    if (trigger)
    {
        TXREG1 = 'k';
        // Loop through rows and check the columns.
        //
        for (row = 0; row < KEYPAD_ROW_NUM; row++)
        {

            // Set all rows high
            //
            LATA |= KEYPAD_ROW_MASK;

            // Set given row low
            //
            LATA &= ~(1 << row);

            column = keypad_column_check();
            if (-1 < column)
            {
                // Keycodes are 0-15 starting from the top left going right
                //
                keypad_keypress((row * KEYPAD_COLUMN_NUM) + column);
            }
        }

        // Deselect all rows
        //
        keypad_row_select(-1);
        trigger = 0;
    }
 
    return;
}   /* keypad_scan() */


void
keypad_isr (void)
{
    // if (IOCIE == 1 && IOCIF == 1)
    // {
        // TXREG1 = 'k';
        // trigger = 1;
        // keypad_scan();
        // if (IOCCFbits.IOCCF1 == 1)
        // {
        //     // col 0
        //     IOCCFbits.IOCCF1 = 0;
        //     trigger = 1;
        //     // keypad_button = 0;
        // }

        // if (IOCCFbits.IOCCF0 == 1)
        // {
        //     // col 1
        //     IOCCFbits.IOCCF0 = 0;
        //     trigger = 1;
        //     // keypad_button = 1;
        // }

        // if (IOCCFbits.IOCCF6 == 1)
        // {
        //     // col 2
        //     IOCCFbits.IOCCF6 = 0;
        //     trigger = 1;
        //     // keypad_button = 2;
        // }

        // if (IOCCFbits.IOCCF7 == 1)
        // {
        //     // col 3
        //     IOCCFbits.IOCCF7 = 0;
        //     trigger = 1;
        //     // keypad_button = 3;
        // }

        if (IOCCF & KEYPAD_COLUMN_MASK) {
            int column = 0;
            int row = 0;

            TXREG1 = 'k';
            // Loop through rows and check the columns.
            //
            for (row = 0; row < KEYPAD_ROW_NUM; row++)
            {

                // Set all rows high
                //
                LATA |= KEYPAD_ROW_MASK;

                // Set given row low
                //
                LATA &= ~(1 << row);

                column = keypad_column_check();
                if (-1 < column)
                {
                    // Keycodes are 0-15 starting from the top left going right
                    //
                    keypad_keypress((row * KEYPAD_COLUMN_NUM) + column);
                }
            }

            // Deselect all rows
            //
            keypad_row_select(-1);
            
            // Clear column IOC flag
            //
            IOCCF &= ~(IOCCF & KEYPAD_COLUMN_MASK);
        }
    // }
}