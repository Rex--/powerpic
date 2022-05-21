#ifndef THERMOMETER_H
#define THERMOMETER_H

void    thermometer_init      (mode_config_t *cfg);
void    thermometer_start     (mode_config_t *cfg);
void    thermometer_thread    (mode_config_t *cfg, unsigned int event);
void    thermometer_stop      (mode_config_t *cfg);

#define THERMOMETER_MODE  \
    "TEMP",   \
    thermometer_init, \
    thermometer_start, \
    thermometer_thread, \
    thermometer_stop


#endif
