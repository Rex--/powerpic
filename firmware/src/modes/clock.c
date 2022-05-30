/** @file @clock.c
 * Simple clock mode for development.
 * Currently implements:
 * - simple time set functionality
 * - 24-hour format
 * - seconds tick(that are updated no more than 1000 ms late :)
*/

#include <xc.h>
#include <stdio.h>

// #include "dev_config.h"

#include "drivers/rtcc.h" // TODO: datatime library

#include "lib/isr.h"
#include "lib/mode.h"
#include "lib/event.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buzzer.h"

#include "modes/clock.h"


static int clock_set = 0;

static unsigned char clock_fmt = 0;

static void time_draw (void);
static void set_time_start (void);
static void set_time_thread (mode_config_t *cfg, unsigned int event);

void clock_second_update_isr (void);
void clock_hour_chime_isr (void);

int update_isr = 0;
int chime_isr = 0;

void
clock_init (mode_config_t *cfg)
{
    cfg->tickrate = 0;
    cfg->keymap = 1;

    // Register isr for hour chime
    //
    chime_isr = isr_register(8, _PIR8_RTCCIF_MASK, &clock_hour_chime_isr);


    // Configure RTC alarm to interrupt every hour forever
    //
    rtcc_set_alarm(0, 1, RTCC_ALARM_HOUR);
    ALRMCONbits.CHIME = 1;
    ALRMRPT = 0xFF;

    // Enable RTCC Alarm and its interrupt
    //
    ALRMCONbits.ALRMEN = 1;
    PIE8bits.RTCCIE = 1;
}


void
clock_start (mode_config_t *cfg)
{
    // Configure alarm interrupt to occur evey second
    //
    ALRMCONbits.ALRMEN = 0;
    rtcc_set_alarm(0, 0, RTCC_ALARM_SECOND);
    printf("Unregistering %i isr\n\r", chime_isr);
    isr_unregister(chime_isr);
    update_isr = isr_register(8, _PIR8_RTCCIF_MASK, &clock_second_update_isr);
    ALRMCONbits.ALRMEN = 1;
    
    clock_set = 0;
    clock_fmt = 1;
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
#       if DEV_BUILD
        unsigned char hour =    (unsigned char)(2 << 4) | 3;
        unsigned char minute =  (unsigned char)(5 << 4) | 9;
        unsigned char second =  (unsigned char)(4 << 4) | 5;
        rtcc_set_time(hour, minute, second);
#       else
        set_time_start();
        clock_set = 1;
#       endif
    }
    else if ((event & 0xFF) == KEYPAD_KEY_PRESS_EVENT)
    {
        unsigned char key = event >> 8;

        if (key == '.')
        {
            // period button on keypad changes 24/48 fmt
            //
            if (clock_fmt)
            {
                clock_fmt = 0;
            }
            else
            {
                clock_fmt = 1;
            }
            time_draw();
        }

        // Right column controls backlight
        //
        else if (key == '/' || key == '*' || key == '-' || key == '+')
        {
            LATGbits.LATG7 = 1;
        }
    }
    else if ((event & 0xFF) == KEYPAD_KEY_RELEASE_EVENT)
    {
        unsigned char key = event >> 8;

        if (key == '/' || key == '*' || key == '-' || key == '+')
        {
            LATGbits.LATG7 = 0;
        }
    }
    else
    {
        time_draw();
    }
}

void
clock_stop (mode_config_t *cfg)
{
    // Configure alarm interrupt to occur evey hour for hourly chime
    //
    ALRMCONbits.ALRMEN = 0;
    rtcc_set_alarm(0, 1, RTCC_ALARM_HOUR);
    printf("Unregistering %i isr\n\r", chime_isr);
    isr_unregister(update_isr);
    chime_isr = isr_register(8, _PIR8_RTCCIF_MASK, &clock_hour_chime_isr);
    ALRMCONbits.ALRMEN = 1;
}


static void
time_draw (void)
{
    unsigned char hour_chime = MINUTES + SECONDS;

    if (0 == hour_chime)
    {
        buzzer_beep();
    }

    // Display 24 hours time
    display_misc_clear(DISPLAY_MISC_PM);
    display_misc_clear(DISPLAY_MISC_AM);
    display_primary_string(1, rtcc_get_time());

    // Format 12-hour if needed
    if (clock_fmt)
    {
        unsigned char hour_dec = ((HOURS >> 4) * 10) + (HOURS & 0x0F);

        if (hour_dec > 12)
        {
            // PM
            hour_dec -= 12;
            if (hour_dec < 12)
            {
                display_misc(DISPLAY_MISC_PM);
                display_primary_character(2, (hour_dec%10));
                hour_dec /= 10;
                if (hour_dec)
                {
                    display_primary_character(1, 1);
                }
                else
                {
                    display_primary_clear(1);
                }
            }
        }

        else if (hour_dec == 12)
        {
            // 12 PM - Noon
            display_misc(DISPLAY_MISC_PM);
        }

        else if (hour_dec == 0)
        {
            // 12 AM - Midnight
            display_misc(DISPLAY_MISC_AM);
            display_primary_character(1, 1);
            display_primary_character(2, 2);
        }

        else
        {
            // AM
            display_misc(DISPLAY_MISC_AM);

            if ((HOURS >> 4) == 0)
            {
                display_primary_clear(1);
            }
        }

    }
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
    if ((event & 0xFF) == KEYPAD_KEY_PRESS_EVENT)
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

void
clock_second_update_isr (void)
{
    event_add(0x01, 0x00);
    PIR8bits.RTCCIF = 0;
}

void
clock_hour_chime_isr (void)
{
    TX1REG = 'z';
    buzzer_beep();
    PIR8bits.RTCCIF = 0;
}

