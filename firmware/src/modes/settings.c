#include <xc.h>
#include <stdio.h>

#include "lib/mode.h"
#include "lib/keypad.h"
#include "lib/display.h"

#include "modes/settings.h"

#define SETTINGS_MENU_COUNT         3
#define SETTINGS_MENU_DISPLAY_LEN   6

#define SETTINGS_MENU_LED           0
#define SETTINGS_MENU_BEEP          1
#define SETTINGS_MENU_RTCC          2


static const char *settings_menu_str[SETTINGS_MENU_COUNT] = {
    "hello",
    "bEEP",
    "RTCC",
};

typedef void (*submenu_thread_t) (unsigned char key);
void    submenu_led (unsigned char key);

static const submenu_thread_t settings_submenu[SETTINGS_MENU_COUNT] = {
    &submenu_led,
};

static unsigned char settings_menu_sel = 0;
static unsigned char settings_menu_sub = 0;
static unsigned char settings_menu_sub_sel = 0;

void    settings_menu_draw (void);
void    submenu_thread (mode_config_t *cfg, unsigned int event);


void
settings_init (mode_config_t *cfg)
{
    cfg->tickrate = 0;
    cfg->keymap = KEYPAD_KEYMAP_DIR;
}


void
settings_start (mode_config_t *cfg)
{
    settings_menu_sel = 0;
    settings_menu_sub = 0;
    settings_menu_draw();
}


void
settings_thread (mode_config_t *cfg, unsigned int event)
{
    if (settings_menu_sub)
    {
        settings_submenu[settings_menu_sel](event >> 8);
        // submenu_thread(cfg, event);
        return;
    }

    // Tick events
    if ((event & 0xFF) == 0x01)
    {

    }
    // Keypad events
    else if ((event & 0xFF) == KEYPAD_KEY_PRESS_EVENT)
    {
        unsigned char key = event >> 8;

        printf("keypress: %x\n\r", key);

        if ('>' == key)
        {
            if (settings_menu_sel < SETTINGS_MENU_COUNT-1)
            {
                settings_menu_sel++;
            }
        }
        else if ('<' == key)
        {
            if (settings_menu_sel > 0)
            {
                settings_menu_sel--;
            }
        }
    }
    // Adjust button
    else if (event == 0x020b)
    {
        // if (++settings_menu_sel >= SETTINGS_MENU_COUNT)
        // {
        //     settings_menu_sel = 0;
        // }
        settings_menu_sub = 1;
        settings_menu_sub_sel = 0;
        printf("starting submenu %i\n\r", settings_menu_sel);

        if (settings_submenu[settings_menu_sel] != NULL)
        {
            settings_submenu[settings_menu_sel](0);
        }
        else
        {
            display_primary_string(0, "ERROR?");
        }
        // submenu_thread(cfg, event);
        return;
    }
    else
    {

    }
    settings_menu_draw();
}

void
settings_stop (mode_config_t *cfg)
{

}

void
settings_menu_draw (void)
{
    // for (char lcd_pos = 2; lcd_pos < 8; lcd_pos++)
    // {
    //     display_primary_character(lcd_pos, settings_menu[settings_menu_sel][lcd_pos-2]);
    // }

    display_primary_clear(0);
    display_period_clear(0);

    display_primary_string(0, settings_menu_str[settings_menu_sel]);

    display_primary_character(1, '<');
    display_primary_character(8, '>');
    
    display_period(settings_menu_sel+1);
}

// Sub Menu functions

void
submenu_thread (mode_config_t *cfg, unsigned int event)
{
    unsigned char key = event >> 8;
    if ((event & 0xFF) == KEYPAD_KEY_PRESS_EVENT)
    {

    }

    switch (settings_menu_sel)
    {
        case 0:
            submenu_led(key);
        break;
        
        default:
            display_primary_string(2, "Error?");
        break;
    }
}


void
submenu_led (unsigned char key)
{
    switch (settings_menu_sub_sel)
    {
        case 0:
            display_primary_string(1, "duty");
            display_primary_number(7, 10000);
            display_primary_character(7, '*');
            display_primary_character(8, 'o');
        break;

        default:
            display_primary_string(1, "Error1");
    }
}
