
#include <xc.h>
#include <stdio.h>

#include "drivers/timers.h"


/**
 * Union to hold the value of a 16-bit timer.
 * Bytes are accessible in the bytes array.
 * The full 16-bit value can be accessed with value.
*/
typedef union
{
    /** High byte is 1, low byte is 0.
    */
    unsigned char bytes[2];

    /** 16-bit timer value
    */
    unsigned short value;
} timer_16_t;

/**
 * Setup timer0.
 * This configures timer0 to be used for our system ticks.
 * 
*/
void
timer0_init (void)
{
    // 16-bit mode
    T0CON0bits.T016BIT = 1;

    // post scaler 1:1
    T0CON0bits.T0OUTPS = 0b0000;

#   if (1 == PCB_REV)
    // clock src LFINTOSC
    //
    T0CON1bits.T0CS = 0b100;
#   endif

// #   if (2 == PCB_REV)
    // clock src SOSC
    //
    T0CON1bits.T0CS = 0b110;
// #   endif

    // not syncd with Fosc to allow for operation during sleep
    T0CON1bits.T0ASYNC = 1;

    // default prescaler 1:1
    T0CON1bits.T0CKPS = 0b0000;
}

/**
 * Start timer0.
 * Starts the timer counting from the last value. 
*/
inline void
timer0_start (void)
{
    T0CON0bits.T0EN = 1;
}

/**
 * Stop timer0.
 * Stops the timer.
*/
inline void
timer0_stop (void)
{
    T0CON0bits.T0EN = 0;
}

/**
 * Set timer value.
 * This allows you to start the timer from a non-zero value or reset the timer.
*/
inline void
timer0_set (unsigned short value)
{
    timer_16_t timer0;

    timer0.value = value;

    TMR0H = timer0.bytes[1];
    TMR0L = timer0.bytes[0];
}

unsigned int
timer0_get (void)
{
    timer_16_t timer0;

    timer0.bytes[0] = TMR0L;
    timer0.bytes[1] = TMR0H;

    return timer0.value;
}

/**
 * Set timer prescaler value.
 * 
 * @param[in] value The value to set the prescaler to.
 * This is a byte with the lower 4-bits representing the prescaler value. The
 * 4 high bits are ignored.
*/
inline void
timer0_prescaler_set (unsigned char value)
{
    T0CON1 &= 0xF0;
    T0CON1 |= value & 0x0F;
}

inline void
timer0_postscaler_set (unsigned char value)
{
    T0CON0 &= 0xF0;
    T0CON0 |= value & 0x0F;
}

/**
 * Enable timer0 interrupts.
 * An interrupt is generated whenever the timer overflows from 0xFFFF to 0x0.
*/
inline void
timer0_interrupt_enable (void)
{
    PIE0bits.TMR0IE = 1;
}

/**
 * Disable timer0 interrupts.
 * Interrupts will no longer be generated on timer0 overflows.
*/
inline void
timer0_interrupt_disable (void)
{
    PIE0bits.TMR0IE = 0;
}
