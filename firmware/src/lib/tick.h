
#ifndef TICK_H
#define TICK_H


void        tick_enable (void);

void        tick_set_ms (unsigned int rate);

int         tick_ticked (void);

void        tick_reset (void);

void tick_set_ps(unsigned char scale);

#endif