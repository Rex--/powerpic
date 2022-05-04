#ifndef CLOCK_H
#define CLOCK_H

void clock_init (mode_config_t *cfg);
void clock_start (mode_config_t *cfg);
void clock_thread (mode_config_t *cfg, unsigned int event);
void clock_stop (mode_config_t *cfg);


#define CLOCK_MODE \
            "COC", \
            clock_init, \
            clock_start, \
            clock_thread, \
            clock_stop

#endif
