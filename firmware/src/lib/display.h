/** @file display.h
 * 
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



void    display_enable (void);
void    display_update  (void);

void    display_primary_character       (signed char position, unsigned char character);
void    display_primary_string          (signed char position, const char *string);
void    display_primary_number          (signed char position, long number);
void    display_primary_hex             (signed char position, long number);
void    display_primary_clear           (signed char position);

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

#endif

/*** EOF ***/
