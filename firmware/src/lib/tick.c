
#include <xc.h>

#include "drivers/timers.h"

#include "lib/isr.h"
#include "lib/event.h"

#include "lib/tick.h"


#define TICK_RATE_DEFAULT    1000


static unsigned int tick_rate = 0;

static unsigned char tick_tick = 0;

void tick_isr (void);

/**
 * Enable system ticks.
 * Timer0 is used to provide a tick every set amount of milliseconds. This tick
 * can generate an interrupt to wake up from sleep.
*/
void
tick_enable (void)
{
    tick_tick = 0;
    tick_rate = 65535 - TICK_RATE_DEFAULT;

    timer0_init();

    // 1:32 - This value is chosen for the low freq osc that runs at 31kHz.
    // This gives us a timer clock of ~1ms. Eventually the 32.768kHz SOSC will
    // be used as a clock source.
    //
    timer0_prescaler_set(0b0101);

    timer0_set(tick_rate);

    // Register isr
    isr_register(0, _PIE0_TMR0IE_MASK, &tick_isr);

    // Enable interrupts and start timer0.
    // timer0_interrupt_enable();
    // timer0_start();
}

void
tick_start (void)
{
    timer0_start();
}

void
tick_stop (void)
{
    timer0_stop();
}

void
tick_set_ps (unsigned char scale)
{
    timer0_postscaler_set(scale);
}


void
tick_set_ms (unsigned int rate)
{
    tick_rate = 65535 - rate;
    timer0_set(tick_rate);
}


void
tick_reset (void)
{
    tick_tick = 0;
    timer0_set(tick_rate);
}


void
tick_enable_interrupts (void)
{
    timer0_interrupt_enable();
}

void
tick_disable_interrupts (void)
{
    timer0_interrupt_disable();
}

void
tick_isr (void)
{
    // timer0_set(tick_rate);

    //emit event
    event_add(0x01, (unsigned char)(tick_tick++));

    //clear flag
    PIR0bits.TMR0IF = 0;
}