/** @file settings.c
 * 
 * Persistent settings lib for CasiOS.
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
    unsigned int ret = (unsigned int)(nvm_eeprom_read(idh) << 8);  // Get MSB of setting
    ret |= (unsigned int)(nvm_eeprom_read(idl));        // Get LSB of setting
    return ret;
}

void
settings_set (unsigned char id, unsigned char value)
{
    // Right now this function is just a wrapper around writing eeprom.
    nvm_eeprom_write(id, value);
}

void
settings_set_int (unsigned char idh, unsigned char idl, unsigned int value)
{
    nvm_eeprom_write(idh, (unsigned char)(value >> 8));
    nvm_eeprom_write(idl, (unsigned char)(value & 0xFF));
}


// EOF //
