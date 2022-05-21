/** @file
 * Fixed Voltage Reference Driver for PIC16LF1919x Devices.
 * The FVR driver must be initialized before use of the FVR for other
 * peripherials. After initialization, each peripherial is expected to
 * configure their respective `FVRCONbits`.
*/

#include <xc.h>

#include "drivers/fvr.h"



/**
 * Enables the FVR for use by peripherials.
 * 
 * This is a stub function that is provided to keep a common interface amoung
 * drivers. All it does currently is call the macro FVR_ENABLE.
*/
void
fvr_init (void)
{
    FVR_ENABLE;
}


/**
 * Configures the ADC Buffer Gain.
 * 
 * Pass in one of the pre-defined macros or construct a byte with the two LSbs
 * corresponding to the ADFVR bits in the FVRCON register.
 * 
 * @param[in] gain The buffer gain for the FVR output to the ADC.
 * 
*/
void
fvr_adc_set (uint8_t gain)
{
    FVRCONbits.ADFVR0 = gain & 1U;
    FVRCONbits.ADFVR1 = (gain >> 1) & 1U;
}

/**
 * Configures the Comparator Buffer Gain.
 * 
 * Pass in one of the pre-defined macros or construct a byte with the two LSbs
 * corresponding to the CDAFVR bits in the FVRCON register.
 * 
 * @param[in] gain The buffer gain for the FVR output to the Comparator.
 * 
*/
void
fvr_cda_set (uint8_t gain)
{
    FVRCONbits.CDAFVR0 = gain & 1U;
    FVRCONbits.CDAFVR1 = (gain >> 1) & 1U;
}


/**
 * Configures the Temperature Indicator.
 * 
 * Pass in one of the pre-defined macros or construct a byte with the two LSbs
 * corresponding to the TSEN and TSRNG bits respectively in the FVRCON register.
 * 
 * NOTE: This feels like it'd be better as some macros, but this function
 * keeps consistency across the FVR driver interface.
 * 
 * @param[in] range Temperature settings.
 * 
*/
void
fvr_tim_power (unsigned char range)
{
    switch (range)
    {
    case FVR_TEMPERATURE_LOW:
        FVRCONbits.TSEN = 1;
        FVRCONbits.TSRNG = 0;
    break;

    case FVR_TEMPERATURE_HIGH:
        FVRCONbits.TSEN = 1;
        FVRCONbits.TSRNG = 1;
    break;
    
    default:
        FVRCONbits.TSEN = 0;
    break;
    }
}


/*** EOF ***/
