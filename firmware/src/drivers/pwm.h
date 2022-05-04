#ifndef PWM_H
#define PWM_H

#define PWM_ENABLE (PWM4CONbits.PWM4EN = 1)
#define PWM_DISABLE (PWM4CONbits.PWM4EN = 0)

void    pwm_init (void);

#endif