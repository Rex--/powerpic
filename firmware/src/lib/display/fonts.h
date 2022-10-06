/** @file fonts.h
 * 
 * This file contains the fonts for use on the 7-segment display.
 * 
 * Each font character is represented as a byte with each bit representing a
 * segment. The font range spans a limited range of the ascii character set from
 * 32 (space) to 126 (~). This gives 95 unique font characters for use by the 
 * display.
 * 
 * The default font maps ascii characters to the closest representation possible.
 * Both capital and lowercase letters are implemented. Some cases cannot be made
 * on a 7-segment display so the opposite case is used instead. For example, 'D'
 * cannot be represented without looking like a '0', so lowercase 'd' is used
 * instead. Special cases like these are marked with a '**' in the code.
*/

#ifndef DISPLAY_FONTS_H
#define DISPLAY_FONTS_H

/**
 * Offset of ascii character values to our font array index.
 * The characters are in the same order as ascii, starting with 32 (0x40) or
 * a space (" ").
*/
#define DISPLAY_CHARACTER_ASCII_OFFSET  32

/** The size of the font array. */
#define FONT_SIZE   95

/**
 * Enum to easily define character segments.
 * A-P are used for the primary display characters,
 * this allows them to fit into a byte.
 * A-J are use for the secondary display which allows us to display more
 * characters.
*/
enum display_segments {
    A=1,        // Top      Center
    B=2,        // Top      Right
    C=4,        // Bottom   Right
    D=8,        // Bottom   Center
    E=0x10,     // Bottom   Left
    F=0x20,     // Top      Left
    G=0x40,     // Center   Center
    P=0x80,     // Period   (Not implemented)

    H=0x100,    // Extra segments for the secondary display. (Not implemented)
    I=0x200,
    J=0x400
};

/**
 * Default display font.
*/
const unsigned char default_font[FONT_SIZE] = {

    // Punctuation
    0x0,                // Space
    B|C|P,              // !
    B|F,                // "
    0x0,                // # ???
    0x0,                // $ ???
    0x0,                // % ???
    0x0,                // & ???
    F,                  // ' Top left
    B|C,                // ( Line on right side
    E|F,                // ) Line on left side
    A|B|F|G,            // * Like a degrees sign
    D|E,                // + Kinda like a shorter L
    E,                  // , Bottom left
    G,                  // -
    P,                  // . This adds a period but takes up a space.
    B|E|G,              // /

    // Numbers 0-9
    A|B|C|D|E|F,        // 0
    B|C,                // 1
    A|B|D|E|G,          // 2
    A|B|C|D|G,          // 3
    B|C|F|G,            // 4
    A|C|D|F|G,          // 5
    A|C|D|E|F|G,        // 6
    A|B|C|F,            // 7
    A|B|C|D|E|F|G,      // 8
    A|B|C|D|F|G,        // 9

    // Punctuation
    0x0,                // : ???
    0x0,                // ; ???
    D|E|G,              // < Like c
    D|G,                // =
    C|D|G,              // > Like a backwards c
    A|B|E|G,            // ? No dot - kinda weird
    0x0,                // @ ???
    
    // Uppercase alphabet
    A|B|C|E|F|G,        // A
    F|E|G|C|D,          // B ** Lowercase
    A|D|E|F,            // C
    B|C|D|E|G,          // D ** Lowercase
    A|D|E|F|G,          // E
    A|E|F|G,            // F
    A|C|D|E|F|G,        // G
    B|C|E|F|G,          // H
    E|F,                // I Like 1 except on the left
    B|C|D|E,            // J
    B|C|E|F|G,          // K Like H and X
    D|E|F,              // L
    A|C|E,              // M Kinda weird
    A|B|C|E|F,          // N ** Big lowercase n
    A|B|C|D|E|F,        // O
    A|B|E|F|G,          // P
    A|B|C|D|E|F,        // Q Like O
    A|E|F,              // R ** Lowercase, but big
    A|C|D|F|G,          // S Like a 5
    A|E|F,              // T Upside down L
    B|C|D|E|F,          // U Like V
    B|C|D|E|F,          // V Like U
    B|D|F,              // W Kinda weird
    B|C|E|F|G,          // X Like H and K
    B|C|F|G,            // Y
    A|B|D|E|G,          // Z

    // Punctuation
    A|D|E|F,            // [ Like C
    C|F|G,              // \ backslash
    A|B|C|D,            // ] Like a backwards C
    A|B|F,              // ^
    D,                  // _
    B,                  // ` Top right
    
    // Lowercase alphabet
    A|B|C|E|F|G,        // A ** Uppercase
    F|E|G|C|D,          // b
    D|E|G,              // c
    B|C|D|E|G,          // d
    A|D|E|F|G,          // E ** Uppercase
    A|E|F|G,            // f
    A|B|C|D|F|G,        // g
    C|E|F|G,            // h
    E,                  // i No dot, on left
    B|C|D|E,            // J
    B|C|E|F|G,          // K ** Uppercase Like H and X
    D|E|F,              // l Like a capitol I
    A|C|E,              // M Kinda weird
    C|E|G,              // n
    C|D|E|G,            // o
    A|B|E|F|G,          // P ** Uppercase
    A|B|C|F|G,          // q Backwards P
    E|G,                // R ** Lowercase
    A|C|D|F|G,          // S Like a 5
    D|E|F|G,            // t
    C|D|E,              // u Like v
    C|D|E,              // v Like u
    B|D|F,              // W Kinda weird
    B|C|E|F|G,          // X ** Uppercase Like H and K
    B|C|D|F|G,          // Y
    A|B|D|E|G,          // Z ** Uppercase pretty much

    // Punctuation
    B|C|G,              // {
    B|C,                // | Left side
    E|F|G,              // }
    A,                  // ~ Top segment
};

/**
 * Available display fonts.
*/
const unsigned char * display_fonts[] = {
    default_font,
};

#endif

// EOF //
