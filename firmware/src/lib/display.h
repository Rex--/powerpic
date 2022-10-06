/** @file display.h
 * 
 * Display library for PowerOS
 * 
 * This library uses the LCD module in the PIC16LF1919x to drive the casio
 * watch display. It maps ascii characters onto the 7 segment displays based
 * on a customizable font.
 * 
 * The display is split into 5 main groups to hopefully make things easier:
 *  1. Primary      The primary display includes the main 8 7-segment displays.
 *                  This allows a max string length of 8.
 * 
 *  2. Secondary    The secondary display includes 2 7-segment displays that
 *                  normally display the day of week.
 * 
 *  3. Period       The period group includes 8 periods separating the primary
 *                  display characters plus the colon.
 * 
 *  4. Sign         The sign group includes the 4 arithmetic signs.
 * 
 *  5. Misc         The misc group includes the AM/PM indicators, the
 *                  constant(K) indicator, and both alarm indicators.
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_PERIOD_COLON            9

#define DISPLAY_MISC_BELL               1
#define DISPLAY_MISC_WAVE               2
#define DISPLAY_MISC_K                  3
#define DISPLAY_MISC_PM                 4
#define DISPLAY_MISC_AM                 5

#define DISPLAY_SIGN_ADD                1
#define DISPLAY_SIGN_SUBTRACT           2
#define DISPLAY_SIGN_MULTIPLY           3
#define DISPLAY_SIGN_DIVIDE             4



void    display_init (void);
void    display_update  (void);

void    display_set_font                (unsigned char font);

void    display_primary_segments        (signed char position, unsigned char segments);
void    display_primary_character       (signed char position, unsigned char character);
void    display_primary_string          (signed char position, const char *string);
void    display_primary_number          (signed char position, long number);
void    display_primary_hex             (signed char position, unsigned long number);
void    display_primary_clear           (signed char position);

void    display_secondary_segments      (signed char position, unsigned int segments);
void    display_secondary_character     (signed char position, unsigned char character);
void    display_secondary_string        (signed char position, const char *string);
void    display_secondary_number        (signed char position, int number);
void    display_secondary_hex           (signed char position, unsigned char number);
void    display_secondary_clear         (signed char position);

void    display_period                  (signed char position);
void    display_period_clear            (signed char position);

void    display_sign                    (signed char position);
void    display_sign_clear              (signed char position);

void    display_misc                    (signed char position);
void    display_misc_clear              (signed char position);


void display_segments_zero (void);

#endif

// EOF //
