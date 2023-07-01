/** @file alarm.c
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

/** Index of current registered alarm. */
static unsigned char registered_alarms_head = 0;

/**
 * This inserts an alarm at the given index. Shifts alarms around if needed.
*/
void alarm_insert(unsigned char index, datetime_t *alarm_dt);

/**
 * Delete an alarm event at a given index. Shifts the the alarms up.
*/
void alarm_delete(unsigned char index);

/** Alarm interrupt service routine. */
static void alarm_isr (void);

void
alarm_init (void)
{
    registered_alarms_count = 0;
    registered_alarms_head = 0;

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
    *alarm_datetime = registered_alarms[registered_alarms_head];
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

        // Populate the alarm with current date.
        datetime_today(&alarm_dt.date);

        // Figure out true date of alarm.
        // NOTE: This will make all alarms sort correctly, but what happens
        // if today is the last day of the month? We work around this currently
        // by just matching the alarm time and not the date. It has yet to be
        // determined how this effects midnight rollover.
        //
        // A potential bug is when the next alarm to occur is not on the next
        // day.
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
                                // The caller probably wants it tomorrow?
                                alarm_dt.date.day += 1;
                                //Subject to change
                            }
                            else
                            {
                                // If the alarm second is less than the current
                                // second, we assume the alarm is for tomorrow.
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
                        // assume the alarm is for tomorrow.
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

        // *NOTE: if a 0 is in the second ones place, the alarm will not
        // occur. We work around this by just setting 0 to 1. An unwanted effect
        // of this hack is that e.g. 12:30:00 is considered the same as 12:30:01
        if (0 == (alarm_datetime->time.second & 0x0F))
        {
            alarm_datetime->time.second++;
        }

        // We store the event data in the weekday field
        alarm_datetime->date.weekday = event_data;

        int alarm_index = -1;
        unsigned char alarm_position = 0;
        
        // Loop through the list of registered alarms and compare.
        // Right now we only compare up to the day, so monthly alarms might now
        // work correctly.
        for (unsigned char c = 0; c < registered_alarms_count; c++)
        {
            // Loop from head to tail based on the head and the length
            unsigned char i = (registered_alarms_head + c) % ALARM_MAX_ALARMS;
            alarm_position = c;

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
                            // Check if this is the last registered alarm.
                            if ((c+1) == registered_alarms_count)
                            {
                                LOG_WARNING("Duplicate alarm added: %.2i:%.2i:%.2i",
                                    BCD2DEC(alarm_datetime->time.hour),
                                    BCD2DEC(alarm_datetime->time.minute),
                                    BCD2DEC(alarm_datetime->time.second)
                                );
                                // This is our alarm index
                                alarm_index = i+1;
                                alarm_position++;
                                break;
                            }
                        }
                        else
                        {
                            // Alarm is after this index

                            // Check if this is the last registered alarm.
                            if ((c+1) == registered_alarms_count)
                            {
                                // This is our alarm index
                                alarm_index = i+1;
                                alarm_position++;
                                break;
                            }
                        }   
                    }
                    else
                    {
                        // Alarm is after this index

                        // Check if this is the last registered alarm.
                        if ((c+1) == registered_alarms_count)
                        {
                            // This is our alarm index
                            alarm_index = i+1;
                            alarm_position++;
                            break;
                        }
                    }
                }
                else
                {
                    // Alarm is after this index

                    // Check if this is the last registered alarm.
                    if ((c+1) == registered_alarms_count)
                    {
                        // This is our alarm index
                        alarm_index = i+1;
                        alarm_position++;
                        break;
                    }
                }
            }
            else
            {
                // Alarm is after this index

                // Check if this is the last registered alarm.
                if ((c+1) == registered_alarms_count)
                {
                    // This is our alarm index
                    alarm_index = i+1;
                    alarm_position++;
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
                alarm_index = registered_alarms_head;
            }
            else
            {
                // We should never get here
                LOG_ERROR("Couldn't register alarm!");
                return;
            }
        }

        // Handle tail wrapping
        alarm_index = alarm_index % ALARM_MAX_ALARMS;

        // Insert our alarm into the list.
        alarm_insert((unsigned char)alarm_index, alarm_datetime);

        LOG_INFO("Registered alarm (%i/%i) %.2i:%.2i:%.2i %.2i/%.2i x%.2X%.2X",
            alarm_position+1,
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

    for (unsigned char c = 0; c < registered_alarms_count; c++)
    {
        // Loop from head to tail based on the head and the length
        unsigned char i = (registered_alarms_head + c) % ALARM_MAX_ALARMS;

        if (event_data == registered_alarms[i].date.weekday)
        {
            LOG_DEBUG("Removing alarm: (x%.2X) %.2i:%.2i",
                registered_alarms[i].date.weekday,
                BCD2DEC(registered_alarms[i].time.hour),
                BCD2DEC(registered_alarms[i].time.minute)
            );

            // Remove alarm
            alarm_delete(i);
            deleted_alarms++;

            // Resume search from previous index
            c--;
        }
    }

    return deleted_alarms;
}

void
alarm_delete (unsigned char index)
{
    // Bail early if the index is not in the range of registered alarms.
    if (!(((index >= registered_alarms_head) && (index-registered_alarms_head <= registered_alarms_count)) ||
        ((index < registered_alarms_head) && (index < (registered_alarms_head+registered_alarms_count)%ALARM_MAX_ALARMS))))
        {
            LOG_ERROR("Alarm is out of bounds!");
            return;
        }

    // Shift up all alarms that are:
    // 1) Behind the head
    // 2) Behind the index
    // 3) Before the end of the buffer (no wrapping)
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    //    shift left ^ ^ ^

    if (index >= registered_alarms_head)
    {
        for (unsigned char i = index+1; i < ALARM_MAX_ALARMS; i++)
        {
            // Shift each time and date to the previous position
            registered_alarms[i-1].time = registered_alarms[i].time;
            registered_alarms[i-1].date = registered_alarms[i].date;
        }
    }


    // Next, handle the wrapping of the circular buffer if needed
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    // > (if count > 6)  ^

    if ((index >= registered_alarms_head) && (registered_alarms_head+registered_alarms_count > ALARM_MAX_ALARMS))
    {
        // Shift index 0 to the last index
        registered_alarms[ALARM_MAX_ALARMS-1].time = registered_alarms[0].time;
        registered_alarms[ALARM_MAX_ALARMS-1].date = registered_alarms[0].date;

        // Set index to 0
        // This will shift all alarms from index 1 until the tail in the next loop
        index = 0;
    }


    // Finally, shift the alarms at the beginning (end):
    // 1) Before the head
    // 2) Before the tail
    // 3) Behind the index
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    //   ^ ^ ^ shift left                  ^ ^ shift left

    if (registered_alarms_head+registered_alarms_count > ALARM_MAX_ALARMS)
    {
        for (unsigned char i = index+1; i < (registered_alarms_head+registered_alarms_count)%ALARM_MAX_ALARMS; i++)
        {
            // Shift each time and date to the previous position
            registered_alarms[i-1].time = registered_alarms[i].time;
            registered_alarms[i-1].date = registered_alarms[i].date;
        }
    }

    // Decrement alarm count
    registered_alarms_count--;

}

void
alarm_insert (unsigned char index, datetime_t *alarm_dt)
{
    // Shift down all alarms that are:
    // 1) Before the head
    // 2) Behind the index (in order)
    // 3) Before the tail
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    // ^ ^ ^ right shift                 ^ ^ right shift

    if (registered_alarms_head+registered_alarms_count > ALARM_MAX_ALARMS) // Check if the buffer wraps around
    {
        unsigned char start;
        if (index >= registered_alarms_head)
        {
            start = 0; // If index is behind the head, shift all of the alarms
        } else {
            start = index; // Else shift until the index
        }
        for (unsigned char i = (registered_alarms_head+registered_alarms_count)%ALARM_MAX_ALARMS;i > start; i--)
        {
            // Shift each time and date to the following position
            registered_alarms[i].time = registered_alarms[i-1].time;
            registered_alarms[i].date = registered_alarms[i-1].date;
        }
    }


    // Next, handle the wrapping of the circular buffer if needed
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    // ^ (if count > 6) <<

    if ((index >= registered_alarms_head) && (registered_alarms_head+registered_alarms_count > ALARM_MAX_ALARMS))
    {
        // Shift the last index to index 0
        registered_alarms[0].time = registered_alarms[ALARM_MAX_ALARMS-1].time;
        registered_alarms[0].date = registered_alarms[ALARM_MAX_ALARMS-1].date;
    }


    // Finally, shift the alarms at the end (beginning):
    // 1) After the head
    // 2) Before the end of the buffer
    // 3) Behind the index
    //
    //    head v   v index         index v     v head
    // 0 1 2 3 4 5 6 7 8 9           0 1 2 3 4 5 6 7 8 9
    // shift right ^ ^ ^

    if (index >= registered_alarms_head)
    {
        for (unsigned char i = ALARM_MAX_ALARMS-1; i > index; i--)
        {
            // Shift each time and date to the following position
            registered_alarms[i].time = registered_alarms[i-1].time;
            registered_alarms[i].date = registered_alarms[i-1].date;
        }
    }

    // After shift the alarms around we can insert it at the position
    registered_alarms[index] = *alarm_dt;

    // Increment alarm count
    registered_alarms_count++;

    // If the alarm is going to happen the soonest (head), then set the
    // alarm registers.
    if (index == registered_alarms_head)
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
    // Alarm at head has occurred. alarm is disabled.
    rtcc_alarm_disable();

    unsigned char consumed_alarms = 0;
    // Match all alarms and emit events
    for (unsigned char c = 0; c < registered_alarms_count; c++)
    {
        // Loop from head to tail based on the head and the length
        unsigned char i = (registered_alarms_head + c) % ALARM_MAX_ALARMS;

        if (registered_alarms[registered_alarms_head].time.second == registered_alarms[i].time.second)
        {
            if (registered_alarms[registered_alarms_head].time.minute == registered_alarms[i].time.minute)
            {
                if (registered_alarms[registered_alarms_head].time.hour == registered_alarms[i].time.hour)
                {
                    if (registered_alarms[registered_alarms_head].date.day == registered_alarms[i].date.day)
                    {
                        if (registered_alarms[registered_alarms_head].date.month == registered_alarms[i].date.month)
                        {
                            if (registered_alarms[registered_alarms_head].date.year == registered_alarms[i].date.year)
                            {
                                // Alarm matches
                                consumed_alarms++;
                                // Emit event
                                event_isr((unsigned)
                                    EVENT_ID(
                                        ALARM_EVENT,
                                        registered_alarms[i].date.weekday)
                                );
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }

    // Consume alarm(s)
    registered_alarms_head = (registered_alarms_head + consumed_alarms) % ALARM_MAX_ALARMS;

    // Decrement count of registered alarms
    registered_alarms_count -= consumed_alarms;

    // Check if we have any alarms registered
    if (registered_alarms_count)
    {
        // Set alarm registers with head values.
        ALRMHR  = registered_alarms[registered_alarms_head].time.hour;
        ALRMMIN = registered_alarms[registered_alarms_head].time.minute;
        ALRMSEC = registered_alarms[registered_alarms_head].time.second;
        
        rtcc_alarm_enable();
    }

    // Clear alarm interrupt flag
    rtcc_alarm_interrupt_clear();
}


// EOF //
