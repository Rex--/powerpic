/** @file rtcc.h
 * Driver for the Real Time Clock Calendar Module.
*/

#include <xc.h>

#include "drivers/rtcc.h"
void
rtcc_init (void)
{
    rtcc_writes_enable();

#   if (1 == PCB_REV)
    // Set up clock source to MFINTOSC/16 at 31.25kHz
    RTCCONbits.RTCCLKSEL = 0b01;

#   elif (2 == PCB_REV)
    // Set up clock source to SOSC at 32.768kHz
    RTCCONbits.RTCCLKSEL = 0b00;
#   endif

    // Enable RTCC
    while (0 == rtcc_status())
    {
        rtcc_enable();
    }

    rtcc_writes_disable();
}

void    
rtcc_date_set (unsigned char year, unsigned char month, 
                unsigned char day, unsigned char weekday)
{
    rtcc_writes_enable();
    
    while (rtcc_writes_discouraged())
    {
        // Wait for sync bit
    }

    WEEKDAY = weekday;
    DAY = day;
    MONTH = month;
    YEAR = year;

    rtcc_writes_disable();
}

unsigned char *
rtcc_date_get (void)
{
    static unsigned char date_now[4];

    while(rtcc_reads_discouraged())
    {
        // Wait for the OK to read the regisrs.
    }

    date_now[0] = YEAR;
    date_now[1] = MONTH;
    date_now[2] = DAY;
    date_now[3] = WEEKDAY;

    return &date_now[0];
}


void
rtcc_time_set (unsigned char hour, unsigned char minute, unsigned char second)
{
    rtcc_writes_enable();
    
    while (rtcc_writes_discouraged())
    {
        // Wait for sync bit
    }

    SECONDS = second;
    MINUTES = minute;
    HOURS = hour;

    rtcc_writes_disable();
}

unsigned char *
rtcc_time_get (void)
{
    static unsigned char time_now[3];

    while(rtcc_reads_discouraged())
    {
        // Wait for the OK to read the registers
    }

    time_now[0] = HOURS;
    time_now[1] = MINUTES;
    time_now[2] = SECONDS;

    return &time_now[0];
}


// ALARM
//

void
rtcc_alarm_date_set (unsigned char month, unsigned char day, unsigned char weekday)
{
    ALRMWD = weekday;
    ALRMDAY = day;
    ALRMMTH = month;
}

void
rtcc_alarm_time_set (unsigned char hour, unsigned char minute, unsigned char second)
{
    // *NOTE: if a 0 is in the second ones place, the alarm will not
    // occur, according to Microchip's Errata for the PIC16LF1919x.
    // The given workaround is to use a SECONDS value of 0x0A for alarms
    // to occur at 0 seconds. I found it easier to just increment it by 1.
    if (0 == (second & 0xF))
    {
        second++;
    }

    ALRMSEC = second;
    ALRMMIN = minute;
    ALRMHR = hour;
}

// EOF //
