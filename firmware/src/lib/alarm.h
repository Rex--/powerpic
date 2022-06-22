/** @file alarm.h
 * 
 * This library implements configurable alarms.
 * 
 * The alarm is implemented using the RTCC module's alarm option. This generates
 * an interrupt for a compare match with a given time and date value.
 * Unfortunately, only one alarm is available so we have to do some checking
 * and place the earliest requested alarm in the registers. Then on each
 * interrupt we set the alarm registers based on the next alarm to occur.
 * 
 * Each alarm interrupt will generate an event with the ALARM_EVENT type. The
 * event data will be the value given when the alarm was registered.
*/

#ifndef _alarm_h_
#define _alarm_h_

/**
 * Max alarms that can be registered at once.
 * 
 * A few alarms are going to always be registered:
 * - Hourly chime alarm? Might have its own ISR
 * - User configurable alarm
 * 
 * A couple extra should be enough.
*/
#define ALARM_MAX_ALARMS    4

// We use datetime objects for time and date values
#include "lib/datetime.h"


/**
 * Initialize alarm library.
*/
void
alarm_init (void);

/**
 * Set an alarm for the given time.
 * 
 * If the given time has already occurred today, the alarm will be set for tomorrow.
 * 
*/
void
alarm_set_time (time_t *alarm_time, unsigned char event_data);


/**
 * Set an alarm for a given datetime.
*/
void
alarm_set_datetime (datetime_t *alarm_datetime, unsigned char event_data);

/**
 * Gets the current set alarm.
*/
void
alarm_get (datetime_t *alarm_datetime);


// Events defines
//

#define ALARM_EVENT         0x0A

#endif

// EOF //
