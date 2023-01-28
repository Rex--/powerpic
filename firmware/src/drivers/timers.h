/** @file timers.h
 * Driver for the various timers on the PIC16F1919x.
 * The timers are used as follows:
 * - timer0: 'tick' interrupt for mode application's tickrate.
 * - timer1:
 * - timer2: PWM3 - Backlight
 * - timer4: PWM4 - Buzzer
*/

#ifndef _timers_h_
#define _timers_h_

/**
 * Setup timer0.
 * This configures timer0 to be used for our system ticks.
 * 
*/
void    timer0_init (void);

/**
 * Start timer0.
 * Starts the timer counting from the last value. 
*/
#define timer0_start()  (T0CON0bits.T0EN = 1)

/**
 * Stop timer0.
 * Stops the timer.
*/
#define timer0_stop()   (T0CON0bits.T0EN = 0)

/**
 * Enable timer0 interrupts.
 * An interrupt is generated whenever the timer overflows from 0xFFFF to 0x0.
*/
#define timer0_interrupt_enable() PIE0bits.TMR0IE = 1

/**
 * Disable timer0 interrupts.
 * Interrupts will no longer be generated on timer0 overflows.
*/
#define timer0_interrupt_disable() (PIE0bits.TMR0IE = 0)

/**
 * Clear timer0 interrupt flag.
 * This must to be called after servicing the interrupt.
*/
#define timer0_interrupt_clear()  (PIR0bits.TMR0IF = 0)


/**
 * Set timer0 value.
 * This allows you to start the timer from a non-zero value or reset the timer.
 * 
 * @param[in] value 16-bit Timer value
*/
#define timer0_set(value) TMR0H = (value) >> 8; TMR0L = (value) & 0xFF

/**
 * Get timer0 value.
*/
#define timer0_get(value) (TMR0L | (TMR0H << 8))

/**
 * Set timer prescaler value.
 * 
 * @param[in] value The value to set the prescaler to.
 * This is a byte with the lower 4-bits representing the prescaler value. The
 * 4 high bits are ignored.
*/
#define timer0_prescaler_set(value) T0CON1bits.T0CKPS = ((value) & 0x0F)

/**
 * Set timer postscaler value.
 * 
 * @param[in] value The value to set the postscaler to.
 * This is a byte with the lower 4-bits representing the postscaler value. The
 * 4 high bits are ignored.
*/
#define timer0_postscaler_set(value) T0CON0bits.T0OUTPS = ((value) & 0x0F)


/**
 * Initialize timer1
*/
void    timer1_init (void);

/**
 * Start timer1.
 * This sets the ON bit in T1CON.
*/
#define timer1_start()      (T1CONbits.ON = 1)

/**
 * Stop timer1.
 * This clears the ON bit in T1CON.
*/
#define timer1_stop()      (T1CONbits.ON = 0)

/**
 * Get timer1 value.
 * This currently reads both 8-bit registers separately to get the 16-bit timer
 * value. We should look into the 16-bit access mode (configured by setting
 * T1CON.RD16)
*/
#define timer1_get()        (unsigned)(((TMR1H) << 8) | (TMR1L))


/**
 * Initialize timer4.
 * This configures timer4 to be used for PWM generation for the buzzer.
*/
void    timer4_init (void);

/**
 * Start timer4.
*/
#define timer4_start()      (T4CONbits.ON = 1)

/**
 * Stop timer4.
*/
#define timer4_stop()       (T4CONbits.ON = 0)

/**
 * Set timer4 period value.
*/
#define timer4_period_set(period)   (T4PR = (period))

/**
 * Set timer4 prescaler.
*/
#define timer4_prescaler_set(ps)    (T4CONbits.CKPS = ((ps) & 0x0F))

/**
 * Sync or Async operation of timer4.
*/
#define timer4_psync(s)           (T4HLTbits.PSYNC = (s))

#endif

// EOF //
