/** @file datetime.h
 * Date and Time library for PowerOS.
 * All values are in BCD format.
*/

#ifndef _datetime_h_
#define _datetime_h_


typedef struct {
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char weekday;
} date_t;

typedef struct {
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} time_t;

typedef struct {
    time_t time;
    date_t date;
} datetime_t;

/**
 * Initialize the datetime library.
 * This starts the RTCC module.
*/
void datetime_init (void);

/**
 * Sets the date and time.
 * 
 * @param[in]   datetime    A pointer to a datetime object that holds the
 *                          desired date and time value in BCD format.
*/
void
datetime_set (datetime_t *datetime);

/**
 * Gets the current date and time.
 * 
 * @param[out]  datetime    A pointer to a datetime object that will hold the
 *                          desired date and time value.
*/
void
datetime_now (datetime_t *datetime);

/**
 * Sets the time.
 * 
 * @param[in]   time        A pointer to a time object that holds the desired
 *                          time value.
*/
void
datetime_time_set (time_t *time);

/**
 * Gets the current time.
 * 
 * @param[out]  time        A pointer to a time object that will hold the
 *                          desired time value.
*/
void
datetime_time_now (time_t *time);


// Below are some helper functions to deal with date and time values.
// All time and date values are in BCD format.
// Weekday is encoded as values 0-6 with 0 being Sunday and 6 being Saturday.
//

/**
 * Decode binary coded decimal byte to its decimal value.
 * 
 * @param[in]   val     The binary coded decimal value to convert.
 * 
 * @returns     Decimal value.
*/
#define BCD2DEC(val)    ((((val) >> 4) * 10) + ((val) & 0x0F))

/**
 * Get a short string representation of a weekday value.
 * 
 * The string consists of 2 ASCII characters representing the weekday followed
 * by a NULL byte. (SU, MO, TU, WE, TH, FR, SA)
 * 
 * @param[in]   weekday     A weekday value 0-6.
 * 
 * @returns     A pointer to a 3-byte long string.
*/
const char *
datetime_weekday_str (unsigned char weekday);


#endif

// EOF //
