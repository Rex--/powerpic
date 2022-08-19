/** @file temperature.c
 * 
 * This library implements reading the internal temperature sensor and
 * converting it to degrees.
*/

#include <xc.h>

#include "drivers/adc.h"
#include "drivers/fvr.h"

#define LOG_TAG "lib.temperature"
#include "lib/logging.h"

#include "lib/temperature.h"

static void temperature_adc_config (void);

int
temperature_read (void)
{
    // Configure ADC for temp sensor use.
    temperature_adc_config();

    // Take a reading
    adc_sample();
    while (adc_status())
    {
        // Wait until conversion is complete
    }
    unsigned int average = (unsigned int)adc_result();

    // 10 Samples
    //
    for (int res = 0; res < 10; res++)
    {
        adc_sample();
        
        while (adc_status())
        {
            // Wait until conversion is complete
        }

        average = (average + (unsigned int)adc_result()) / 2;
    }
    LOG_DEBUG("Raw: %u", average);

    // Calculate temperature based on a calibration reading at 25C. This gives
    // a good range for the middle of the temperature range.
    // This is not very accurate.
    // NOTE: This formula was borrowed from Microchip Forum user mbrowning.(1)
    //     Thanks for your help.
    // (1) https://www.microchip.com/forums/m1214187.aspx
    //
    long tmp32s = 25 + ((long)(average - 2775L) * 2048L / -15565L);
    int temp_c_25c = (int)tmp32s;
    LOG_DEBUG("Cal@25: %i", temp_c_25c);

    adc_disable();
    fvr_temp_set(FVR_TEMP_OFF);
    fvr_disable();

    return temp_c_25c;
}

static void
temperature_adc_config (void)
{

    // Set FVR to output 2.048V to the ADC, and FVR as ADC reference.
    //
    fvr_adc_set(FVR_GAIN_2X);
    adc_reference_set(ADC_REF_FVR);

    // Set our ADC channel to the temperature sensor.
    //
    adc_channel_set(ADC_CHAN_TEMP_SENSOR);

    // Enable TS, FVR, and ADC.
    //
    fvr_enable();
    fvr_temp_set(FVR_TEMP_HIGH);
    adc_enable();

    // Wait for ADC to settle after configuring it to the temp sensor.
    // (Minimum 25us)
    __delay_us(50);
}

// EOF //
