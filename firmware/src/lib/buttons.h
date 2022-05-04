/** @file buttons.h
 * Button library.
*/

#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTON_EVENT            0x0b    /**< Base Button Event. */

#define BUTTON_MODE_PRESS       0x01    /**< Mode button pressed event. */
#define BUTTON_MODE_RELEASE     0x11    /**< Mode button released event. */

#define BUTTON_ADJ_PRESS        0x02    /**< Adj button pressed event. */
#define BUTTON_ADJ_RELEASE      0x12    /**< Adj button released event. */

void buttons_enable (void);

#endif