/** @file lcd.h
 * LCD Driver Library for PIC16LF1919x devices using the hardware LCD driver.
*/

#ifndef LCD_H
#define LCD_H


#define LCD_PRIMARY                          1
#define LCD_PRIMARY_CHARACTERS               8
#define LCD_PRIMARY_CHARACTER_SEGMENTS       7

#define LCD_SECONDARY                        2
#define LCD_SECONDARY_CHARACTERS             2
#define LCD_SECONDARY_CHARACTER_SEGMENTS    10

#define LCD_PERIOD                           3
#define LCD_PERIOD_SEGMENTS                  9

#define LCD_SIGN                             4
#define LCD_SIGN_SEGMENTS                    4

#define LCD_MISC                             5
#define LCD_MISC_SEGMENTS                    5


void        lcd_init            (void);
void        lcd_update          (void);

void        lcd_primary_draw    (unsigned char position, unsigned char segment);
void        lcd_primary_clear   (unsigned char position, unsigned char segment);

void        lcd_secondary_draw  (unsigned char position, unsigned char segment);
void        lcd_secondary_clear (unsigned char position, unsigned char segment);

void        lcd_period_draw     (unsigned char segment);
void        lcd_period_clear    (unsigned char segment);

void        lcd_sign_draw       (unsigned char segment);
void        lcd_sign_clear      (unsigned char segment);

void        lcd_misc_draw       (unsigned char segment);
void        lcd_misc_clear      (unsigned char segment);


void        lcd_segments_clear (void);
void        lcd_segments_set (void);

#endif

/*** EOF ***/
