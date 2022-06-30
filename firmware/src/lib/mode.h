/** @file mode.h
 * 
 * This library implements the types and procedures used by Mode applications.
 * 
 * Each mode app should implement the prototyped functions and data structures.
 * The basic interface of the mode is split into 4 call groups:
 * 
 *  Initialization      This function is called once when the device powers on.
 *                      Default tickrates, keymaps, and other values should be
 *                      configured here.
 *                      TODO: Some sort of  persistent EEPROM storage.
 * 
 *  Startup             This function is called every time the mode application
 *                      has been selected.
 * 
 *  Run                 This function is called for any events that occur.
 * 
 *  Stop                This function is called when the mode application loses
 *                      focus.
*/

#ifndef _mode_h_
#define _mode_h_


typedef void        (*init_t)   (void);
typedef void        (*start_t)  (void);
typedef signed char (*run_t)    (unsigned int event);
typedef void        (*stop_t)   (void);
typedef void        (*daemon_t) (unsigned int event);


/**
 * Mode Application.
 * Each application object holds an id, a mode config object, and pointers to
 * functions that implement the mode's functionality.
*/
typedef struct
{
    /** 
     * Mode application identifier.
     * TODO: Mode's ID might should be a fixed length array?
    */
    const char *id;

    /**
     * Mode application initialization function.
     * This function is called once every POR.
    */
    init_t init;

    /**
     * Mode Application start function.
     * This function is called once everytime the mode is selected.
    */
    start_t start;

    /**
     * Mode application run function.
     * This function is called for every event that happens.
     * Could be a tick, keypad event, etc.
    */
    run_t run;

    /**
     * Mode application stop function.
     * This function is called once everytime the user selects another mode.
    */
    stop_t stop;

    /**
     * Mode application daemon function.
     * This function is called for every single event that occurs, even when
     * the mode is not active.
     * 
     * TODO: Perhaps it'd be preferrable to 'filter' the events that are sent
     * to each daemon.
    */
    daemon_t daemon;

} mode_app_t;


/**
 * Initialize mode applications and their config.
 * This functions also selects the first mode and starts it.
*/
void
mode_init (void);

/**
 * Switch to the next mode application.
 * This will stop the current mode and start the next mode in the list, looping
 * back to the beginning if the end of the list is reached
*/
void
mode_next (void);


/**
 * Runs the currently selected mode, passing it all events.
 * This will loop through all the queued events, since last time this function
 * was called, and ass each to the mode's run function.
*/
void
mode_thread (void);



#endif

// EOF //
