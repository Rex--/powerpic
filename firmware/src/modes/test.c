/** @file test.c
 * 
 * This mode application implements various tests.
*/

#include <xc.h>

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/buttons.h"
#include "lib/alarm.h"

#include "modes/test.h"

// Define a LOG_TAG before importing logging lib. This will get preppended to
// each log message.
//
#define LOG_TAG "mode.test"
#include "lib/logging.h"


time_t alarm_time = {
    .hour = 0x16,
    .minute = 0x20,
    .second = 0x00,
};

date_t alarm_date = {
    .day = 0x20,
    .month = 0x04,   // We never check dates past the day
    .year = 0x22,
    .weekday = 0x06
};

datetime_t alarm_dt;

void
test_init (void)
{
    LOG_INFO("Initializing World!");

    // Test alarm_set_datetime()
    alarm_dt.date = alarm_date;
    alarm_dt.time = alarm_time;
    alarm_set_datetime(&alarm_dt, 0xab);
}

void
test_start (void)
{
    LOG_INFO("Starting World!");

    // Test alarm_get()
    alarm_dt = (datetime_t){0};
    alarm_get(&alarm_dt);

    LOG_INFO("Alarm datetime: %.2i:%.2i:%.2i 20%.2i/%.2i/%.2i %.2X0A",
        BCD2DEC(alarm_dt.time.hour),
        BCD2DEC(alarm_dt.time.minute),
        BCD2DEC(alarm_dt.time.second),
        BCD2DEC(alarm_dt.date.year),
        BCD2DEC(alarm_dt.date.month),
        BCD2DEC(alarm_dt.date.day),
        alarm_dt.date.weekday
    );
}

void
test_run (unsigned int event)
{
    // This will cause problems @ 100ms tickrate
    LOG_DEBUG("Running World!");

    if (EVENT_TYPE(event) == EVENT_TICK)
    {

    }

    else if (EVENT_TYPE(event) == EVENT_ALARM)
    {
        // Test alarm_set_time()
        datetime_time_now(&alarm_time);
        alarm_time.second = 
            (unsigned char)DEC2BCD(
                (unsigned char)(BCD2DEC(alarm_time.second) + 10)
            );
        alarm_set_time(&alarm_time, 0xFF);
    }
    
    else if (EVENT_TYPE(event) == EVENT_KEYPAD)
    {
        LOG_INFO("Keypress: %c", EVENT_DATA(event));
    }

    else if (EVENT_TYPE(event) == EVENT_BUTTON)
    {
        if (EVENT_DATA(event) == BUTTON_MODE_RELEASE)
        {
            
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            LOG_INFO("Adj button press");
        }
        if (EVENT_DATA(event) == BUTTON_ADJ_RELEASE)
        {

        }
    }
}

void
test_stop (void)
{
    // I at least have to warn people.
    LOG_WARNING("Stopping World!");
}
