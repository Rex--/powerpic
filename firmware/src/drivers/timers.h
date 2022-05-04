
#ifndef TIMERS_H
#define TIMERS_H

// Timer0
//
void        timer0_init (void);
inline void timer0_start (void);
inline void timer0_stop (void);
inline void timer0_set (unsigned short value);
unsigned int timer0_get (void);
inline void timer0_prescaler_set (unsigned char value);
inline void timer0_postscaler_set (unsigned char value);
inline void timer0_interrupt_enable (void);
inline void timer0_interrupt_disable (void);

void timer_test(unsigned short value);
#endif
