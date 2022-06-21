/** @file xtal.h
 * Clock Configuration Driver.
 * Function-like macros are used instead of inline functions primarily because
 * the unlicensed xc8 compiler does not support an optimization level high
 * enough to actually inline the functions. This saves a couple instructions.
*/

#ifndef _xtal_h_
#define _xtal_h_

/** Initialize clock source.
 * Initialization consists of:
 * - Clearing clock divider.
 * - Setting HFINTOSC to match _XTAL_FREQ macro.
*/
void    xtal_init (void);


// NOSC/COSC Bit Settings
//
#define XTAL_EXTOSC         0b111
#define XTAL_HFINTOSC       0b110
#define XTAL_LFINTOSC       0b101
#define XTAL_SOSC           0b100
#define XTAL_EXTOSC_4X      0b010
#define XTAL_HFINTOSC_2X    0b001

/** Set source for the instruction clock.
 * This function-like macro modifies the NOSC bits of OSCCON1.
*/
#define xtal_source_set(nosc)   (OSCCON1bits.NOSC = nosc)

/** Get source for the instruction clock.
 * This function-like macro returns the current source. Could differ from the
 * set source if the new source is not ready.
*/
#define xtal_source_get()       (OSCCON2bits.COSC)


// NDIV/CDIV Bit Settings
//
#define XTAL_DIV_1      0b0000
#define XTAL_DIV_2      0b0001
#define XTAL_DIV_4      0b0010
#define XTAL_DIV_8      0b0011
#define XTAL_DIV_16     0b0100
#define XTAL_DIV_32     0b0101
#define XTAL_DIV_64     0b0110
#define XTAL_DIV_128    0b0111
#define XTAL_DIV_256    0b1000
#define XTAL_DIV_512    0b1001

/** Set divider for clock source.
 * This function-like macro modifies the NDIV bits of OSCCON1.
*/
#define xtal_divider_set(ndiv)  (OSCCON1bits.NDIV = ndiv)

/** Get divider for clock source.
 * This function-like macro returns the current clock division factor.
*/
#define xtal_divider_get()      (OSCCON2bits.CDIV)

// HFFRQ Settings
//
#define XTAL_HFFRQ_1    0b000
#define XTAL_HFFRQ_2    0b001
#define XTAL_HFFRQ_4    0b010
#define XTAL_HFFRQ_8    0b011
#define XTAL_HFFRQ_12   0b100
#define XTAL_HFFRQ_16   0b101
#define XTAL_HFFRQ_32   0b110

/** Set frequency of HFINTOSC.
 * Sets the frequency of the high frequency internal oscillator module.
*/
#define xtal_hfintosc_freq_set(hffrq)   (OSCFRQbits.HFFRQ = hffrq)

/** Get frequency of HFINTOSC.
 * Gets the frequency of the high frequency internal oscillator module.
*/
#define xtal_hfintosc_freq_get()        (OSCFRQbtis.HFFRQ)


#endif

// EOF //
