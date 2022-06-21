/** @file pwm.h
 * 
 * PWM driver for PIC16LF1919x.
*/

#include <xc.h>
#include <math.h>   // for llroundf()

#include "lib/logging.h"

#include "drivers/pwm.h"


/** Currently cofigured duty cycle. */
unsigned char pwm_duty_ratio = 0;

/** Currently configured frequency. */
unsigned int pwm_frequency = 0;

/** Calculated timer period for the given frequency. */
volatile unsigned char pwm_timer_period = 0;

/** Calculated timer prescale value for the given frequency. */
volatile unsigned char pwm_timer_prescaler = 0;

/** Calculated duty-cycle value. */
volatile unsigned int pwm_duty_cycle = 0;

void
pwm_init (void)
{
#   if (1 == PCB_REV)
    // Set pwm pin to output and configure pps
    TRISGbits.TRISG7 = 0;
    RG7PPS = 0x0c;
#   endif

#   if (2 == PCB_REV)
    // Set PWM pin as output
    // We use RG6 for the buzzer
    //
    TRISGbits.TRISG6 = 0;
    RG6PPS = 0x0C;
#   endif

    // set up timer2
    //
    T2CLKCONbits.CS = 0x01;        // clock - Fosc/4
    T2HLTbits.PSYNC = 1;            // Synced with Fosc
    // T2CONbits.CKPS = 0b011; // 1:8
    // T2PR = 64;
    T2CONbits.ON = 1;
}

void
pwm_enable (void)
{
    // Enable timer
    // T2CONbits.ON = 1;

    // Enable PWM
    PWM4CONbits.PWM4EN = 1;
}

void
pwm_disable (void)
{
    // Disable pwm
    PWM4CONbits.PWM4EN = 0;

    // Disable timer2
    // T2CONbits.ON = 0;
}

void
pwm_freq_set (unsigned int freq)
{
    // To set the PWM to a given frequency we need to figure out two things:
    // 1. Timer period
    // 2. Timer prescaler
    //
    // To calculate the correct values, we start with the lowest available
    // prescaler (1:1). We then calculate the timer period value for each value
    // and stop at the first that is within the timer limits. This gives us the
    // prescale option with the highest resolution available for the frequency.

    int timer_period = 0;
    // As prescaler values get lower, it allows higer frequencies.
    //
    for (unsigned char timer_prescale = 0; timer_prescale < 8; timer_prescale++)
    {
        // Magic formula from the datasheet gives us our timer period.
        // This takes up about 7% of our flash because of the float arithmetic.
        //
        timer_period = (int)lroundf(
            (1 / ((float)freq) /
            (4 * (1 / (float)_XTAL_FREQ) * (1 << timer_prescale))) - 1
        );

        if ((0 < timer_period) && (timer_period < 255))
        {
            LOG_DEBUG("Timer2 period: %i", timer_period);
            LOG_DEBUG("Timer2 prescaler: %i", timer_prescale);

            pwm_timer_period = (unsigned char)timer_period;
            T2PR = pwm_timer_period; // TODO: Don't actually set the period registers until we enable.
            pwm_timer_prescaler = timer_prescale;
            T2CONbits.CKPS = pwm_timer_prescaler; // TODO: Same with prescaler
            return;
        }
    }

    LOG_ERROR("BAD PWM FREQ: %i", freq);
    // Error: bad frequency!
    // What can we do? Should this function return something?
    // Set some defaults?
}

void
pwm_duty_set (unsigned char duty)
{
    // TODO: Comments
    pwm_duty_ratio = duty;
    pwm_duty_cycle = (unsigned int)lroundf(((float)duty/100) * (4 * (pwm_timer_period + 1)));
    LOG_DEBUG("Duty cycle: [%i%%]%i", duty, pwm_duty_cycle);

    PWM4DC = (pwm_duty_cycle << 6);
}


// EOF //
