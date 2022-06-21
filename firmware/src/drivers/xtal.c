/** @file xtal.c 
 * Clock Configuration C Driver.
*/

#include <xc.h>

#include "drivers/xtal.h"

void
xtal_init (void)
{
    xtal_divider_set(XTAL_DIV_1);

#   if   _XTAL_FREQ ==  1000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_1);

#   elif _XTAL_FREQ ==  2000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_2);

#   elif _XTAL_FREQ ==  4000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_4);

#   elif _XTAL_FREQ ==  8000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_8);

#   elif _XTAL_FREQ == 16000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_16);

#   elif _XTAL_FREQ == 32000000
        xtal_hfintosc_freq_set(XTAL_HFFRQ_32);

#   else
#       error "Incompatible _XTAL_FREQ! Must be: 1, 2, 4, 8, 16, 32 MHz"
#   endif

}
