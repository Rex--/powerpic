/** @file lcd.c
 * LCD Driver for PIC16LF1919x devices using the hardware LCD controller.
*/

#include <xc.h>

#include "drivers/lcd.h"


/** Number of registers that the lcddata spans. */
#define LCD_NUM_DATA_REGISTERS      24


// Each segment value consists of a high byte representing the LCDDATA register
// and a low byte representing the bit number of the segment.

/**
 * This matrix holds a value representing each segment's location for the
 * primary 8-character display. The character index starts with the left most
 * position being 1. A blank character is located at index 0.
*/
static const unsigned int primary_segments[9][7] = {
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},

#if (1 == PCB_REV)
    {0x1704, 0x1705, 0x1105, 0x0B04, 0x0B03, 0x1103, 0x1104},   // 1 Left most
    {0x1507, 0x0C00, 0x0600, 0x0907, 0x0902, 0x0F02, 0x0F07},   // 2
    {0x0E07, 0x1201, 0x0C01, 0x0900, 0x0F01, 0x1501, 0x1500},   // 3
    {0x1203, 0x1204, 0x0C04, 0x0603, 0x0602, 0x0C02, 0x0C03},   // 4
    {0x1206, 0x1207, 0x0C07, 0x0606, 0x0605, 0x0C05, 0x0C06},   // 5
    {0x1300, 0x1301, 0x0D01, 0x0700, 0x0906, 0x0F06, 0x0D00},   // 6
    {0x1303, 0x1505, 0x0F05, 0x0703, 0x0702, 0x0D02, 0x0D03},   // 7
    {0x1604, 0x1605, 0x1005, 0x0A04, 0x0A06, 0x1006, 0x1004}    // 8 Right most

#elif (2 == PCB_REV)
    {0x1500, 0x1501, 0x0F01, 0x0900, 0x0807, 0x0E07, 0x0F00},
    {0x1705, 0x1104, 0x0B04, 0x0B05, 0x0902, 0x0F02, 0x1105},
    {0x0D04, 0x1403, 0x0E03, 0x0B02, 0x1103, 0x1703, 0x1702},
    {0x1404, 0x1701, 0x1101, 0x0804, 0x0805, 0x0E05, 0x0E04},
    {0x1604, 0x1507, 0x0F07, 0x0A04, 0x0A05, 0x1005, 0x1004},
    {0x1201, 0x1202, 0x0C02, 0x0601, 0x0600, 0x0C00, 0x0C01},
    {0x1204, 0x1205, 0x0C05, 0x0604, 0x0603, 0x0C03, 0x0C04},
    {0x1207, 0x1506, 0x0F06, 0x0607, 0x0606, 0x0C06, 0x0C07}
#endif
};

/**
 * This matrix holds a value representing each segment's location for the
 * secondary 2.5-character display. The index starts with left character being
 * position 1.
*/
static const unsigned int secondary_segments[3][10] = {
    {0x0},                                                      // 0 Blank

#if (1 == PCB_REV)

    {0x0805, 0x0E05, 0x1404, 0x1606, 0x1406, 0x0E06, 0x1405,    // 1 Left
     0x0806, 0x1302, 0x0807},   // 3 extra segments for the .5 character.
    
    {0x0803, 0x0A07, 0x1007, 0x1607, 0x1403, 0x0E04, 0x0E03,    // 2 Right
     0x0804, 0x0000, 0x0000}    // This charcter only has 1 extra, so the
                                // 2 high bits are empty.

#elif (2 == PCB_REV)
    {0x0703, 0x0D03, 0x1302, 0x1206, 0x1505, 0x0F05, 0x1303,
     0x0905, 0x1203, 0x0704},

    {0x0701, 0x0700, 0x0D00, 0x1300, 0x1301, 0x0D02, 0x0D01,
     0x0702, 0x0000, 0x0000}
#endif
};

/**
 * This array holds a value representing each segment's location for the
 * punctuation on the display. The index start with the left most period being 1
 * and ending with the colon being 9.
*/
static const unsigned int period_segments[10] = {
    0x0,        // 0 Blank

#if (1 == PCB_REV)
    0x0B05,     // 1 Left most
    0x0901,     // 2
    0x0601,     // 3
    0x0604,     // 4
    0x0607,     // 5
    0x0701,     // 6
    0x0905,     // 7
    0x0A05,     // 8 Right most
    0x0F00      // 9th period is the colon.

#elif (2== PCB_REV)
    0x0901,
    0x0B03,
    0x0803,
    0x0B01,
    0x0907,
    0x0602,
    0x0605,
    0x0906,
    0x1102
#endif
};

/**
 * This array holds a value representing each segment's location for the
 * operation signs on the display. The addition sign is index 1, the subtraction
 * sign index 2, the multiplication sign index 3, and the division sign index 4.
*/
static const unsigned int sign_segments[5] = {
    0x0,

#if (1 == PCB_REV)
    0x1506,     // Addition sign
    0x1205,     // Subtraction sign
    0x1202,     // Multiplication sign
    0x1407      // Division sign

#elif (2 == PCB_REV)
    0x1200,
    0x1605,
    0x1405,
    0x1304
#endif
};

/**
 * This array holds a value representing each segment's location for the
 * misc segments on the display. The chime bell is index 1, the alarm wave thing
 * is index 2, the letter K is index 3, PM indicator is index 4, AM indicator
 * is index 5.
 * TODO: Maybe we should make time of day its own set of functions?
*/
static const unsigned int misc_segments[6] = {
    0x0,

#if (1 == PCB_REV)
    0x1102,     // Bell
    0x0B02,     // Wave thingy
    0x1702,     // [O]K
    0x1502,     // PM
    0x1703      // AM

#elif (2 == PCB_REV)
    0x0E06,
    0x0806,
    0x1406,
    0x1502,
    0x1407
#endif
};

/**
 * Pointer to the LCDDATA registers.
 * These directly effect the LCD segments. Each COM is 6 1-byte registers, with
 * each bit controlling a segment. This pointer allows us to address each
 * segment with `lcd_data[x][y]`.
*/
static volatile unsigned char *lcd_data = &LCDDATA0;

/**
 * LCD working buffer.
 * This buffer holds data to be written to the lcd upon a lcd_update() call.
 * This allows a clean screen refresh as the whole lcd can be drawn.
*/
static volatile unsigned char lcd_buffer[LCD_NUM_DATA_REGISTERS];


static void         lcd_segments_enable (void);


void
lcd_init (void)
{
#   if (1 == PCB_REV)
    // Configure controllr to use LFINTOSC
    //
    LCDCONbits.CS = 0;

#   elif (2 == PCB_REV)
    // Configure controllr to use SOSC
    //
    LCDCONbits.CS = 1;
#   endif

    // Configure controller to use 1/4 mux.
    //
    LCDCONbits.LMUX = 0x04;

    // Configure controller to use only charge pump for lcd voltages.
    //
    LCDVCON2bits.LCDVSRC = 0x6;

    // Charge pump in 5v LP mode with output set to 3.78v
    //
    LCDVCON1 = 0b11000000;

    // Type-A waveform
    //
    LCDPSbits.WFT = 0;
    
    // Configure prescaler 1:6 to give us 40 hz
    //
    LCDPSbits.LP = 0b0101;

    lcd_segments_clear();
    
    lcd_segments_enable();

    // Enable LCD Controller
    //
    LCDCONbits.LCDEN = 1;
}

void
lcd_update (void)
{
    // We start at LCDDATA6 which skips COM0 entirely. This is because COM0 is
    // not actually used on the LCD, so we can save some time.
    //
    for (int lcddata = 6; lcddata < LCD_NUM_DATA_REGISTERS; lcddata++)
    {
        lcd_data[lcddata] = lcd_buffer[lcddata];
    }
}


// Primary

void
lcd_primary_draw (unsigned char position, unsigned char segment)
{
    unsigned int  segment_address = primary_segments[position][segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] |= seg_bit_mask;
}

void
lcd_primary_clear (unsigned char position, unsigned char segment)
{
    unsigned int  segment_address = primary_segments[position][segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));
    lcd_buffer[com_byte] &= ~(seg_bit_mask);
}


// Secondary

void
lcd_secondary_draw (unsigned char position, unsigned char segment)
{
    unsigned int  segment_address = secondary_segments[position][segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] |= seg_bit_mask;
}

void
lcd_secondary_clear (unsigned char position, unsigned char segment)
{
    unsigned int  segment_address = secondary_segments[position][segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] &= ~(seg_bit_mask);
}


// Punctuation

void
lcd_period_draw (unsigned char segment)
{
    unsigned int  segment_address = period_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] |= seg_bit_mask;
}

void
lcd_period_clear (unsigned char segment)
{
    unsigned int  segment_address = period_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));
    
    lcd_buffer[com_byte] &= ~(seg_bit_mask);
}


// Math signs

void
lcd_sign_draw (unsigned char segment)
{
    unsigned int  segment_address = sign_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] |= seg_bit_mask;
}

void
lcd_sign_clear (unsigned char segment)
{
    unsigned int  segment_address = sign_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));
    
    lcd_buffer[com_byte] &= ~(seg_bit_mask);
}


// Misc segments

void
lcd_misc_draw (unsigned char segment)
{
    unsigned int  segment_address = misc_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));

    lcd_buffer[com_byte] |= seg_bit_mask;
}

void
lcd_misc_clear (unsigned char segment)
{
    unsigned int  segment_address = misc_segments[segment];
    unsigned char com_byte = (segment_address >> 8);
    unsigned char seg_bit_mask = (unsigned char)(1U << (segment_address & 0xF));
    
    lcd_buffer[com_byte] &= ~(seg_bit_mask);
}


// All segments

void
lcd_segments_clear (void)
{
    for (char lcddata = 0; lcddata < LCD_NUM_DATA_REGISTERS; lcddata++)
    {
        lcd_data[lcddata] = 0x0;
        lcd_buffer[lcddata] = 0x0;
    }
}

void
lcd_segments_set (void)
{
    for (char lcddata = 0; lcddata < LCD_NUM_DATA_REGISTERS; lcddata++)
    {
        lcd_data[lcddata] = 0xFF;
        lcd_buffer[lcddata] = 0xFF;
    }
}


/**
 * Enable LCD controller segments.
 * This enables the needed segments for COM0-3.
*/
static void
lcd_segments_enable (void)
{
#   if (1 == PCB_REV)
    // Seg 1-7
    //
    LCDSE0 = 0xFF;

    // Seg 8-11
    //
    LCDSE1 = 0x0F;

    // Seg 19-23
    //
    LCDSE2 = 0xF8;

    // Seg 24 - 26, 29-31
    //
    LCDSE3 = 0xE7;

    // Seg 36 - 39
    //
    LCDSE4 = 0xF0;

    //Seg 42-45
    //
    LCDSE5 = 0x3C;

#   elif (2 == PCB_REV)
    LCDSE0 = 0b11111111;

    LCDSE1 = 0b00011111;

    LCDSE2 = 0b11111000;

    LCDSE3 = 0b11100111;

    LCDSE4 = 0b00110000;

    LCDSE5 = 0b00111111;
#   endif
}

// EOF //
