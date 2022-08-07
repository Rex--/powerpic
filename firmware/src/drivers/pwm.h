/** @file pwm.h
 * 
 * PWM driver for PIC16LF1919x.
 * 
 * This driver implements a single PWM module, PWM4, using timer4.
*/

#ifndef _pwm_h_
#define _pwm_h_

/**
 * Initialize the pwm driver for use.
*/
void
pwm_init (void);

/**
 * Set the duty cycle of the pwm output.
*/
void
pwm_duty_set (unsigned char duty_cycle);

/**
 * Gets the current duty cycle of the pwm output.
*/
unsigned char
pwm_duty_get (void);


/**
 * Set the frequency of the pwm output.
*/
void
pwm_freq_set (unsigned int freq);

/**
 * Get the current frequency of the pwm output.
*/
unsigned int
pwm_freq_get (void);

/**
 * Enables the PWM output.
*/
void
pwm_enable (void);

/**
 * Disables the PWM output.
*/
void
pwm_disable (void);

#endif

// EOF //
