/** @file mode_manager.h
 * 
 * Manages switching between Mode Applications.
*/

#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

void mode_manager_init (void);
void mode_manager_thread (unsigned int event);
int mode_manager_config_update (void);
int mode_manager_get_tickrate (void);
int mode_manager_get_keymap (void);
#endif

/*** EOF ***/
