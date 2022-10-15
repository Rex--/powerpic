/** @file temperature.c
 * 
 * This library implements reading the internal temperature sensor and
 * converting it to degrees.
*/

#include <xc.h>

#include "drivers/adc.h"
#include "drivers/fvr.h"
#include "drivers/nvm.h"

#define LOG_TAG "lib.temperature"
#include "lib/logging.h"

#include "lib/temperature.h"


static int          temperature_cal_degrees = 0;
static unsigned int temperature_cal_adc = 0;

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
    // long tmp32s = 25 + ((long)(average - 2775L) * 2048L / -15565L);
    long tmp32s = temperature_cal_degrees + ((long)(average - (long)temperature_cal_adc) * 2048L / -15565L);
    int temp_c = (int)tmp32s;
    LOG_DEBUG("Cal@90: %i", temp_c);

    adc_disable();
    fvr_temp_set(FVR_TEMP_OFF);
    fvr_disable();

    return temp_c;
}

void
temperature_calibrate (int degrees)
{
    // To calibrate we take an adc reading of the temperature sensor,
    // and then relate that to a given degrees. We then use this relation in
    // our calculation of actual temp.

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

    // Record the ADC result and given degrees
    temperature_cal_adc = average;
    temperature_cal_degrees = degrees;
}

static void
temperature_adc_config (void)
{
    // Read the calibration value if not set
    //
    if ((0 == temperature_cal_adc) && (0 == temperature_cal_degrees))
    {
        // Read factory calibration stored in DIA.
        temperature_cal_adc = nvm_read(1, 0x0116);
        LOG_DEBUG("90C TSHR: %u", temperature_cal_adc);

        // Set to factory calibration temperature (90C)
        temperature_cal_degrees = 90;
    }

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
