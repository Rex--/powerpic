/** @file
 * ADC Driver for PIC16LF1919x devices using the ADC^2 Module.
 * 
 * The Analog to Digital Converter with Comptutation Module.
*/

#include <xc.h>

#include "drivers/adc.h"


/**
 * Initialize the ADC module.
 * 
 * This configures the ADC clock and result justification.
 * 
*/
void
adc_init (void)
{
    // Configure the ADC clock source to 1MHz / 2 :
    // - CS - Clock Select: 0 - Fosc , 1 - Frc
    // - ADCLK - Prescaler : 0 - Fosc/2
    //
    ADCON0bits.CS = 0;
    ADCLK = 0x0;

    // Select right-justified data results in ADRES and ADPREV.
    //
    ADCON0bits.FM = 1;

    // Double sample enable bit. First conversion appears in prev.
    //
    // ADCON1bits.DSEN = 1;

    return;
}   /* adc_init() */

/**
 * Selects the the input channel to the ADC.
 * 
 * @param[in] channel The ADC channel you wish to sample.
 * 
*/
void
adc_channel_select (uint8_t channel)
{
    ADPCH = channel;

    return;
}   /* adc_channel_select() */

/**
 * Selects the positive voltage reference for the ADC.
 * 
 * @param[in] reference The reference voltage you wish to use.
 * 
*/
void
adc_reference_select (uint8_t reference)
{
    ADREFbits.PREF0 = reference & 1U;
    ADREFbits.PREF1 = (reference >> 1) & 1U;

    return;
}   /* adc_reference_select() */


/*** EOF ***/
