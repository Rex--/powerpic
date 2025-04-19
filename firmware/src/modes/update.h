/** @file update.h
 * 
 * This mode provides an easy way for the user to get into the bootloader.
 *
 * When the screen reads "Update", a press of the Adj button will display
 * "reboot". A confirmation press of any keypad key or the Mode button will
 * reboot the device, while another press of the Adj will cancel.
*/

#ifndef _update_h_
#define _update_h_

void            update_init  (void);
void            update_start (void);
signed char     update_run   (unsigned int event);
void            update_stop  (void);

mode_app_t update_mode = {
        "update",
        &update_init,
        &update_start,
        &update_run,
        &update_stop
};

#endif // _update_h_

// EOF //
