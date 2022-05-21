
#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS_ID     "SET"

void    settings_init      (mode_config_t *cfg);
void    settings_start     (mode_config_t *cfg);
void    settings_thread    (mode_config_t *cfg, unsigned int event);
void    settings_stop      (mode_config_t *cfg);

#define SETTINGS_MODE   \
    SETTINGS_ID,        \
    settings_init,      \
    settings_start,     \
    settings_thread,    \
    settings_stop

#endif
