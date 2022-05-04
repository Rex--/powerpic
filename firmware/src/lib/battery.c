/** @file
 * Battery module for the PowerPIC.
 * 
 * This module provides functions to determine the current battery status.
*/

#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#include "drivers/adc.h"
#include "drivers/fvr.h"



#define FIXED_VOLTAGE_REP   2058UL
#define MAX_VOLTAGE         4095UL


/**
 * Enables the battery module.
 * This procedure enables the Fixed Voltage Reference and configures the Analog
 * to Digital Converter to compare Vdd to the fixed voltage. This allows us to
 * do some simple math and derive the current Vdd voltage, which in our
 * application is the battery voltage.
*/
void
battery_enable (void)
{
    adc_init();

    // Set our FVR to output 2.048V to the ADC
    //
    fvr_adc_set(FVR_BUFFER_GAIN_2X);
    adc_channel_select(ADC_CHANNEL_FVR_BUFFER_1);

    // Select Vdd as ADC input channel.
    //
    adc_reference_select(ADC_REFERENCE_VDD);

    // Enable needed drivers
    //
    FVR_ENABLE; // This is already enable by driver_manager.
    ADC_ENABLE;
    return;
}   /* battery_enable() */

/**
 * Disables the battery module.
 * This should be called after you are done using the battery module to prevent
 * unneeded power usage.
*/
void
battery_disable (void)
{
    // Disable the FVR ADC output
    //
    fvr_adc_set(FVR_BUFFER_OFF);

    // Disable drivers
    //
    ADC_DISABLE;
    FVR_DISABLE;

    return;
}   /* battery_disable() */

/**
 * Reads the current voltage from the battery.
 * The battery module should already have been enabled.
 * 
 * @returns
 * Integer representing the battery voltage.
*/
unsigned int
battery_read_voltage (void)
{
    uint16_t average = 0;
    uint16_t voltage = 0;

    for (int res = 0; res < 10; res++)
    {
        ADC_CONVERSION_START;

        while (1 == ADC_CONVERSION_STATUS)
        {
            // Wait until conversion is complete
        }
        
        average = (average + (uint16_t)ADC_RESULT) >> 1;
    }

    // Calculate voltage
    //
    voltage = (uint16_t)((FIXED_VOLTAGE_REP * MAX_VOLTAGE) / average);

    // printf("[lib]battery: %i\n\r", voltage);

    return voltage;

}   /* battery_read_voltage() */

/*** EOF ***/
