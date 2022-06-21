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

/**
 * Mode application configuration.
 * This value is passed to each function. Should be initialized in the
 * mode's init() function.
*/
typedef struct
{
    /**
     * Mode tickrate.
     * The tickrate is specified in milliseconds. Anything above 65,535ms is
     * only configurable down to the second (1,000 ms). The max tickrate
     * available is 65,535 seconds (65,535,000 ms)
    */
    long tickrate;

    /**
     * Mode's keymap.
     * This specifies the keymap to use for keypad events.
    */
    unsigned char keymap;

} mode_config_t;


typedef void (*init_t)   (void);
typedef void (*start_t)  (void);
typedef void (*run_t)    (unsigned int event);
typedef void (*stop_t)   (void);


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
     * Pointer to the mode's config object.
    */
    mode_config_t *config;

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
