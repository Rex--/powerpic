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

#include "lib/battery.h"

static void temperature_adc_config (void);

float
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

    // 100 Samples
    //
    for (int res = 0; res < 100; res++)
    {
        adc_sample();
        
        while (adc_status())
        {
            // Wait until conversion is complete
        }

        average = (average + (unsigned int)adc_result()) / 2;
    }

    LOG_INFO("Result: %u", average);

    adc_disable();
    fvr_temp_set(FVR_TEMP_OFF);
    fvr_disable();

    return 0.0;
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
