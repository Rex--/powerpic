/** @file adc.h
 * 
 * This driver implements the functionality of the ADC^2 module in the
 * PIC16LF1919x devices.
 * 
*/

#ifndef _adc_h_
#define _adc_h_

// ADC Configuration options
//
#define ADC_CHAN_FVR_BUFFER_1       0x3E    /**< FVR Buffer 1 Channel */
#define ADC_CHAN_FVR_BUFFER_2       0x3F    /**< FVR Buffer 2 Channel */
#define ADC_CHAN_TEMP_SENSOR        0x3C    /**< Temperature Sensor Channel */

#define ADC_REF_VDD                 0x00    /**< Vdd as reference voltage */
#define ADC_REF_FVR                 0x03    /**< FVR as reference voltage */

/**
 * Initialize the adc driver.
*/
void
adc_init (void);

/**
 * Enable the ADC module.
 * This must be called before sampling.
*/
#define adc_enable()    (ADCON0bits.ON = 1)

/**
 * Disable the ADC module.
 * This should be called after a sample is taken to prevent excessive power use.
*/
#define adc_disable()    (ADCON0bits.ON = 0)

/**
 * Start an analog to digital conversion.
*/
#define adc_sample()    (ADCON0bits.GO = 1)

/**
 * Status of an analog to digital conversion.
 * 
 * @returns     ADC status bit.
 * - 1 - ADC Conversion currently ongoing.
 * - 0 - ADC Conversion complete.
*/
#define adc_status()    (ADCON0bits.GO)

/**
 * Result of analog to digital conversion.
 * 
 * @returns
 * 12-bit wide result aligned to the LSb.
*/
#define adc_result()     ((ADRESH << 8) | ADRESL)

/**
 * Select the ADC channel to sample.
 * 
 * Channel macros are implemented as ADC_CHAN_*.
 * 
 * @param[in]   chan    ADC channel.
*/
void
adc_channel_set (unsigned char chan);

/**
 * Select the ADC reference to use.
 * 
 * @param[in]   ref     ADC reference.
*/
void
adc_reference_set (unsigned char ref);

#endif

// EOF //
