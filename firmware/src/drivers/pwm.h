#ifndef PWM_H
#define PWM_H

#define PWM4_ENABLE (PWM4CONbits.PWM4EN = 1)
#define PWM4_DISABLE (PWM4CONbits.PWM4EN = 0)

void    pwm4_init (void);

void    pwm4_set_duty_cycle (unsigned int duty_cycle);

void    pwm4_set_frequency (unsigned char frequency);

#endif