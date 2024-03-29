/** @file pwm.h
 * 
 * PWM driver for PIC16LF1919x.
*/

#include <xc.h>
#include <math.h>   // for llroundf()

#include "drivers/timers.h"

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

#   else
    // Set PWM pin as output
    // We use RG6 for the buzzer
    //
    TRISGbits.TRISG6 = 0;
    RG6PPS = 0x0C;
#   endif

    // Init up timer4
    //
    timer4_init();

    // Configure PWM4 to use timer4
    CCPTMRS0bits.P4TSEL = 0b10;
}

void
pwm_enable (void)
{
    // Enable timer
    timer4_start(); 

    // Enable PWM
    PWM4CONbits.PWM4EN = 1;
}

void
pwm_disable (void)
{
    // Disable pwm
    PWM4CONbits.PWM4EN = 0;

    // Disable timer
    timer4_stop();
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
    // As prescaler values get lower, it allows higher frequencies.
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
            pwm_frequency = freq;

            pwm_timer_period = (unsigned char)timer_period;
            timer4_period_set(pwm_timer_period); // TODO: Don't actually set the period registers until we enable.
            pwm_timer_prescaler = timer_prescale;
            timer4_prescaler_set(pwm_timer_prescaler); // TODO: Same with prescaler
            return;
        }
    }
    // Error: bad frequency!
    // What can we do? Should this function return something?
    // Set some defaults?
}

unsigned int
pwm_freq_get (void)
{
    return pwm_frequency;
}


void
pwm_duty_set (unsigned char duty)
{
    pwm_duty_ratio = duty; // LOL -- DUTY!
    pwm_duty_cycle = (unsigned int)lroundf(((float)duty/100) * (4 * (pwm_timer_period + 1)));

    PWM4DC = (pwm_duty_cycle << 6);
}

unsigned char
pwm_duty_get (void)
{
    return pwm_duty_ratio;
}


// EOF //
