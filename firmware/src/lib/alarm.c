/** @file alarm.h
 * 
 * This library implements configurable alarms.
*/

#include <xc.h>

#include "drivers/rtcc.h"

#include "lib/isr.h"
#include "lib/events.h"

#define LOG_TAG "lib.alarm"
#include "lib/logging.h"

#include "lib/alarm.h"


/**
 * This array holds the alarms that are currently registered.
*/
static datetime_t registered_alarms[ALARM_MAX_ALARMS] = {0};


/** Number of currently registered alarms. */
static unsigned char registered_alarms_count = 0;

/**
 * This inserts an alarm at the given index. Shifts alarms around if needed.
*/
void alarm_insert(unsigned char index, datetime_t *alarm_dt);

/** Alarm interrupt service routine. */
static void alarm_isr (void);

void
alarm_init (void)
{
    registered_alarms_count = 0;

    // Set alarm mask to match HH:MM:SS
    rtcc_alarm_mask_set(0b0110);

    // Register alarm isr
    isr_register(8, _PIR8_RTCCIF_MASK, &alarm_isr);

    // Enable alarm interrupts
    rtcc_alarm_interrupt_enable();
}

void
alarm_get (datetime_t *alarm_datetime)
{
    *alarm_datetime = registered_alarms[0];
}

void
alarm_set_time (time_t *alarm_time, unsigned char event_data)
{
    // Check if we have a free spot
    //
    if ((ALARM_MAX_ALARMS > registered_alarms_count))
    {
        // Our datetime object that represents this alarm.
        datetime_t alarm_dt;

        // Set the time of our alarm.
        alarm_dt.time = *alarm_time;

        // **NOTE**: if a 0 is in the second ones place, the alarm will not
        // occur. We work around this by just setting 0 to 1. An unwanted effect
        // of this hack is that e.g. 12:30:00 is considered the same as 12:30:01
        // and one will not be registered if the other is. We could solve this
        // by supporting more than one alarm event for a given time.
        if (0 == (alarm_dt.time.second & 0x0F))
        {
            alarm_dt.time.second++;
        }

        // Populate the alarm with current date.
        datetime_today(&alarm_dt.date);

        // Figure out true date of alarm.
        // NOTE: This will make all alarms sort correctly, but what happens
        // if today is the last day of the month? We work around this currently
        // by just matching the alarm time and not the date. It has yet to be
        // determined how this effects midnight rollover.
        //
        // Check to see if time is before right now.
        if (alarm_time->hour <= HOURS)
        {
            if (alarm_time->hour == HOURS)
            {
                if (alarm_time->minute <= MINUTES)
                {
                    if (alarm_time->minute == MINUTES)
                    {
                        if (alarm_time->second <= SECONDS)
                        {
                            if (alarm_time->second == SECONDS)
                            {
                                // ALARM IS RIGHT NOW! PANIC!
                            }
                            else
                            {
                                // If the alarm second is less than the current
                                // second, we assume the alarm is for tomrrow.
                                alarm_dt.date.day += 1;
                            }
                        }
                        else
                        {
                            // The alarm is in less than a minute.
                        }
                    }
                    else
                    {
                        // If the alarm minute is less than the current minute, we
                        // assume the alarm is for tomrrow.
                        alarm_dt.date.day += 1;
                    }
                }
                else
                {
                    // The alarm is in less than an hour.
                }
            }
            else
            {
                // If the alarm hour is less than the current hour, we assume the
                // alarm is for tomorrow.
                alarm_dt.date.day += 1;
            }
        }
        else
        {
            // If it is later than the current hour, we assume today.
        }
        
        // Set our datetime
        alarm_set_datetime(&alarm_dt, event_data);
    }
    else
    {
        // Max alarms have been added
        LOG_ERROR("Max alarms added: %.2i:%.2i:%.2i",
            BCD2DEC(alarm_time->hour),
            BCD2DEC(alarm_time->minute),
            BCD2DEC(alarm_time->second)
        );
    }
}

void
alarm_set_datetime (datetime_t *alarm_datetime, unsigned char event_data)
{
    // Check if we have a free spot
    //
    if ((ALARM_MAX_ALARMS > registered_alarms_count))
    {
        // TODO: Verify alarm is within bounds of time:
        // - Hour     0-23
        // - Minute   0-59
        // - Seconds *0-59

        // **NOTE**: if a 0 is in the second ones place, the alarm will not
        // occur. We work around this by just setting 0 to 1. An unwanted effect
        // of this hack is that e.g. 12:30:00 is considered the same as 12:30:01
        // and one will not be registered if the other is. We could solve this
        // by supporting more than one alarm event for a given time.
        if (0 == (alarm_datetime->time.second & 0x0F))
        {
            alarm_datetime->time.second++;
        }

        // We store the event data in the weekday field
        alarm_datetime->date.weekday = event_data;

        int alarm_index = -1;
        
        // Loop through the list of registered alarms and compare.
        // Right now we only compare up to the day, so monthly alarms might now
        // work correctly.
        for (unsigned char i = 0; i < registered_alarms_count; i++)
        {
            if (alarm_datetime->date.day < registered_alarms[i].date.day)
            {
                // This is our alarm index
                alarm_index = i;
                break;
            }
            else if (alarm_datetime->date.day == registered_alarms[i].date.day)
            {
                // If the day is the same, compare the times.

                if (alarm_datetime->time.hour < registered_alarms[i].time.hour)
                {
                    // This is our alarm index
                    alarm_index = i;
                    break;
                }
                else if (alarm_datetime->time.hour == registered_alarms[i].time.hour)
                {
                    // If the hour is the same, compare the minute.

                    if (alarm_datetime->time.minute < registered_alarms[i].time.minute)
                    {
                        // This is our alarm index
                        alarm_index = i;
                        break;
                    }
                    else if (alarm_datetime->time.minute == registered_alarms[i].time.minute)
                    {
                        // If the minute is the same, compare the seconds.

                        if (alarm_datetime->time.second < registered_alarms[i].time.second)
                        {
                            // This is our alarm index
                            alarm_index = i;
                            break;
                        }
                        else if (alarm_datetime->time.second == registered_alarms[i].time.second)
                        {
                            // An alarm with that time is already registered.
                            LOG_WARNING("Duplicate alarm");
                            return;
                        }
                        else
                        {
                            // Alarm is after this index

                            // Check if this is the last registered alarm.
                            if ((i+1) == registered_alarms_count)
                            {
                                // This is our alarm index
                                alarm_index = i+1;
                                break;
                            }
                        }   
                    }
                    else
                    {
                        // Alarm is after this index

                        // Check if this is the last registered alarm.
                        if ((i+1) == registered_alarms_count)
                        {
                            // This is our alarm index
                            alarm_index = i+1;
                            break;
                        }
                    }
                }
                else
                {
                    // Alarm is after this index

                    // Check if this is the last registered alarm.
                    if ((i+1) == registered_alarms_count)
                    {
                        // This is our alarm index
                        alarm_index = i+1;
                        break;
                    }
                }
            }
            else
            {
                // Alarm is after this index

                // Check if this is the last registered alarm.
                if ((i+1) == registered_alarms_count)
                {
                    // This is our alarm index
                    alarm_index = i+1;
                    break;
                }
            }
        }

        // Make sure our alarm index has been set in the spaghetti above.
        if (-1 == alarm_index)
        {
            // Check if we have any alarms registered
            if (0 == registered_alarms_count)
            {
                // Alarm is the only registered alarm
                alarm_index = 0;
            }
            else
            {
                // We should never get here
                LOG_ERROR("Couldn't register alarm!");
                return;
            }
        }

        // Increment the count of registered alarms.
        registered_alarms_count++;

        // Insert our alarm into the list.
        alarm_insert((unsigned char)alarm_index, alarm_datetime);

        LOG_INFO("Registered alarm (%i/%i) %.2i:%.2i:%.2i %.2i/%.2i x%.2X%.2X",
            alarm_index,
            registered_alarms_count,
            BCD2DEC(alarm_datetime->time.hour),
            BCD2DEC(alarm_datetime->time.minute),
            BCD2DEC(alarm_datetime->time.second),
            BCD2DEC(alarm_datetime->date.day),
            BCD2DEC(alarm_datetime->date.month),
            alarm_datetime->date.weekday,
            ALARM_EVENT
        );


    }
    else
    {
        // Max alarms have been added
        LOG_ERROR("Max alarms added: %.2i:%.2i:%.2i",
            BCD2DEC(alarm_datetime->time.hour),
            BCD2DEC(alarm_datetime->time.minute),
            BCD2DEC(alarm_datetime->time.second)
        );
    }
}

unsigned char
alarm_del_event (unsigned char event_data)
{
    unsigned char deleted_alarms = 0;

    // Loop through the registered_alarm list and remove alarms that have the
    // specified event_data. The event_data is stored in the weekday field.

    for (int i = registered_alarms_count-1; i >= 0; i--)
    {
        if (event_data == registered_alarms[i].date.weekday)
        {
            // Remove alarm and shift all other up.
            registered_alarms[i] = (datetime_t){0};
            deleted_alarms++;

            for (int j = i+1; j < registered_alarms_count; j++)
            {
                registered_alarms[j-1] = registered_alarms[j];
            }
        }
    }

    return deleted_alarms;
}


void
alarm_insert (unsigned char index, datetime_t *alarm_dt)
{
    // Shift alarms behind 
    for (int i = registered_alarms_count; i >= index; i--)
    {
        registered_alarms[i+1] = registered_alarms[i];
    }

    // Insert alarm at the index
    registered_alarms[index] = *alarm_dt;

    // If the alarm is going to happen the soonest (index 0), then set the
    // alarm registers.

    if (0 == index)
    {
        // Disable alarm to change registers
        rtcc_alarm_disable();

        // Set Registers
        // ALRMDAY = registered_alarms[0].date.day;
        ALRMHR  = registered_alarms[0].time.hour;
        ALRMMIN = registered_alarms[0].time.minute;
        ALRMSEC = registered_alarms[0].time.second;

        // Enable alarm
        rtcc_alarm_enable();
    }
}


static void
alarm_isr (void)
{
    // Alarm with index 0 has occurred. alarm is disabled.
    rtcc_alarm_disable();

    // Emit an alarm event with the specified data.
    event_isr((unsigned)
        EVENT_ID(
            ALARM_EVENT,
            registered_alarms[0].date.weekday)
    );

    // Shift alarms down
    for (int i = 1; i < registered_alarms_count; i++)
    {
        registered_alarms[i-1] = registered_alarms[i];
    }

    // Decrement count of registered alarms
    registered_alarms_count--;

    // Check if we have any alarms registered
    if (registered_alarms_count)
    {
        // Set alarm registers with index 0 values.
        ALRMHR  = registered_alarms[0].time.hour;
        ALRMMIN = registered_alarms[0].time.minute;
        ALRMSEC = registered_alarms[0].time.second;
        
        rtcc_alarm_enable();
    }

    // Clear alarm interrupt flag
    rtcc_alarm_interrupt_clear();
}


// EOF //
