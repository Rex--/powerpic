/** @file fvr.h
 * 
 * Fixed Voltage Reference Driver Library for PIC16LF1919x Devices.
 * 
 * The FVR driver must be initialized before use of the FVR for other
 * peripherals. After initialization, each peripheral is expected to
 * configure their respective `FVRCONbits`.
*/

#ifndef _fvr_h_
#define _fvr_h_


/**
 * Initialize the FVR driver.
*/
//void
//fvr_init (void);  // NOTE: No initialization needed on PIC16(L)F1919x devices.

/**
 * Enable the fixed voltage reference. 
*/
#define fvr_enable()        (FVRCONbits.FVREN = 1)

/**
 * Disable the fixed voltage reference. 
*/
#define fvr_disable()       (FVRCONbits.FVREN = 0)


// Predefined macros for the various gain ampliifier values.
//
#define FVR_GAIN_OFF        0x00    /**< Fixed voltage is disabled */
#define FVR_GAIN_1X         0x01    /**< Fixed voltage gain of 1x */
#define FVR_GAIN_2X         0x02    /**< Fixed voltage gain of 2x */
#define FVR_GAIN_4X         0x03    /**< Fixed voltage gain of 4x */

// Predefined macros for the temperature indicator power modes.
//
#define FVR_TEMP_OFF        0b00    /**< Temperature indicator power off. */
#define FVR_TEMP_LOW        0b10    /**< Temperature indicator low power. */
#define FVR_TEMP_HIGH       0b11    /**< Temperature indicator high power. */

/**
 * Set the voltage gain amplifier for the ADC output (Buffer 1).
*/
#define fvr_adc_set(gain)       (FVRCONbits.ADFVR = ((gain) & 0b11))

/**
 * Set the voltage gain amplifier for the CDA output (Buffer 2).
*/
#define fvr_cda_set(gain)       (FVRCONbits.CDAFVR = ((gain) & 0b11))

/**
 * Set the power mode of the temperature indicator module.
*/
#define fvr_temp_set(power)     (FVRCON = ((FVRCON & 0b11001111) | (power << 4)))


#endif

// EOF //
