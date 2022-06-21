/** @file pins.h
 * General I/O Pin driver.
 * This driver handles setting the direction of, and reading/writing to pins.
*/


#ifndef _pins_h_
#define _pins_h_


void    pins_init (void);


// Direction Control
//

/** Set pin to function as output. */
#define pin_set_output(tris, pin)   ((tris) &= ~(1 << (pin)))

/** Set pin to function as input. */
#define pin_set_input(tris, pin)    ((tris) |= (1 << (pin)))

/** Set pin mask to function as output. */
#define pin_mask_output(tris, mask) ((tris) &= ~(mask))

/** Set pin mask to function as input. */
#define pin_mask_input(tris, mask) ((tris) |= (mask))

// Digital Functionality
//

/** Read current pin level. */
#define pin_get_level(port, pin)        (port & (1 << pin)) >> pin

/** Set pin level to value. */
#define pin_set_level(port, pin, level) \
    port = (unsigned char)((port & ~(1 << pin)) | (level << pin))

/** Set pin level high. */
#define pin_set_high(lat, pin)         (lat) |= (1 << pin)

/** Set pin level low */
#define pin_set_low(lat, pin)          (lat) &= ~(1 << pin)

/** Set pin mask level high. */
#define pin_mask_high(lat, mask)        ((lat) |= (mask))

/** Set pin mask level low. */
#define pin_mask_low(lat, mask)        ((lat) &= ~(mask))


// Pull-Up Control
//

/** Enable pull-up on pin. */
#define pin_enable_pullup(wpu, pin)     (wpu |= (1 << (pin)))

/** Disable pull-up on pin. */
#define pin_disable_pullup(wpu, pin)    (wpu &= ~(1 << (pin)))

/** Enable pull-up on pin mask. */
#define pin_mask_pullup(wpu, mask)      ((wpu) |= (mask))


// Analog Functionality Control
//
#define pin_enable_analog(ansel, pin)   (ansel |= (1 << (pin)))
#define pin_disable_analog(ansel, pin)  (ansel &= ~(1 << (pin)))

#endif

// EOF //
