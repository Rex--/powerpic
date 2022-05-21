#include <xc.h>

#include "dev_config.h"

#include "drivers/pwm.h"

#include "lib/buzzer.h"


#define BUZZER_BEEP_FREQ_DEFAULT 64
#define BUZZER_BEEP_DUTY_DEFAULT 130

void
buzzer_enable (void)
{
    pwm4_init();
    pwm4_set_duty_cycle(BUZZER_BEEP_DUTY_DEFAULT);
    pwm4_set_frequency(BUZZER_BEEP_FREQ_DEFAULT);
}

void
buzzer_beep (void)
{
    PWM4_ENABLE;
    __delay_ms(75);
    PWM4_DISABLE;
    __delay_ms(100);
    PWM4_ENABLE;
    __delay_ms(75);
    PWM4_DISABLE;
    // __delay_ms(100);
    // PWM4_ENABLE;
    // __delay_ms(500);
    // PWM4_DISABLE;
}
