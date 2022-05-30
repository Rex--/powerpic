/** @file
 * RTCC Driver Library for PIC16LF1919x devices using the hardware RTCC module.
 * 
*/

#ifndef RTCC_H
#define RTCC_H

#define RTCC_ALARM_SECOND       0x1
#define RTCC_ALARM_SECOND_TEN   0x2
#define RTCC_ALARM_MINUTE       0x3
#define RTCC_ALARM_MINUTE_TEN   0x4
#define RTCC_ALARM_HOUR         0x5
#define RTCC_ALARM_DAY          0x6
#define RTCC_ALARM_WEEK         0x7
#define RTCC_ALARM_MONTH        0x8
#define RTCC_ALARM_YEAR         0x9

void rtcc_init (void);

char * rtcc_get_time(void);

void rtcc_set_time (unsigned char hour, unsigned char minute, unsigned char second);

void rtcc_set_alarm (unsigned char minute, unsigned char second, unsigned char mask);

#endif

/*** EOF ***/
