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


#define TICK_PRESCALER_MS       0b0101

#define TICK_PRESCALER_SEC      0b1111


static void tick_isr (void);

/** The tickrate currently configured. */
static unsigned int tick_rate;

/** The timer value to start couting from. */
static unsigned int tick_timer_seed;

/** The prescaler currently configured. */
static unsigned char tick_prescaler;

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
tick_rate_set_ms (unsigned int ms)
{
    // Stop tick timer to prevent interrupts while setting a new tickrate.
    //
    tick_disable();

    LOG_INFO("Setting tickrate: %u ms", ms);

    // Record our new config.
    tick_rate = ms;
    tick_timer_seed = (65535 - tick_rate) + 1;
    tick_prescaler = TICK_PRESCALER_MS;

    // Set prescaler to 1:32
    timer0_prescaler_set(TICK_PRESCALER_MS);

    // Seed timer with our new tickrate
    timer0_set(tick_timer_seed);

    // Start tick ticking
    tick_enable();

}

void
tick_rate_set_sec (unsigned int sec)
{
    // Stop tick timer to prevent interrupts while setting a new tickrate.
    //
    tick_disable();

    LOG_INFO("Setting tickrate: %u sec", sec);

    // Record our new config.
    tick_rate = sec;
    tick_timer_seed = (65535 - tick_rate) + 1;
    tick_prescaler = TICK_PRESCALER_SEC;

    // Set prescaler to 1:32768
    timer0_prescaler_set(TICK_PRESCALER_SEC);

    // Seed timer with our new tickrate
    timer0_set(65535-tick_rate);

    // Start tick ticking
    tick_enable();
}

unsigned int
tick_rate_get (void)
{
    return tick_rate;
}

unsigned char
tick_prescaler_get (void)
{
    return tick_prescaler;
}

void
tick_reset (void)
{
    // We can't reset this here because the tick is reset after every tick
    // event is processed. So the other function is needed.
    // tick_counter = 0; 
    timer0_set(tick_timer_seed);

    // Enable ticks if they aren't already
    // tick_enable();
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
    timer0_set(tick_timer_seed);
}

// EOF //
