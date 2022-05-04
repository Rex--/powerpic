/** @file
 * RTCC Driver for PIC16LF1919x devices using the hardware RTCC module.
 * 
*/

#include <xc.h>
#include <stdio.h>


typedef struct
{
    // Year
    unsigned char year;

    // Month
    unsigned char month;

    // Day
    unsigned char day;

    // Hours
    unsigned char hour;
    
    // Minutes
    unsigned char minute;
    
    // Seconds
    union
    {
        struct
        {
            unsigned    second_ones : 4;
            unsigned    second_tens : 4;
        };
        unsigned char second;
    };
    
    
    // Weekday
    unsigned char weekday;

} datetime_t;

void rtcc_datetime_set (datetime_t *datetime);
datetime_t * rtcc_datetime_get (void);


// datetime_t four_twenty;

// datetime_t midnight;

/**
 * Init the real time clock-cal module.
 */
void
rtcc_init (void)
{

    // Set clock to 4:19
    //
    // four_twenty.second = 0x01;
    // four_twenty.minute = 0x19;
    // four_twenty.hour = 0x04;
    // four_twenty.day = 0x07;
    // four_twenty.month = 0x06;
    // four_twenty.year = 0x22;
    // four_twenty.weekday = 0x03;

    // midnight.second = 0x00;
    // midnight.minute = 0x00;
    // midnight.hour = 0x00;
    // midnight.day = 0x01;
    // midnight.month = 0x06;
    // midnight.year = 0x22;
    // midnight.weekday = 0x03;

    // Enable rtcc writes
    RTCCONbits.RTCWREN = 1;
    if (RTCCONbits.RTCWREN)
    {
        // Set up clock source to MFINTOSC/16
        //
        RTCCONbits.RTCCLKSEL = 0b01;
        // RTCCAL = 0b11111111;
        // rtcc_datetime_set(&midnight);
        RTCCONbits.RTCEN = 1;
    }
    else
    {
        printf("RTCC ERROR\n\r");
    }
    // RTCCONbits.RTCWREN = 0;

    return;
}

#define ONES(x) (x & 0x0F)
#define TENS(x) (x >> 4)
#define ASCII_NUMBER_OFFSET 48

char now_str[9];

char *
rtcc_get_time (void)
{
    datetime_t *now;

    while (RTCSYNC)
    {}
    now = rtcc_datetime_get();

    now_str[0] = TENS(now->hour)+ASCII_NUMBER_OFFSET;
    now_str[1] = ONES(now->hour)+ASCII_NUMBER_OFFSET;
    now_str[2] = ' '; // colon
    now_str[3] = TENS(now->minute)+ASCII_NUMBER_OFFSET;
    now_str[4] = ONES(now->minute)+ASCII_NUMBER_OFFSET;
    now_str[5] = ' ';
    now_str[6] = now->second_tens + ASCII_NUMBER_OFFSET;
    now_str[7] = now->second_ones + ASCII_NUMBER_OFFSET;

    return &now_str[0];
}

datetime_t datetime;

datetime_t *
rtcc_datetime_get (void)
{

    datetime.second = SECONDS;

    datetime.minute = MINUTES;

    datetime.hour = HOURS;

    datetime.day = DAY;

    datetime.month = MONTH;

    datetime.year = YEAR;

    datetime.weekday = WEEKDAY;

    return &datetime;
}

void
rtcc_datetime_set (datetime_t *dt)
{
    SECONDS = dt->second;

    MINUTES = dt->minute;

    HOURS = dt->hour;

    DAY = dt->day;

    MONTH = dt->month;

    YEAR = dt->year;

    WEEKDAY = dt->weekday;
}

void
rtcc_set_time (unsigned char hour, unsigned char minute, unsigned char second)
{
    datetime_t new_time = *rtcc_datetime_get();

    new_time.hour = hour;
    new_time.minute = minute;
    new_time.second = second;

    rtcc_datetime_set(&new_time);
}

void
rtcc_set_default (void)
{

}


/*** EOF ***/
