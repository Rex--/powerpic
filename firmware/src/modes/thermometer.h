/** @file thermometer.h
 * 
 * This mode implements a thermometer that uses the internal temperature sensor.
 * Due to this, it may not be very accurate for the ambient temp.
*/

#ifndef _thermometer_h_
#define _thermometer_h_

void            thermometer_init  (void);
void            thermometer_start (void);
signed char     thermometer_run   (unsigned int event);
void            thermometer_stop  (void);

mode_app_t thermometer_mode = {
        "thermometer",
        &thermometer_init,
        &thermometer_start,
        &thermometer_run,
        &thermometer_stop,
};

#endif

// EOF //
