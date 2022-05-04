
#ifndef POWER_H
#define POWER_H

#define POWER_ID "PWR"

void    power_init      (mode_config_t *cfg);
void    power_start     (mode_config_t *cfg);
void    power_thread    (mode_config_t *cfg, unsigned int event);
void    power_adj       (mode_config_t *cfg, unsigned int event);
void    power_stop      (mode_config_t *cfg);

#define POWER_MODE  \
    POWER_ID,   \
    power_init, \
    power_start, \
    power_thread, \
    power_stop

#endif
