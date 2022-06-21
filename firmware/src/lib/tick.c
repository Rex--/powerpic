/** @file tick.h
 * Tick library for PowerOS.
*/

#include <xc.h>
#include <stdlib.h>

#include "drivers/timers.h"

#include "lib/isr.h"
#include "lib/events.h"
#include "lib/logging.h"

#include "lib/tick.h"

#undef  LOG_TAG
#define LOG_TAG "lib.tick"

#define TICK_MAX_FINE   65535
#define TICK_PRESCALER_FINE     0b0101

#define TICK_MAX_COURSE 65535000
#define TICK_PRESCALER_COURSE   0b1111


static void tick_isr (void);

/** The tickrate currently configured. */
static long tick_rate;

/**
 * The tickrate that has been scaled.
 * This is the timer value to achieve our desired tickrate.
*/
static unsigned int  tick_rate_scaled;

/**
 * Tick counter for tick events.
 * This is joined with the tick event for a tick event id.
*/
static volatile unsigned char tick_counter;

void
tick_init (void)
{
    LOG_INFO("Initializing tick...");

    tick_counter = 0;

    // We use timer0 for our tick timer
    timer0_init();

    // Register ISR
    isr_register(0, _PIR0_TMR0IF_MASK, &tick_isr);
}

void
tick_enable (void)
{
    timer0_interrupt_enable();

    timer0_start();
}

void
tick_disable (void)
{
    timer0_stop();

    timer0_interrupt_disable();
}

void
tick_rate_set (long rate)
{
    // Get the absolute value of rate. Eventually negative will do something.
    //
    rate = labs(rate);

    // Disable tick to prevent interrupts while setting a new tickrate.
    //
    tick_disable();

    LOG_INFO("Setting tickrate to %li", rate);
    tick_rate = rate;

    // If the mode requests a 0 rate, we disable ticks.
    // NOTE: This will proably be moved to -1 eventually.
    // MOTE: 0 will loop without delay
    //
    if (rate)
    {
        // If our tickrate is less than 65535, we use 1:32 or FINE prescaler.
        // This allows ~1ms granularity in tickrates.
        if (TICK_MAX_FINE > rate)
        {
            LOG_DEBUG("Setting prescaler to FINE");
            tick_rate_scaled = TICK_MAX_FINE - (unsigned int)rate;
            timer0_prescaler_set(TICK_PRESCALER_FINE);
        }

        // Else we use the 1:32768 or COURSE prescaler. This only allows ~1s
        // granularity in tickrates but allows us a max tickrate of
        // 65,535,000ms or ~1 hour. Eventually we could have 'subticks' and
        // allow longer tickrates but maybe the mode that needs that long should
        // refactor.
        //
        else
        {
            LOG_DEBUG("Setting prescaler to COURSE");
            tick_rate_scaled = TICK_MAX_FINE - (unsigned int)(rate / 1000);
            timer0_prescaler_set(TICK_PRESCALER_COURSE);
        }

        // After scaling our tickrate and setting the prescaler we set the
        // timer value and start it ticking.
        //
        LOG_DEBUG("Scaled tickrate is %u", tick_rate_scaled);
        timer0_set(tick_rate_scaled);
        tick_enable();
    }

    // Our tickrate is 0, we disable ticks entirely.
    //
    else
    {
        tick_disable();
    }
}

long
tick_rate_get (void)
{
    return tick_rate;
}

void
tick_reset (void)
{
    // We can't reset this here because the tick is reset after every tick
    // event is processed. So the other function is needed.
    // tick_counter = 0; 
    timer0_set(tick_rate_scaled);

    // Enable ticks if they aren't already
    tick_enable();
}

void
tick_counter_reset (void)
{
    tick_counter = 0;
}

static void
tick_isr (void)
{
    // Emit tick event
    event_isr((unsigned int)EVENT_ID(TICK_EVENT, tick_counter++));

    // Clear interrupt flag.
    timer0_interrupt_clear();

    // Reset tick timer.
    timer0_set(tick_rate_scaled);
}

// EOF //
