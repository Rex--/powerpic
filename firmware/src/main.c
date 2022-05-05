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

#include "dev_config.h"
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
    // Init some base libs
    //
    rtcc_init();
    uart_init();
    display_enable();
    keypad_enable();
    buttons_enable();

    // Enable interrupts
    //
    ei();
    // Enable peripherial interrupts
    //
    INTCONbits.PEIE = 1;

    // Now we have basic watch functionality
    printf("\n\r\n\rHello World!\n\r\n\r");
    display_primary_string(0, "PowerPIC");
    // display_update();

    mode_manager_init();

    // mode_manager_add(test_mode_init());
    // mode_manager_add(clock_mode_init());
    // mode_manager_add(power_init());

    // mode_manager_start(0);

    int mode_return_value = 0;
    int mode_tickrate = 0;
    int mode_keymap = 0;
    
    tick_enable();
    tick_set_ms(1000);


    printf("RTCCAL: %x\n\r", RTCCAL);

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

        if (mode_manager_config_update())
        {
            mode_tickrate = mode_manager_get_tickrate();

            if (0 < mode_tickrate)
            {
                tick_set_ms((unsigned int)mode_tickrate);
                timer0_interrupt_enable();
            }
            else
            {
                timer0_interrupt_disable();
            }

            mode_keymap = mode_manager_get_keymap();

            keypad_keymap_set(mode_keymap);

            display_set_font(mode_manager_get_font());
        }

        if (0 != mode_tickrate)
        {
            tick_reset();
            // Sleep. To be awoken by a tick or a keypad press
            //
            SLEEP();
            // printf("sleep timer0: %u\n\r", timer0_get());
            NOP();
        }
    }
}

/*** EOF ***/
