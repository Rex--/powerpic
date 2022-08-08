/** @file post.c
 * 
 * Power on self tests after a reset event.
*/

#include <xc.h>

#include "lib/datetime.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buttons.h"
#include "lib/buzzer.h"
#include "lib/backlight.h"

#include "post.h"

#define LOG_TAG "POST"
#include "lib/logging.h"


#define POST_SPLASH "PowerPIC"

static time_t post_time = {0};

void
post_bios (void)
{
    // POST Code 0
    display_secondary_character(2, '0');
    display_update();

    // Set rtcc time to 00:00:00. We will check after other tests and verify
    // the timer is ticking.
    datetime_time_set(&post_time);


    // POST Code 1
    display_secondary_character(2, 1);
    display_update();

    // The start of a our post is a splash screen of sorts.
    // 'PowerPIC' sliding in from the right
    for (signed char offset=8; offset != 0; offset--)
    {
        display_primary_string(offset, POST_SPLASH);
        display_update();
        __delay_ms(150);
    }

    // POST Code 2
    display_secondary_character(2, 2);
    display_update();

    // Turn backlight on
    backlight_set(BACKLIGHT_ON);
    __delay_ms(500);

    // 'PowerPIC' blinks twice and beeps
    display_primary_clear(0);   display_update();
    __delay_ms(250);

    // Buzzer gives two quick beeps
    buzzer_tone(2500, 50, 25);
    __delay_ms(75);
    buzzer_tone(2500, 100, 25);
    display_primary_string(1, POST_SPLASH); display_update();
    __delay_ms(500);

    display_primary_clear(0);   display_update();
    __delay_ms(250);

    // Buzzer gives two quick beeps
    buzzer_tone(2500, 50, 25);
    __delay_ms(75);
    buzzer_tone(2500, 100, 25);
    display_primary_string(1, POST_SPLASH); display_update();
    __delay_ms(500);

    // POST Code 3
    display_secondary_character(2, 3);
    display_update();

    // Turn backlight off
    backlight_set(BACKLIGHT_OFF);

    // POST Code 4
    display_secondary_character(2, 4);
    display_update();

    // Finish countdown to 10 seconds and procede
    while (10 > post_time.second)
    {
        datetime_time_now(&post_time);
        display_secondary_character(2, post_time.second);
        display_update();
        __delay_ms(100);
    }

    // Clear displays when done
    display_primary_clear(0);
    display_secondary_clear(0);
    display_update();
}


// EOF //
