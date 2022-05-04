/** @file
 * ADC Driver Library for PIC16LF1919x devices using the ADC^2 Module.
 * 
 * The Analog to Digital Converter with Comptutation Module.
*/

#ifndef ADC_H
#define ADC_H

#define ADC_CHANNEL_FVR_BUFFER_1    0x3E    /**< FVR Buffer 1 Channel */
#define ADC_CHANNEL_FVR_BUFFER_2    0x3F    /**< FVR Buffer 2 Channel */
#define ADC_CHANNEL_TEMP_SENSOR     0x3C    /**< Temperature Sensor Channel */

#define ADC_REFERENCE_VDD       0x00    /**< Vdd as reference voltage */
#define ADC_REFERENCE_FVR       0x03    /**< FVR as reference voltage */

/**
 * Enable the analog to digital converter.
*/
#define ADC_ENABLE              (ADCON0bits.ON = 1)

/**
 * Disable the analog to digital converter.
*/
#define ADC_DISABLE              (ADCON0bits.ON = 0)

/**
 * Start an analog to digital conversion.
*/
#define ADC_CONVERSION_START    (ADCON0bits.GO = 1)

/**
 * Status of an analog to digital conversion.
 * 
 * @returns
 * - 1 - ADC Conversion currently ongoing.
 * - 0 - ADC Conversion complete.
*/
#define ADC_CONVERSION_STATUS   (ADCON0bits.GO)

/**
 * Result of analog to digital conversion.
 * 
 * @returns
 * 12-bit wide result aligned to the LSb.
*/
#define ADC_RESULT              ((ADRESH << 8) | ADRESL)

/**
 * Previous result of analog to digital conversion.
 * 
 * This is automatically stored by the hardware.
 * 
 * @returns
 * 12-bit wide result aligned to the LSb.
*/
#define ADC_PREVIOUS              ((ADPREVH << 8) | ADPREVL)

void        adc_init (void);

void        adc_channel_select (uint8_t channel);

void        adc_reference_select (uint8_t reference);

#endif

/*** EOF ***/