/** @file rtcc.h
 * Driver for the Real Time Clock Calendar Module.
*/

#ifndef _rtcc_h_
#define _rtcc_h_

/**
 * Initialize the RTCC driver.
 * Module must be enabled for the clock to tick.
*/
void    rtcc_init(void);

/**
 * Enable the rtcc module.
 * Driver must be initialized first.
*/
#define rtcc_enable()   (RTCCONbits.RTCEN = 1)

/**
 * Disable the rtcc module.
*/
#define rtcc_disable()   (RTCCONbits.RTCEN = 0)

/**
 * Gets the status of the rtcc.
*/
#define rtcc_status()   (RTCCONbits.RTCEN)

/**
 * Enable rtcc alarm.
 * Writes to the alarm enable bit are only allowed if writes are enabled.
*/
#define rtcc_alarm_enable()             (ALRMCONbits.ALRMEN = 1)

/**
 * Disable rtcc alarm.
 * Writes to the alarm enable bit are only allowed if writes are enabled.
*/
#define rtcc_alarm_disable()            (ALRMCONbits.ALRMEN = 0)

/**
 * Enable alarm interrupts.
*/
#define rtcc_alarm_interrupt_enable()   (PIE8bits.RTCCIE = 1)

/**
 * Disable alarm interrupts.
*/
#define rtcc_alarm_interrupt_disable()  (PIE8bits.RTCCIE = 0)

/**
 * Clear alarm interrupt flag.
*/
#define rtcc_alarm_interrupt_clear()    (PIR8bits.RTCCIF = 0)

/** Enable RTCC writes. */
#define rtcc_writes_enable()    RTCCONbits.RTCWREN = 1

/** Disable RTCC writes. */
#define rtcc_writes_disable()    RTCCONbits.RTCWREN = 0

/** RTCSYNC bit. */
#define rtcc_reads_discouraged()    (RTCCONbits.RTCSYNC)
#define rtcc_writes_discouraged()    (RTCCONbits.RTCSYNC)



/**
 * Set the date of the rtcc.
*/
void    
rtcc_date_set (
    unsigned char year, 
    unsigned char month, 
    unsigned char day, 
    unsigned char weekday
);

/**
 * Get the date of the rtcc.
*/
unsigned char *
rtcc_date_get (void);


/**
 * Set the time of the rtcc.
*/
void    
rtcc_time_set (
    unsigned char hour,
    unsigned char minute,
    unsigned char second
);

/**
 * Get the time of the rtcc.
*/
unsigned char *
rtcc_time_get (void);



#define RTCC_AMASK_SECOND       0x1
#define RTCC_AMASK_SECOND_TEN   0x2
#define RTCC_AMASK_MINUTE       0x3
#define RTCC_AMASK_MINUTE_TEN   0x4
#define RTCC_AMASK_HOUR         0x5
#define RTCC_AMASK_DAY          0x6
#define RTCC_AMASK_WEEK         0x7
#define RTCC_AMASK_MONTH        0x8
#define RTCC_AMASK_YEAR         0x9

/**
 * Set the interval of the alarm.
 * The interval is based on the alarm mask which configures how many digits of
 * the alarm must match.
*/
#define rtcc_alarm_mask_set(mask)   ALRMCONbits.AMASK = (mask & 0x0F)

/**
 * Get the interval of the alarm.
*/
#define rtcc_alarm_mask_get()       (ALRMCONbits.AMASK)



/**
 * Set the date for the alarm to interrupt.
 * The alarm should only be changed while it is disabled.
*/
void
rtcc_alarm_date_set (
    unsigned char month,
    unsigned char day,
    unsigned char weekday
);


/**
 * Set the time for the alarm to interrupt.
 * The alarm should only be changed while it is disabled.
*/
void
rtcc_alarm_time_set (
    unsigned char hour,
    unsigned char minute,
    unsigned char second
);



/**
 * Set the drift calibration value.
*/
#define rtcc_drift_set(drift)   (RTCCAL = (drift))

/**
 * Get the drift calibration value.
*/
#define rtcc_drift_get()        (RTCCAL)


#endif

// EOF //
