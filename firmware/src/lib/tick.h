/** @file tick.h
 * Tick library for CasiOS.
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
 * Set the tickrate in milliseconds.
 * This uses a 1:32 prescaler for a 32.768KHz clock, so ticks are ~1ms.
 * Tickrates below ~150 ms can get iffy if your mode run function takes a while.
*/
void
tick_rate_set_ms (unsigned int ms);

/**
 * Set the tickrate in seconds.
 * This uses a 1:32768 prescaler for a 32.768KHz clock, so tick are pretty darn
 * close to 1 second.
 * NOTE: tick_rate_set_s() feels weird.
*/
void
tick_rate_set_sec (unsigned int sec);

/**
 * Get the current tickrate.
*/
unsigned int tick_rate_get (void);

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
