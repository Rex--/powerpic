/** @file pins.h
 * I/O Pin Driver Library for PIC16LF1919x Devices.
 * 
*/

#ifndef PINS_H
#define PINS_H

#define PIN0        0
#define PIN1        1
#define PIN2        2
#define PIN3        3
#define PIN4        4
#define PIN5        5
#define PIN6        6
#define PIN7        7

#define pin_mode_input2(port, pin)   (port |= (1<< pin))

inline void         pin_mode_input  (volatile unsigned char port, const unsigned char pin);
inline void         pin_mode_output (volatile unsigned char port, const unsigned char pin);

#endif

/*** EOF ***/
