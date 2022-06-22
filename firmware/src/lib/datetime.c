/** @file datetime.c
 * Date and Time library for PowerOS.
*/

#include <xc.h>

#include "drivers/rtcc.h"

#define LOG_TAG "lib.datetime"
#include "lib/logging.h"

#include "lib/datetime.h"


const char weekday_str[7][3] = {
    "SU",
    "MO",
    "TU",
    "WE",
    "TH",
    "FR",
    "SA"
};


void
datetime_init (void)
{
    LOG_INFO("Initializing rtcc...");
    // Init and enable rtcc driver
    rtcc_init();
    rtcc_writes_enable();
    while (0 == rtcc_status())
    {
        rtcc_enable();
    }
    rtcc_writes_disable();
}



// Datetime functions

void
datetime_set (datetime_t *dt)
{
    LOG_INFO("Setting datetime: %.2i:%.2i:%.2i 20%.2i/%.2i/%.2i %s",
        BCD2DEC(dt->time.hour),                 \
        BCD2DEC(dt->time.minute),               \
        BCD2DEC(dt->time.second),               \
        BCD2DEC(dt->date.year),                 \
        BCD2DEC(dt->date.month),                \
        BCD2DEC(dt->date.day),                  \
        datetime_weekday_str(dt->date.weekday)  \
    );
    rtcc_writes_enable();
    
    while (rtcc_writes_discouraged())
    {
        // Wait for sync bit
    }
    rtcc_time_set(dt->time.hour, dt->time.minute, dt->time.second);
    rtcc_date_set(dt->date.year, dt->date.month, dt->date.day, dt->date.weekday);
    rtcc_writes_disable();
}

void
datetime_now (datetime_t *dt)
{
    unsigned char *now_time = rtcc_time_get();
    dt->time.hour = *now_time++;
    dt->time.minute = *now_time++;
    dt->time.second = *now_time;

    unsigned char *now_date = rtcc_date_get();
    dt->date.year = *now_date++;
    dt->date.month = *now_date++;
    dt->date.day = *now_date++;
    dt->date.weekday = *now_date;

    LOG_DEBUG("Now: %.2i:%.2i:%.2i 20%.2i/%.2i/%.2i %s",
        BCD2DEC(dt->time.hour),                 \
        BCD2DEC(dt->time.minute),               \
        BCD2DEC(dt->time.second),               \
        BCD2DEC(dt->date.year),                 \
        BCD2DEC(dt->date.month),                \
        BCD2DEC(dt->date.day),                  \
        datetime_weekday_str(dt->date.weekday)  \
    );
}


// Time functions

void
datetime_time_set (time_t *t)
{
    LOG_INFO("Setting time: %.2i:%.2i:%.2i",    \
        BCD2DEC(t->hour),                       \
        BCD2DEC(t->minute),                     \
        BCD2DEC(t->second)                      \
    );
    rtcc_writes_enable();
    while (rtcc_writes_discouraged())
    {
        // Wait for sync bit
    }
    rtcc_time_set(t->hour, t->minute, t->second);
    rtcc_writes_disable();
}

void
datetime_time_now (time_t *t)
{
    unsigned char *now = rtcc_time_get();
    t->hour = *now++;
    t->minute = *now++;
    t->second = *now;

    LOG_DEBUG("Now: %.2i:%.2i:%.2i",   \
        BCD2DEC(t->hour),               \
        BCD2DEC(t->minute),             \
        BCD2DEC(t->second)              \
    );
}


// Date functions

void
datetime_today (date_t *d)
{
    unsigned char *now_date = rtcc_date_get();
    d->year = *now_date++;
    d->month = *now_date++;
    d->day = *now_date++;
    d->weekday = *now_date;

    // This warns of a stack overflow if you call _today()
    // from a mode application and have debug logging enabled.
#   if 0
    LOG_DEBUG("Today: 20%.2i/%.2i/%.2i",
        BCD2DEC(d->year),
        BCD2DEC(d->month),
        BCD2DEC(d->day)
        // &weekday_str[d->weekday][0]
    );
#   endif
}

const char *
datetime_weekday_str (unsigned char weekday)
{
    return &weekday_str[weekday][0];
}

// EOF //
