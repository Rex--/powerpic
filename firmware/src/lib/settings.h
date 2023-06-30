/** @file settings.h
 * 
 * Persistent settings lib for PowerOS.
 * 
 * Settings are 8-bits in length and are stored in EEPROM. There are 256 bytes
 * available in EEPROM, and currently all are available for settings.
 * Settings have an associated ID 0-255. The ID is relative to its place in
 * EEPROM.
*/

#ifndef _settings_h_
#define _settings_h_

/**
 * Get setting value.
 * 
 * @param[in]   id      Setting ID.
 * 
 * @returns     Setting value.
*/
unsigned char
settings_get(unsigned char id);

/**
 * Get integer value stored as two settings.
 * 
 * @param[in]   idh     Setting ID of high byte.
 * @param[in]   idl     Setting ID of low byte.
 * 
 * @returns     Setting value.
*/
unsigned int
settings_get_int (unsigned char idh, unsigned char idl);

/**
 * Set setting value.
 * 
 * @param[in]   id      Setting ID.
 * @param[in]   value   Value to set.
*/
void
settings_set(unsigned char id, unsigned char value);

/**
 * Set integer value stored as two settings.
 * 
 * @param[in]   idh     Setting ID of high byte.
 * @param[in]   idl     Setting ID of low byte.
 * @param[in]   value   Value to set.
*/
void
settings_set_int (unsigned char idh, unsigned char idl, unsigned int value);


#endif

// EOF //
