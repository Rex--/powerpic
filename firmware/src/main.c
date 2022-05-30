/** @file main.c
 *
 * Main program for the PowerPIC firmware.
 *
 * TODO: Write a nice description about the PowerPIC firmware.
 * 
*/

#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#include "dev_config.h"

#include "drivers/timers.h"
#include "drivers/lcd.h"
#include "drivers/rtcc.h"

// We use these libraries to implement basic watch functionality, including:
// - Modes
// - Interrupts
// - Display
// - Buttons (2x side)
// - UART
//
#include "lib/mode.h"
#include "lib/event.h"
#include "lib/tick.h"
#include "lib/isr.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/uart.h"
#include "lib/buzzer.h"

#include "mode_manager.h"

#define EVER ;;


/**
 * Entrypoint of firmware.
 * 
 * This configures some base libraries and kicks off the mode manager.
*/
void
main (void)
{
    // Change clock divider 1:1
    // Gives us a 4MHz clock
    //
    OSCCON1bits.NDIV = 0x0;

    // Set HFINTOSC to 32MHz giving us our desired freq of 32MHz
    //
    OSCFRQ = 0b010;

    // Set HFINTOSC x2 PLL as clock source
    // OSCCON1bits.NOSC = 0b001;

    // Init some base libs
    //
    rtcc_init();
    uart_init();
    tick_enable();
    display_enable();
    keypad_enable();
    buttons_enable();
    buzzer_enable();

    // Backlight
    LATGbits.LATG7 = 0;
    TRISGbits.TRISG7 = 0;
    ANSELGbits.ANSG7 = 0;

    // Disable Modules to save power
    // PMD0 |= 0b00100100;
    // PMD1 |= 0b00010110;
    // PMD2 |= 0b01000111;
    // PMD3 |= 0b00001111;
    // PMD4 |= 0b10010001;
    // PMD5 |= 0b01011110;

    // Enable interrupts
    //
    ei();
    // Enable peripherial interrupts
    //
    INTCONbits.PEIE = 1;

    // Now we have basic watch functionality
    //
    printf("\n\r\n\rHello World!\n\r\n\r");
    display_primary_string(0, "PowerPIC");

    mode_manager_init();

    // int mode_tickrate = 0;
    // unsigned char mode_keymap = 0;
    
    // tick_set_ms(1000);
    // tick_reset();
    // tick_start();

#   define ever ;;
    // LOL
    for(ever)
    {
        unsigned int event = event_get_last();
        while (event)
        {
            // Scan the keypad, this should return immediately if no button is down.
            //
            // keypad_scan();

            // Run the mode application's main thread.
            //
            mode_manager_thread(event);

            // Update display after the mode has edited the buffer.
            // Should do the actual updating in an interrupt maybe?
            //
            display_update();

            event = event_get_last();
        }

        // if (mode_manager_config_update())
        // {
        //     mode_tickrate = mode_manager_get_tickrate();

        //     if (0 < mode_tickrate)
        //     {
        //         tick_set_ms((unsigned int)mode_tickrate);
        //         timer0_interrupt_enable();
        //     }
        //     else
        //     {
        //         timer0_interrupt_disable();
        //     }

        //     mode_keymap = mode_manager_get_keymap();

        //     keypad_keymap_set(mode_keymap);

        //     display_set_font(mode_manager_get_font());
        // }

        // if (0 != mode_tickrate)
        // {
            // tick_reset();
            // Sleep. To be awoken by a tick or a keypad press
            //
            SLEEP();
            // printf("sleep timer0: %u\n\r", timer0_get());
            NOP();
        // }
    }
}

/*** EOF ***/
