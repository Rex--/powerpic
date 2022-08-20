/** @file settings.c
 * 
 * Persistent settings lib for PowerOS.
*/

#include "drivers/nvm.h"

#include "lib/settings.h"

unsigned char
settings_get (unsigned char id)
{
    // Right now this function is just a wrapper around reading eeprom.
    return nvm_eeprom_read(id);
}

unsigned int
settings_get_int (unsigned char idh, unsigned char idl)
{
    unsigned int ret = 0;

    ret = (unsigned int)(nvm_eeprom_read(idh) << 8);  // Get MSB of setting
    ret |= nvm_eeprom_read(idh);        // Get LSB of setting

    return ret;
}

void
settings_set (unsigned char id, unsigned char value)
{
    // Right now this function is just a wrapper around writing eeprom.
    nvm_eeprom_write(id, value);
}


// EOF //
