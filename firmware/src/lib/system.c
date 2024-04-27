/** @file system.
 * 
 * System library for CasiOS.
*/

#include <xc.h>

#include "drivers/xtal.h"
#include "drivers/pins.h"
#include "drivers/interrupts.h"

#include "lib/system.h"

void system_init (void)
{
    // Initialize some base system drivers.
    //
    xtal_init();
    pins_init();

    // Enable interrupts
    //
    interrupts_peripherial_enable();
    interrupts_global_enable();
}


// EOF //
