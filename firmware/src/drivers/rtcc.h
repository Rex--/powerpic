/** @file
 * RTCC Driver Library for PIC16LF1919x devices using the hardware RTCC module.
 * 
*/

#ifndef RTCC_H
#define RTCC_H

void rtcc_init (void);

char * rtcc_get_time(void);

void rtcc_set_time (unsigned char hour, unsigned char minute, unsigned char second);

#endif

/*** EOF ***/
