/** @file settings.c
 * 
 * This mode provides a way to configure watch settings.
*/

#include <xc.h>

#include "drivers/rtcc.h"

#include "lib/mode.h"
#include "lib/events.h"
#include "lib/tick.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/keypad.h"
#include "lib/settings.h"           // This might get confusing
// #include "modes/mode_settings.h"    // Oh yea this is gonna be bad

#define LOG_TAG "mode.settings"
#include "lib/logging.h"

#include "modes/settings.h"

#define SETTINGS_KEYMAP KEYMAP_DIRECTIONAL

// Holds the current setting id and value
static unsigned char settings_active_id = 0;
static unsigned char settings_active_value = 0;

// Holds the current value being edited
static unsigned char edit_value = 0;

// Variable to determine if we should blink when editing value
unsigned char is_blinking = 0;

// Alternative mode functions
signed char settings_edit (unsigned int event);

// Display setting's ID and value
static void settings_display (unsigned char id);
static void settings_display_edit (void);


void
settings_init (void)
{
}

void
settings_start (void)
{
    // Display settings tag.
    display_secondary_string(1, "SE");

    // We use button events.
    // tick_disable();

    // We use the selection keypad.
    keypad_keymap_set(SETTINGS_KEYMAP);

    // Display initial setting.
    settings_display(0);
    settings_active_id = 0;
    settings_active_value = 0;
}

signed char
settings_run (unsigned int event)
{
    unsigned char keycode;

    switch (EVENT_TYPE(event))
    {

    case EVENT_TICK:
    break;

    case KEYPAD_EVENT_PRESS:
        keycode = EVENT_DATA(event);

        if ('>' == keycode)
        {
            // Next setting
            settings_display(settings_active_id + 1);
        }
        if ('<' == keycode)
        {
            // Previous setting
            settings_display(settings_active_id - 1);
        }
        if ('^' == keycode)
        {
            // Next setting +10
            // settings_active_id += 10;
            settings_display(settings_active_id + 10);
        }
        if ('v' == keycode)
        {
            // previous setting -10
            // settings_active_id -= 10;
            settings_display(settings_active_id - 10);
        }
    break;

    case EVENT_BUTTON:
        if (BUTTON_MODE_PRESS == EVENT_DATA(event))
        {
            return 1;
        }
        if (BUTTON_ADJ_PRESS == EVENT_DATA(event))
        {
            // Edit setting value
            edit_value = settings_active_value;
            keypad_keymap_set(KEYMAP_CASIO);
            settings_mode.run = &settings_edit;
            tick_rate_set_ms(500); // Set tickrate to 500ms for blinking
            is_blinking = 1; // We want to blink off the bat
        }
    break;

    default:
    break;
    }

    return 0;
}




signed char
settings_edit (unsigned int event)
{
    unsigned char keypress;

    switch (EVENT_TYPE(event))
    {
    case EVENT_TICK:
        if (is_blinking)
        {
            // Clear equal sign
            display_primary_clear(4);
            is_blinking = 0;
        }
        else
        {
            // Display equal sign
            display_primary_character(4, '=');
            is_blinking = 1;
        }
    break;

    case KEYPAD_EVENT_PRESS:
        keypress = EVENT_DATA(event);
        if ('0' <= keypress && keypress <= '9')
        {
            // Keypress is a number
            keypress = keypress - 48;   // Get int value
            edit_value = (edit_value * 10) + keypress;  // Add keypress as LSD.
            // Display new value.
            settings_display_edit();
            is_blinking = 0;
        }
        if ('.' == keypress)
        {
            // Period key deletes last digit
            edit_value /= 10;

            // Display new value
            settings_display_edit();
            is_blinking = 0;
        }
    break;

    case EVENT_BUTTON:
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Press of the mode button cancels
            settings_display(settings_active_id);      // Display value
            keypad_keymap_set(SETTINGS_KEYMAP);    // Set keymap back
            settings_mode.run = &settings_run;      // Set run loop back
            tick_disable(); // disable ticks
        }
        else if (EVENT_DATA(event) == BUTTON_ADJ_PRESS)
        {
            LOG_DEBUG("Setting value %i", edit_value);
            settings_active_value = edit_value;
            // Adj button sets the value
            // display_primary_clear(0);
            // display_primary_string(4, "SET");       // Display Confirmation
            settings_set(settings_active_id, settings_active_value);
            keypad_keymap_set(SETTINGS_KEYMAP);    // Set keymap back
            settings_display(settings_active_id);      // Display set value
            settings_mode.run = &settings_run;      // Set run loop back
            tick_disable(); // disable ticks
            // Adj button up returns to main settings loop
        }
        else if (EVENT_DATA(event) == BUTTON_ADJ_RELEASE)
        {
        }
    break;
    
    default:
    break;
    }

    return 0;
}


static void
settings_display (unsigned char setting_index)
{
    settings_active_id = setting_index;
    settings_active_value = settings_get(setting_index);
    display_primary_clear(0);                   // Clear display
    display_primary_number(3, settings_active_id);  // Display label
    display_primary_character(4, '='); // Display '='
    display_primary_number(8, settings_active_value); // Display value
}

static void
settings_display_edit (void)
{
    display_primary_clear(0);                   // Clear display
    display_primary_number(3, settings_active_id);  // Display index
    display_primary_character(4, '='); // Display '='
    display_primary_number(8, edit_value); // Display value
}


// EOF //
