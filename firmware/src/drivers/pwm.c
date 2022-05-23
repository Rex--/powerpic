
#include <xc.h>

#include "dev_config.h"


#define DUTY_CYCLE 130

static int pwm_timer_prescale = 0;

void
pwm4_init (void)
{
#   if (1 == PCB_REV)
    // Set pwm pin to output and configure pps
    //
    TRISGbits.TRISG7 = 0;
    RG7PPS = 0x0c;
#   endif

#   if (2 == PCB_REV)
    TRISGbits.TRISG6 = 0;
    RG6PPS = 0x0C;
#   endif


    // Set up pwm module
    //
    // PWM4DC = DUTY_CYCLE<<6;
    // PWM4CONbits.PWM4EN = 1;

    // set up timer2
    //
    T2CLKCON = 0x01; // clock - Fosc/4
    T2HLTbits.PSYNC = 1; // Synced with Fosc
    T2CONbits.CKPS = 0b101; // 1:32
    // T2PR = 64;
    T2CONbits.ON = 1;
}

void
pwm4_set_duty_cycle (unsigned int duty_cycle)
{
    PWM4DC = duty_cycle << 6;
}

void
pwm4_set_frequency (unsigned char frequency)
{
    T2PR = frequency;
    // if (frequency > 1000)
    // {
    //     return;
    // }
    // else if (frequency > 500)
    // {

    // }
    // else if (frequency > 250)
    // {
    //     T2CONbits.CKPS = 2; // 1:4 prescaler
    // }
}
