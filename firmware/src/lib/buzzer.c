/** @file buzzer.c
 * 
 * This library implements functionality of the piezo buzzer on the watch.
*/

#include <xc.h>

#include "drivers/pwm.h"

#include "lib/buzzer.h"


void
buzzer_init (void)
{
    // Initialize our pwm driver. This configures it for our buzzer.
    pwm_init();
}

void
buzzer_tone (unsigned int frequency, unsigned char volume, unsigned int duration)
{
    pwm_freq_set(frequency);

    pwm_duty_set(volume/2);

    pwm_enable();

    // This delay loop blocks for the given duration. It would be preferrable
    // to use an interrupt. Perhaps use the pwm timer interrupt? Would have to
    // service it at 1/4 the frequency, but it might be better than sitting
    // around twiddling our cycles.
    //
    int delay_count = 0;
    while (delay_count++ < duration)
    {
        __delay_us(990);
    }

    pwm_disable();
}

// EOF //
