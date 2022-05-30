
#ifndef TICK_H
#define TICK_H


void        tick_enable (void);

void        tick_start (void);
void        tick_stop (void);

void        tick_set_ms (unsigned int rate);

int         tick_ticked (void);

void        tick_reset (void);

void        tick_set_ps(unsigned char scale);

void        tick_enable_interrupts (void);
void        tick_disable_interrupts (void);

#endif