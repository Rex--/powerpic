/** @file battery.c
 * 
 * This library implements functionality relating to the battery in the watch.
 * 
 * Currently, the voltage returned is within +-0.02 of the actual voltage, at
 * room temp. Further testing needs to be done across temperature ranges (Like
 * with wearing).
 * The ADC and FVR is only enabled when sampling. This might present issues by
 * introducing delays in the read_voltage() call, so plenty of time should be
 * given.
*/

#include <xc.h>

#include "drivers/adc.h"
#include "drivers/fvr.h"

#include "lib/battery.h"

#define LOG_TAG "lib.battery"
#include "lib/logging.h"


#define FIXED_VOLTAGE_REP   1024UL    // Should we calculate this based on temp?
#define MAX_VOLTAGE         4095UL    // Max ADC result.

// Configure ADC for battery use.
static void battery_adc_config (void);

void
battery_init (void)
{
    LOG_INFO("Initializing battery...");

    // Initialize ADC driver.
    adc_init();
}

float
battery_read_voltage (void)
{
    unsigned int average = 0;
    unsigned int voltage = 0;

    // Configure and enable needed modules and wait for them to start up.
    //
    battery_adc_config();
    fvr_enable();
    adc_enable();
    __delay_ms(1);

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

    // Turn off used peripherals
    adc_disable();
    fvr_disable();

    // Calculate voltage from average reading.
    //
    voltage = (unsigned int)((FIXED_VOLTAGE_REP * MAX_VOLTAGE) / average);

    LOG_DEBUG("Voltage: %u", voltage);

    return ((float)voltage / 1000);
}

static void
battery_adc_config (void)
{
    // Set our FVR to output 1.024V to the ADC, and our ADC channel to the FVR
    // buffer.
    //
    fvr_adc_set(FVR_GAIN_1X);
    adc_channel_set(ADC_CHAN_FVR_BUFFER_1);

    // Select Vdd as ADC reference.
    //
    adc_reference_set(ADC_REF_VDD);
}

// EOF //
