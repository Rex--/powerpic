/** @file tick.h
 * Tick library for PowerOS.
 * Used to to generate an interrupt at configurable intervals.
 * Each tick < 65535 is ~1ms (1.024kHz)
 *   The external 32.768kHz xtal with a 1:32 prescaler is used for tickrates
 *   less than 16-bits.
 * Each tick > 65535 is only configurable to the nearest second.
 *   The external xtal with a 1:32768 prescaler is used for tickrates above
 *   16-bits. The tickrate is divided by 1,000 and capped at 65535 seconds.
*/

#ifndef _tick_h_
#define _tick_h_

#include "drivers/timers.h"

// Tick Events
//

#define TICK_EVENT      0x01

/**
 * Initialize the tick library.
*/
void tick_init (void);

/**
 * Enable ticks.
 * This enables tick events.
*/
void
tick_enable (void);

/**
 * Disable ticks.
*/
void
tick_disable (void);

/**
 * Set the tick rate.
 * This sets how often a tick interrupt is generated.
*/
void tick_rate_set (long rate);

/**
 * Get the current tickrate.
*/
long tick_rate_get (void);

/**
 * Set the tick prescaler.
 * This modifies the tick clock prescaler to allow a wider range of tickrates.
 * Setting a tickrate automatically figures out the best prescaler value and
 * sets it, so no need to manually change this.
*/
void tick_prescaler_set (unsigned char scale);

/**
 * Get the current tick prescaler.
*/
unsigned char tick_prescaler_get (void);


/**
 * Reset the tick timer to the tickrate.
*/
void tick_reset (void);

/**
 * Reset the tick counter.
 * The counter is incremented every tick event and gets combined with
 * TICK_EVENT to form an event id. We explicitly reset this counter when we
 * switch modes.
*/
void tick_counter_reset (void);

#endif

// EOF //
