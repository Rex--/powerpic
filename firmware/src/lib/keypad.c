/** @file
 * Keypad library for casio databank watches.
*/

#include <xc.h>
#include <stdio.h>

#include "dev_config.h"

#include "drivers/ioc.h"

#include "lib/isr.h"
#include "lib/event.h"

#include "lib/keypad.h"

#define KEYPAD_ROW_MASK          0x0F
#define KEYPAD_ROW_0             PORTAbits.RA0   // row 0 - RA0
#define KEYPAD_ROW_1             PORTABITS.RA1   // row 1 - RA1
#define KEYPAD_ROW_2             PORTABITS.RA2   // row 2 - RA2
#define KEYPAD_ROW_3             PORTABITS.RA3   // row 3 - RA3

#if (1 == PCB_REV)
#   define KEYPAD_COLUMN_MASK      0xC3
#   define KEYPAD_COLUMN_0         PORTCbits.RC1   // col 0 - RC1
#   define KEYPAD_COLUMN_1         PORTCbits.RC0   // col 1 - RC0
#   define KEYPAD_COLUMN_2         PORTCbits.RC6   // col 2 - RC6
#   define KEYPAD_COLUMN_3         PORTCbits.RC7   // col 3 - RC7
#endif

#if (2 <= PCB_REV)
    #define KEYPAD_COLUMN_MASK      0xCC
    #define KEYPAD_COLUMN_0         PORTCbits.RC6   // col 0 - RC6
    #define KEYPAD_COLUMN_1         PORTCbits.RC7   // col 1 - RC7
    #define KEYPAD_COLUMN_2         PORTCbits.RC2   // col 2 - RC2
    #define KEYPAD_COLUMN_3         PORTCbits.RC3   // col 3 - RC3
#endif

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

static int keypad_lastkey = 0;

static int trigger = 0;

static const char * keypad_keymap = NULL;


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
    keypad_lastkey = -1;
    trigger = 0;
    keypad_keymap = &keypad_keymap_default[0];

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
keypad_keymap_set (int keymap)
{
    switch (keymap)
    {
        case KEYPAD_KEYMAP_DEFAULT:
            keypad_keymap = &keypad_keymap_default[0];
        break;

        case KEYPAD_KEYMAP_DIR:
            keypad_keymap = &keypad_keymap_directional[0];
        break;

        case KEYPAD_KEYMAP_SEL:
            keypad_keymap = &keypad_keymap_selection[0];
        break;

        case KEYPAD_KEYMAP_T9:
            keypad_keymap = &keypad_keymap_telephone[0][0];
        break;
        
        default:
        case KEYPAD_KEYMAP_CODE:
            keypad_keymap = NULL;
        break;
    }
}


void
keypad_keypress (unsigned char keycode)
{
    unsigned char mapped_key = 0;

    if (NULL == keypad_keymap)
    {
        mapped_key = keycode;
    }
    else
    {
        mapped_key = keypad_keymap[keycode];
    }

    trigger = 1;
    keypad_lastkey = mapped_key;
    event_add(KEYPAD_KEY_PRESS_EVENT, mapped_key);
}

void
keypad_keyrelease (void)
{
    trigger = 0;
    event_add(KEYPAD_KEY_RELEASE_EVENT, keypad_lastkey);
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
    }

    return column;
}

#if 0
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
#endif


#if 0
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
#endif


void
keypad_isr (void)
{
    if (IOCCF & KEYPAD_COLUMN_MASK) {
        TXREG1 = 'k';
        if (trigger)
        {
            // Key release
            keypad_keyrelease();
        }
        else
        {
            int column = 0;
            int row = 0;

            // Loop through rows and check the columns.
            //
            TRISA &= ~KEYPAD_ROW_MASK;
            for (row = 0; row < 4; row++)
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
                    keypad_keypress((row * 4) + column);
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