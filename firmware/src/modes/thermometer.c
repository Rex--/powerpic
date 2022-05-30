
#include <xc.h>
#include <stdio.h>


#include "drivers/fvr.h"
#include "drivers/adc.h"

#include "lib/mode.h"
#include "lib/display.h"

#include "modes/thermometer.h"



void
thermometer_init (mode_config_t *cfg)
{
    cfg->tickrate = 10000;
    cfg->keymap = 0;
}


void
thermometer_start (mode_config_t *cfg)
{
    display_primary_string(1, "temp--*F");
    fvr_tim_power(FVR_TEMPERATURE_HIGH);
    // FVRCONbits.TSEN = 1;
    // FVRCONbits.TSRNG = 1;

    // Setup the 2v FVR we need
    fvr_adc_set(FVR_BUFFER_GAIN_2X);
    FVR_ENABLE;
    
    // init adc
    adc_init();

    adc_channel_select(ADC_CHANNEL_TEMP_SENSOR);
    adc_reference_select(ADC_REFERENCE_FVR);

    ADC_ENABLE;

    // Wait a second to everything power up
    //
    cfg->tickrate = 1000;
}


void
thermometer_thread (mode_config_t *cfg, unsigned int event)
{
    // tick events
    if ((event & 0xFF) == 0x01)
    {
        ADC_CONVERSION_START;

        while (ADC_CONVERSION_STATUS)
        {

        }

        int temp = ADC_RESULT;

        temp = (temp * 0.263 + 50) * 10;

#       if DEV_BUILD
        printf("Temp: %i\n\r", temp);
#       endif

        // display_primary_clear(0);
        display_primary_string(1, "temp");
        display_primary_number(0, temp);
        display_primary_string(-1, "*F");

        if (cfg->tickrate != 60000)
        {
            cfg->tickrate = 60000;
        }
    }
    else
    {
    }
}

void
thermometer_stop (mode_config_t *cfg)
{
    ADC_DISABLE;
    fvr_tim_power(FVR_TEMPERATURE_OFF);
    fvr_adc_set(FVR_BUFFER_OFF);
}

