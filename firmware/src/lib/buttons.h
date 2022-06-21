/** @file buttons.h
 * 
 * This library implements the functionality of the side buttons.
 * 
 * Each button generates an event on press an release.
 * Currently the MODE button press event is intercepted to switch modes and is
 * not passed on to the mode application. The MODE release event, however, is
 * passed to the mode application.
 * Both ADJ press and release events are sent to the mode.
 * 
 * TODO: Right now there is no debouncing implemented. Maybe we can use timer0.
*/

#ifndef _buttons_h_
#define _buttons_h_

#define BUTTON_MODE_PRESS       0x01
#define BUTTON_MODE_RELEASE     0x11

#define BUTTON_ADJ_PRESS        0x02
#define BUTTON_ADJ_RELEASE      0x12

/**
 * Initializes the buttons.
 * Events are generated on each press and release.
*/
void    buttons_init (void);

#endif

// EOF //
