/** @file lcd.h
 * LCD Driver for PIC16LF1919x devices using the hardware LCD controller.
 * Provides a buffer to work in that is the size of the lcd registers.
 * Currently:
 * - 1/4 MUX
 * - 1/3 bias
 * - 3.78v drive
 * - 35Hz frame frequency
*/

#ifndef _lcd_h_
#define _lcd_h_


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

/**
 * Initialize the LCD driver.
 *
 * No configuration needed, as we aren't trying to drive more than one
 * specific LCD. The lcd driver will be ready to enable after this.
*/
void        lcd_init            (void);

/**
 * Draw working buffer to screen.
 * 
 * This function should be called after drawing to any segments to update the
 * LCDDATA registers.
*/
void        lcd_update          (void);

/**
 * Draws a segment to the primary display.
 * This function does not directly interact with the LCDDATA registers. To
 * actually draw the segment on the screen, lcd_update() must be called at any
 * point after this.
 * 
 * @param[in] position  The position of the character on screen.
 * @param[in] segment   The segment to draw.
*/
void        lcd_primary_draw    (unsigned char position, unsigned char segment);
/**
 * Clears a segment from the primary display.
 * 
 * @param[in] position  The position of the character on screen.
 * @param[in] segment   The segment to clear.
*/
void        lcd_primary_clear   (unsigned char position, unsigned char segment);

/**
 * Draws a segment to the secondary display.
 * This function does not directly interact with the LCDDATA registers. To
 * actually draw the segment on the screen, lcd_update() must be called at any
 * point after this.
 * 
 * @param[in] position  The position of the character on screen.
 * @param[in] segment   The segment to draw.
*/
void        lcd_secondary_draw  (unsigned char position, unsigned char segment);
/**
 * Clears a segment from the secondary display.
 * 
 * @param[in] position  The position of the character on screen.
 * @param[in] segment   The segment to clear.
*/
void        lcd_secondary_clear (unsigned char position, unsigned char segment);

/**
 * Draw period segment.
 * This function does not directly interact with the LCDDATA registers. To
 * actually draw the segment on the screen, lcd_update() must be called at any
 * point after this.
 * 
 * @param[in] segment   The segment to draw.
*/
void        lcd_period_draw     (unsigned char segment);
/**
 * Clear a period segment.
 * 
 * @param[in] segment   The segment to clear.
*/
void        lcd_period_clear    (unsigned char segment);

/**
 * Draw a sign segment.
 * This function does not directly interact with the LCDDATA registers. To
 * actually draw the segment on the screen, lcd_update() must be called at any
 * point after this.
 * 
 * @param[in] segment   The segment to draw.
*/
void        lcd_sign_draw       (unsigned char segment);
/**
 * Clear a sign segment.
 * 
 * @param[in] segment   The segment to clear.
*/
void        lcd_sign_clear      (unsigned char segment);

/**
 * Draw a misc segment.
 * This function does not directly interact with the LCDDATA registers. To
 * actually draw the segment on the screen, lcd_update() must be called at any
 * point after this.
 * 
 * @param[in] segment   The segment to draw.
*/
void        lcd_misc_draw       (unsigned char segment);
/**
 * Clear a misc segment.
 * 
 * @param[in] segment   The segment to clear.
*/
void        lcd_misc_clear      (unsigned char segment);

/**
 * Clears all segments.
 * This clears ALL segments on COM0-3, including unused segments. This is
 * handy for initialization, but another procedure should probably be used in
 * the main application.
 * 
 * Currently this function is called on by the initialization function so the
 * LCD is blank upon enabling prior to the first update.
*/
void        lcd_segments_clear (void);
/**
 * Sets all segments.
 * This sets ALL segments on COM0-3, including unused segments. This function
 * is really only useful for debug purposes.
*/
void        lcd_segments_set (void);

#endif

// EOF //
