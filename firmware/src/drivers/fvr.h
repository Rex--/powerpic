/** @file
 * Fixed Voltage Reference Driver Library for PIC16LF1919x Devices.
 * The FVR driver must be initialized before use of the FVR for other
 * peripherials. After initialization, each peripherial is expected to
 * configure their respective `FVRCONbits`.
*/

#ifndef FVR_H
#define FVR_H

/**
 * Enable the fixed voltage reference. 
*/
#define FVR_ENABLE              (FVRCONbits.FVREN = 1)

/**
 * Disable the fixed voltage reference. 
*/
#define FVR_DISABLE             (FVRCONbits.FVREN = 0)

#define FVR_BUFFER_OFF          0x00 /**< Fixed voltage is disabled */
#define FVR_BUFFER_GAIN_1X      0x01 /**< Fixed voltage gain of 1x */
#define FVR_BUFFER_GAIN_2X      0x02 /**< Fixed voltage gain of 2x */
#define FVR_BUFFER_GAIN_4X      0x03 /**< Fixed voltage gain of 4x */

#define FVR_TEMPERATURE_OFF     0x00
#define FVR_TEMPERATURE_LOW     0x02
#define FVR_TEMPERATURE_HIGH    0x03


void        fvr_init (void);

void        fvr_adc_set (uint8_t gain);

void        fvr_cda_set (uint8_t gain);

void        fvr_temperature_sensor_set (uint8_t temperature_settings);

#endif

/*** EOF ***/
