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

#include "lib/logging.h"

#include "modes/settings.h"


#undef  LOG_TAG
#define LOG_TAG "mode.settings"

// Each setting item holds a display label and the setting value.
typedef struct
{
    const char * label;
    volatile int value;
} settings_item_t;

// This array holds a setting struct for each setting.
// The values defined here are the defaults.
static settings_item_t settings_list[] = {
    {"rtc", 0},         // RTC Drift Calibration
    // {"max", -128},
    // {"min", 127},
};

// Setting index values as enum
enum settings_items {
    SETTING_RTC,
    SETTING_MAX,
    SETTING_MIN,
    MAX_SETTINGS,
};

// Display setting's label and value
static void settings_display (unsigned char setting_index);
static void settings_display_edit (unsigned char setting_index);

// Set setting
static void settings_value_set (unsigned char setting_index, int value);

// Run function that gets input from user for value.
signed char settings_edit (unsigned int event);

// Holds the current setting index
static unsigned char settings_active = 0;

// Holds the current value being edited
static int edit_value = 0;


void
settings_start (void)
{
    // Display settings tag.
    display_secondary_string(1, "SE");

    // We use button events.
    tick_disable();

    // We use the selection keypad.
    keypad_keymap_set(KEYMAP_SELECTION);

    // Display initial setting.
    settings_display(0);
    settings_active = 0;
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
            if (MAX_SETTINGS-1 == settings_active)
            {
                // Loop to first setting
                settings_active = 0;
            }
            else
            {
                settings_active++;
            }
            settings_display(settings_active);
        }
        if ('<' == keycode)
        {
            // Previous setting
            if (0 == settings_active)
            {
                // Loop to last setting
                settings_active = MAX_SETTINGS-1;
            }
            else
            {
                settings_active--;
            }
            settings_display(settings_active);
        }
        if ('^' == keycode)
        {
            // Adjust setting +1
        }
        if ('v' == keycode)
        {
            // Adjust setting -1
        }
        if ('=' == keycode)
        {
            // Edit selection (keypad input)
            edit_value = 0;                     // Initial number is 0.
            settings_display_edit(settings_active);
            keypad_keymap_set(KEYMAP_CASIO);    // Casio keymap
            settings_mode.run = &settings_edit; // Edit run loop gets value.
        }
    break;

    case KEYPAD_EVENT_RELEASE:
    break;

    case EVENT_BUTTON:
        if (BUTTON_MODE_PRESS == EVENT_DATA(event))
        {
            return 1;
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
    case KEYPAD_EVENT_PRESS:
        keypress = EVENT_DATA(event);
        if ('0' <= keypress && keypress <= '9')
        {
            // Keypress is a number
            keypress = keypress - 48;   // Get int value
            edit_value = (edit_value * 10) + keypress;  // Add keypress as LSD.
            // Display new value.
            settings_display_edit(settings_active);
        }
        if ('.' == keypress)
        {
            // Period key deletes last digit
            edit_value /= 10;

            // Display new value
            settings_display_edit(settings_active);
        }
        if ('-' == keypress)
        {
            // Minus key converts number to negative
            if (0 < edit_value)
            {
                edit_value = -edit_value;
            }
        }
        if ('+' == keypress)
        {
            // Plus key converts number to positive
            if (0 > edit_value)
            {
                edit_value = -edit_value;
            }
        }
        if ('=' == keypress)
        {
            LOG_DEBUG("Setting value %i", edit_value);
            // Equals key sets the value
            settings_value_set(settings_active, edit_value);
            settings_display(settings_active);      // Display value
            keypad_keymap_set(KEYMAP_SELECTION);    // Set keymap back
            settings_mode.run = &settings_run;      // Set run loop back
        }
    break;

    case EVENT_BUTTON:
        if (EVENT_DATA(event) == BUTTON_MODE_PRESS)
        {
            // Press of the mode button cancels
            settings_display(settings_active);      // Display value
            keypad_keymap_set(KEYMAP_SELECTION);    // Set keymap back
            settings_mode.run = &settings_run;      // Set run loop back
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
    // Settings are pretty unique so we might should do something different for
    // each one.
    settings_item_t *setting = &settings_list[setting_index];
    display_primary_clear(0);                   // Clear display
    display_primary_string(1, setting->label);  // Display label
    display_primary_number(-1, setting->value); // Display value
}

static void
settings_display_edit (unsigned char setting_index)
{
    // Display the current edited value
    settings_item_t *setting = &settings_list[setting_index];
    display_primary_clear(0);                   // Clear display
    display_primary_string(1, setting->label);  // Display label
    display_primary_number(-1, edit_value);     // Display value
}


static void
settings_value_set (unsigned char setting_index, int value)
{
    settings_item_t *setting = &settings_list[setting_index];

    // Save new value.
    setting->value = value;

    // Each value needs to set something specific.
    switch (setting_index)
    {
    case SETTING_RTC:
        // This setting modifies the RTCCAL register.
        // Acceptable values are -128 to 127, but we don't check.
        rtcc_drift_set((unsigned char)value);
    break;
    
    default:
        LOG_DEBUG("Setting %s to %i", setting->label, value);
    break;
    }
}


// EOF //
