/** @file battery.c
 * 
 * This library implements functionality relating to the battery in the watch.
 * 
 * Currently, basic functionality is implemented, but power and other ADC uses
 * are not considered. The library should configure the ADC for each reading if
 * needed, and only enable the ADC and FVR modules when actively sampling.
*/

#include <xc.h>

#include "drivers/adc.h"
#include "drivers/fvr.h"

#include "lib/battery.h"

#define LOG_TAG "lib.battery"
#include "lib/logging.h"


#define FIXED_VOLTAGE_REP   1024UL    // Should we calculate this base on temp?
#define MAX_VOLTAGE         4095UL    // Max ADC result.

void
battery_init (void)
{
    LOG_INFO("Initializing battery...");
    adc_init();

    // Set our FVR to output 2.048V to the ADC, and our ADC channel to the FVR
    // buffer.
    //
    fvr_adc_set(FVR_GAIN_1X);
    adc_channel_set(ADC_CHAN_FVR_BUFFER_1);

    // Select Vdd as ADC reference.
    //
    adc_reference_set(ADC_REF_VDD);

    // Enable needed modules
    //
    fvr_enable();
    adc_enable();
}

float
battery_read_voltage (void)
{
    unsigned int average = 0;
    unsigned int voltage = 0;

    // We take 10+1 samples to get a nice rolling average reading.
    // 
    
    // Initial reading.
    //
    adc_sample();
    while (1 == adc_status())
    {
        // Wait until conversion is complete
    }
    average = (unsigned int)adc_result();

    // 10 Samples
    //
    for (int res = 0; res < 10; res++)
    {
        adc_sample();
        
        while (1 == adc_status())
        {
            // Wait until conversion is complete
        }

        average = (average + (unsigned int)adc_result()) / 2;
    }

    // Calculate voltage from average reading.
    //
    voltage = (unsigned int)((FIXED_VOLTAGE_REP * MAX_VOLTAGE) / average);

    LOG_DEBUG("Voltage: %u", voltage);

    return ((float)voltage / 1000);
}

// EOF //
