/** @file adc.h
 * 
 * This driver implements the functionality of the ADC^2 module in the
 * PIC16LF1919x devices.
*/

#include <xc.h>

#include "drivers/adc.h"


void
adc_init (void)
{
    // Configure the ADC clock source to Frc:
    // - CS - Clock Select: 0 - Fosc , 1 - Frc
    // - ADCLK - Prescaler : 0 - Fosc/2
    //
    // We chose to use Frc even though its slow because it allows usage across
    // all clock speeds.
    //
    ADCON0bits.CS = 1;
    // ADCLK = 0x0;

    // Select right-justified conversion results. The 12-bit result is stored
    // in ADRES.
    //
    ADCON0bits.FM = 1;
}

void
adc_channel_set (unsigned char chan)
{
    ADPCH = chan;
}


void
adc_reference_set (unsigned char ref)
{
    ADREFbits.PREF0 = ref & 0x01;
    ADREFbits.PREF1 = (ref >> 1) & 0x01;
}


// EOF //
