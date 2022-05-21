
#include <xc.h>
#include <stdio.h>

#include "lib/mode.h"
#include "lib/display.h"
#include "lib/keypad.h"
#include "lib/battery.h"

#include "modes/power.h"


static void voltage_display (void);

static unsigned int voltage = 0;

void
power_init (mode_config_t *cfg)
{
    cfg->tickrate = 5000;
    cfg->keymap = 1;
}


void
power_start (mode_config_t *cfg)
{
    display_primary_string(1, "BAT ---v");
    display_period(5);
    battery_enable();
}


void
power_thread (mode_config_t *cfg, unsigned int event)
{
    // tick events
    if ((event & 0xFF) == 0x01)
    {
        voltage_display();
    }
    //keypad events
    else if ((event & 0xFF) == KEYPAD_KEY_PRESS_EVENT)
    {

    }
    else if (event == 0x020b)
    {
        LATGbits.LATG7 = ~LATGbits.LATG7;
    }
    else
    {

    }
}

void
power_stop (mode_config_t *cfg)
{
    display_period_clear(5);
    battery_disable();
}


static void
voltage_display (void)
{
    if (0 == voltage)
    {
        voltage = battery_read_voltage();
    }
    else
    {
        voltage += battery_read_voltage();
        voltage /= 2;
    }
    printf("Voltage: %u\n\r", voltage);
    display_primary_clear(0);
    display_primary_string(1, "BAT");
    display_primary_number(0, voltage);
    display_primary_character(8, 'v');
}
