/** @file nvm.h
 * 
 * Driver for the NVM control module for PIC16LF1919x.
 * 
 * This driver handles reading/writing to the EEPROM and flash, and supports
 * reading configuration and device id words.
*/

#ifndef _nvm_h_
#define _nvm_h_

/**
 * Read a word from PFM.
 * 
 * @param[in]   address Address to read from.
 * 
 * @returns     Integer representing a 14-bit word of flash.
*/
unsigned int
nvm_read (unsigned char nvmregs, unsigned int address);

/**
 * Read a byte of data from EEPROM.
 * 
 * There are 256 bytes of EEPROM available for use. The addresses map to
 * 0xF000-0xF0FF in the flash.
 * 
 * @param[in]   address EEPROM memory address to read.
 * 
 * @returns     Byte of data read.
*/
unsigned char
nvm_eeprom_read (unsigned char address);


/**
 * Write a byte of data to EEPROM.
 * 
 * @param[in]   address EEPROM memory address to write.
 * @param[in]   data    Byte of data to write.
*/
void
nvm_eeprom_write (unsigned char address, unsigned char data);

#endif

// EOF //