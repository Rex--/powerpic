/** @file buzzer.c
 * 
 * This library implements functionality of the piezo buzzer on the watch.
*/

#include <xc.h>
#include <math.h>

#include "drivers/pwm.h"
#include "lib/isr.h"
#include "lib/logging.h"

#include "lib/buzzer.h"


#undef  LOG_TAG
#define LOG_TAG "lib.pwm"


static volatile unsigned int tone_duration = 0;
static volatile unsigned int played_duration = 0;

static void buzzer_isr (void);

void
buzzer_init (void)
{
    // Initialize our pwm driver. This configures it for our buzzer.
    pwm_init();

    // Register ISR
    // Should this be done only when we are playing a tone?
    isr_register(4, _PIR4_TMR2IF_MASK, &buzzer_isr);
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


void
buzzer_tone2 (unsigned int frequency, unsigned char volume, unsigned int duration)
{
    pwm_freq_set(frequency);

    pwm_duty_set(volume/2);

    // Duration of 1 interrupt in ms
    float timer_duration = 1.0F/((frequency / 3.0F) / 1000);
    
    // Figure out how many interrupt we need to delay
    tone_duration = (unsigned int)lroundf(duration / timer_duration);

    played_duration = 0;

    LOG_DEBUG("Timer Duration: %f", timer_duration);
    LOG_DEBUG("Tone Duration: %i", tone_duration);
    
    // enable interrupt
    PIE4bits.TMR2IE = 1;

    // start tone
    pwm_enable();
}

static void
buzzer_isr (void)
{
    // This gets called at ~1/3 the PWM frequency

    if (tone_duration <= played_duration)
    {
        // stop tone
        // pwm_disable();
        PWM4CONbits.PWM4EN = 0;

        // Disable interrupt
        PIE4bits.TMR2IE = 0;
    }
    else
    {
        played_duration++;
    }

    // Clear interrupt flag
    PIR4bits.TMR2IF = 0;
}


// EOF //
