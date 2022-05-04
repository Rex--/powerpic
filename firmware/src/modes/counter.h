
#ifndef COUNTER_H
#define COUNTER_H


void    counter_init (mode_config_t *cfg);
void    counter_start (mode_config_t *cfg);
void    counter_thread (mode_config_t *cfg, unsigned int event);

#define COUNTER_MODE        \
            "CNT",          \
            counter_init,           \
            counter_start,  \
            counter_thread, \
            NULL

#endif

/*** EOF ***/
