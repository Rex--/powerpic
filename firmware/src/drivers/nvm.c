/** @file nvm.c
 * 
 * Driver for the NVM control module for PIC16LF1919x.
*/

#include <xc.h>

#include "drivers/nvm.h"

// NVM unlock sequence. This will set the WR bit and write any data.
static void nvm_unlock (void);

unsigned char
nvm_eeprom_read (unsigned char address)
{
    // Set bit to indicate were reading EEPROM
    NVMCON1bits.NVMREGS = 1;

    // Load address into NVMADR registers
    NVMADRH = 0x70;
    NVMADRL = address;

    // Set bit to initiate read
    NVMCON1bits.RD = 1;

    // Data is available the very next cycle.
    return NVMDATL;
}


void
nvm_eeprom_write (unsigned char address, unsigned char data)
{
    // Set bit to indicate were writing EEPROM
    NVMCON1bits.NVMREGS = 1;

    // Enable NVM writes
    NVMCON1bits.WREN = 1;

    // Load data to write
    NVMDATL = data;

    // Load address
    NVMADRH = 0x70;
    NVMADRL = address;

    // Perform unlock sequence, this will also start writing data.
    nvm_unlock();

    while (NVMCON1bits.WR)
    {
        // Wait for write completion.
    }

    // Disable writes
    NVMCON1bits.WREN = 0;
}

static void
nvm_unlock (void)
{
    // The unlock sequence must not be interrupted, so we disable interrupts.
    di();

    // Step 1. Write 0x55 to NVMCON2
    NVMCON2 = 0x55;

    // Step 2. Write 0xAA to NVMCON2
    NVMCON2 = 0xAA;

    // Step 3. Set WR bit of NVMCON1
    //  This will start the write.
    NVMCON1bits.WR = 1;

    // Unlock sequence complete, we may reenable interrupts.
    ei();
}


// EOF //
