/** @file @clock.c
 * Simple clock mode for development.
 * Currently implements:
 * - simple time set functionality
 * - 24-hour format
 * - seconds tick(that are updated no more than 1000 ms late :)
*/

#include <xc.h>
#include <stdio.h>

#include "drivers/rtcc.h" // TODO: datatime library

#include "lib/mode.h"
#include "lib/display.h"

#include "modes/clock.h"


static int clock_set = 0;

static void time_draw (void);
static void set_time_start (void);
static void set_time_thread (mode_config_t *cfg, unsigned int event);


void
clock_init (mode_config_t *cfg)
{
    cfg->tickrate = 1000;
    cfg->keymap = 0;
}


void
clock_start (mode_config_t *cfg)
{
    clock_set = 0;
    display_primary_clear(0);
    display_period(DISPLAY_PERIOD_COLON);
    time_draw();
}

void
clock_thread (mode_config_t *cfg, unsigned int event)
{
    if (clock_set)
    {
        set_time_thread(cfg, event);
    }
    else if (event == 0x020b)
    {
        set_time_start();
        clock_set = 1;
    }
    else
    {
        time_draw();
    }
}

void
clock_stop (mode_config_t *cfg)
{
    display_period_clear(DISPLAY_PERIOD_COLON);
}


static void
time_draw (void)
{
    display_primary_string(1, rtcc_get_time());
}


// Set Time functions

static void set_time_draw (void);

static unsigned char set_time[6];
static unsigned char set_digit = 0;

static void
set_time_draw (void)
{
    display_primary_character(1, set_time[0]);    //Hours
    display_primary_character(2, set_time[1]);
    display_primary_clear(3);                     // Colon
    display_primary_character(4, set_time[2]);    //Min
    display_primary_character(5, set_time[3]);
    display_primary_clear(6);                     // Space
    display_primary_character(7, set_time[4]);    //Sec
    display_primary_character(8, set_time[5]);

    if (3 < set_digit)
    {
        display_primary_character((signed char)(set_digit + 3), '_');
    }
    else if (1 < set_digit)
    {
        display_primary_character((signed char)(set_digit + 2), '_');
    }
    else
    {
        display_primary_character((signed char)(set_digit + 1), '_');
    }
}

static void
set_time_start (void)
{
    set_digit = 0;
    
    for (int digit = 0; digit < 6; digit++)
    {
        set_time[digit] = '0';
    }

    set_time_draw();
}


static void
set_time_thread (mode_config_t *cfg, unsigned int event)
{
    // Keypress
    if ((event & 0xFF) == 0xCE)
    {
        printf("Keypress\n\r");
        unsigned char keypress = (unsigned char)(event >> 8);
        if (keypress >= '0' && keypress <= '9')
        {
            set_time[set_digit++] = keypress - 48;
            if (set_digit > 5)
            {
                set_digit = 0;
            }
            printf("Time set: %c\n\r", keypress);
            set_time_draw();
        }
        else if (keypress == '=')
        {
            //set rtc
            unsigned char hour = (unsigned char)(set_time[0] << 4) | set_time[1];
            unsigned char minute = (unsigned char)(set_time[2] << 4) | set_time[3];
            unsigned char second = (unsigned char)(set_time[4] << 4) | set_time[5];
            rtcc_set_time(hour, minute, second);

            display_primary_string(0, "time set");
            clock_set = 0;
        }
    }

    else if (event == 0x020b)
    {
        clock_set = 0;
    }
}