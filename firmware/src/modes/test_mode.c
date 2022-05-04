/** @file test_mode.c
 * This Mode is for testing and development.
*/

#include <xc.h>
#include <stdio.h>

#include "lib/mode.h"
#include "lib/display.h"
#include "lib/keypad.h"

#include "modes/test_mode.h"


static int keypress;

static int test_mode_adj = 0;


/**
 * Initialize test.
 * 
 * for tseting mode app
*/
void
test_mode_init (mode_config_t *cfg)
{
    // Default configuration
    //
    cfg->tickrate = 250;
    cfg->keymap = KEYPAD_KEYMAP_DEFAULT;
    printf("Hello\n\r");
}

/**
 * Start test.
 * 
 * for testing mode applications.
*/
void
test_mode_start (mode_config_t *cfg)
{
    // Starting state
    test_mode_adj = 0;
    keypress = 0;
    display_primary_clear(0);
    display_primary_string(0, "hello");
}


static int keymap_edit = 0;

void
test_mode_edit_start (mode_config_t *cfg)
{
    display_primary_clear(0);
    display_primary_string(1, "LAYOUT");
    display_primary_character(8, cfg->keymap);
    if (cfg->keymap != KEYPAD_KEYMAP_DEFAULT)
    {
        cfg->keymap = KEYPAD_KEYMAP_DEFAULT;
    }
    keymap_edit = -1;
}

void
test_mode_edit (mode_config_t *cfg, unsigned int event)
{

    // Keypress event
    if ((event & 0xFF) == 0xCE)
    {
        // Keypresses 0-9
        if ( ((event >> 8) - 48) < 10 )
        {
            // Press button twice to select keymap
            if ((event >> 8) - 48 == keymap_edit)
            {
                cfg->keymap = keymap_edit;
                // cfg->tickrate = 1; // 1 tick timeout
                test_mode_adj = 0;
                display_primary_string(1, "SAVED ");
            }
            else
            {
                keymap_edit = (int)(event >> 8) - 48;
                display_primary_character(8, (unsigned char)keymap_edit);
            }
        }
    }
}


static signed char test_justify = 0;
static const char *test_justify_string = "test";

void test_mode_justify_start (mode_config_t *cfg)
{
    test_justify = 0;
    display_primary_clear(0);
    display_primary_string(test_justify, test_justify_string);
    cfg->keymap = KEYPAD_KEYMAP_DIR;
}

void test_mode_justify (mode_config_t *cfg, unsigned int event)
{
    if ((event & 0xFF) == 0xCE)
    {
        unsigned char key = event >> 8;
        if (key == '>' || key == '^')
        {
            test_justify++;
        }
        else if (key == '<' || key == 'v')
        {
            test_justify--;
        }
        display_primary_clear(0);
        display_secondary_clear(0);
        display_primary_string(test_justify, test_justify_string);
        display_secondary_number(0, test_justify);
    }
    else if (event == 0x020b)
    {
        test_mode_adj = 0;
    }
}

const char *test_str = "Press button or change Views with ADJ 8]}";
char *test_scroll_marker = NULL;
unsigned char test_str_len = 8;
signed char scrolla = 8;
signed char scrollb = 0;

void
test_mode_scroll (void)
{
    if (*test_scroll_marker)
    {
        display_primary_clear(0);
        display_primary_string(1, test_scroll_marker++);

    }
    else
    {
        test_scroll_marker = test_str;
    }





    // if (scrolla < 0)
    // {
    //     display_secondary_character(1, '-');
    //     display_secondary_character(2, -scrolla);
    // }
    // else
    // {
    //     display_secondary_clear(1);
    //     display_secondary_character(2, scrolla);
    // }
    // display_primary_clear(0);
    // display_primary_string(scrolla, test_str);
    // scrolla--;

    // if (scrolla == 0)
    // {
    //     scrolla = -2;
    // }
    // else if(scrolla < -8)
    // {
    //     scrolla = 8;
    // }



    // else if (scrolla == (LCD_PRIMARY_CHARACTERS - test_str_len))
    // {
    //     scrolla = -2;
    // }

}

/**
 * Initialize test
 * 
 * for tseting mode app
*/
void
test_mode_thread (mode_config_t *cfg, unsigned int event)
{
    if (test_mode_adj)
    {
        if (test_mode_adj == 1)
            test_mode_edit(cfg, event);
        else if (test_mode_adj == 2)
            test_mode_justify(cfg, event);
        return;
    }

    if ((event & 0xFF) == 0xCE)
    {
        if (event == 0x00ce)
        {
            test_mode_justify_start(cfg);
            test_mode_adj = 2;
            return;
        }

        display_primary_string(1, "button  ");

        unsigned char key = (event >> 8);

        printf("Key: %x\n\r", key);

        if ((9 < key) && (key < 16))
        {
            display_primary_character(7, 1);
            display_primary_character(8, key - 10);
        }
        else
        {
            display_primary_character(8, key);
        }
    }
    else if (event == 0x020b)
    {
        test_mode_adj = 1;
        test_mode_edit_start(cfg);
    }
    else if ((event & 0xFF) == 0x01)
    {
        // if (cfg->tickrate != -1)
        // {
        //     cfg->tickrate = -1;
        // }
        test_mode_scroll();
    }
    else
    {
        display_primary_clear(0);
        display_primary_string(0, "Error");
    }
}

/**
 * Initialize test
 * 
 * for tseting mode app
*/
void
test_mode_stop (mode_config_t *cfg)
{
    // display_primary_string_draw("G00DBYE1");
    // display_period_draw(0x07);
}


/*** EOF ***/