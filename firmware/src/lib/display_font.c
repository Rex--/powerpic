
#include "lib/display_font.h"

/**
 * Enum to easily define character segments.
 * A-P are used for the primary display characters,
 * this allows them to fit into a byte.
 * A-J are use for the secondary display which allows us to display more
 * characters.
*/
enum segment_names {
    A=1, 
    B=2, 
    C=4, 
    D=8, 
    E=0x10, 
    F=0x20, 
    G=0x40,
    P=0x80,     // Period
    H=0x100,
    I=0x200,
    J=0x400
};

const unsigned char default_font[] = {

    // Punctuation
    0x0,                // Space
    B|C|P,              // !
    B|F,                // "
    0x0,                // #
    0x0,                // $
    0x0,                // %
    0x0,                // &
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
    A|B|C,              // 7
    A|B|C|D|E|F|G,      // 8
    A|B|C|D|F|G,        // 9

    // Puncuation
    0x0,                // :
    0x0,                // ;
    D|E|G,              // < Like c
    D|G,                // =
    C|D|G,              // > Like a backwards c
    A|B|E|G,            // ? No dot - kinda weird
    0x0,                // @
    
    // Uppercase alphabet
    A|B|C|E|F|G,        // A
    F|E|G|C|D,          // B ** Lowercase
    A|D|E|F,            // C
    B|C|D|E|G,          // D ** Lowercase
    A|D|E|F|G,          // E
    A|E|F|G,            // F
    A|C|D|E|F|G,        // G ** Lowercase
    C|E|F|G,            // h ** Lowercase
    E|F,                // I Like 1 except on the left
    B|C|D|E,            // J
    B|C|E|F|G,          // K Like H and X
    D|E|F,              // L
    A|C|E,              // M Kinda weird but best I can do
    A|B|C|E|F,          // N Big n
    A|B|C|D|E|F,        // O
    A|B|E|F|G,          // P
    A|B|C|D|E|F,        // Q
    E|G,                // R ** Lowercase
    A|C|D|F|G,          // S Like a 5
    D|E|F|G,            // T ** Lowercase
    B|C|D|E|F,          // U Like V
    B|C|D|E|F,          // V Like U
    B|D|F,              // W Kinda weird
    B|C|E|F|G,          // X Like H and K
    B|C|D|F|G,          // Y ** Lowercase
    A|B|D|E|G,          // Z

    // Puncuation
    A|D|E|F,            // [ Like C
    C|F|G,              // backslash
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
    A|C|D|E|F|G,        // G ** Lowercase?
    C|E|F|G,            // h
    E|F,                // i No dot, on left
    B|C|D|E,            // J
    B|C|E|F|G,          // K ** Uppercase Like H and X
    D|E|F,              // l Like a capitol I
    A|C|E,              // M Kinda weird but best I can do
    C|E|G,              // n
    C|D|E|G,            // o
    A|B|E|F|G,          // P ** Uppercase
    A|B|C|D|E|F,        // q Backwards P
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