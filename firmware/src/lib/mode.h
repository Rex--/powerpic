/** @file mode.h
 * Types and util procedures used by Mode Applications.
*/

#ifndef MODE_H
#define MODE_H

/**
 * Mode Configuration.
*/
typedef struct
{
    /** Mode's tickrate. */
    unsigned int tickrate;

    /** Mode's keymap. */
    unsigned char keymap;

    /** Mode's font. */
    unsigned char font;
} mode_config_t;

typedef void (*init_t)   (mode_config_t *cfg);
typedef void (*start_t)  (mode_config_t *cfg);
typedef void (*run_t)    (mode_config_t *cfg, unsigned int event);
typedef void (*stop_t)   (mode_config_t *cfg);

/**
 * Mode Application.
 * Each application objects hold an id, a mode config object, and pointers to
 * functions that implement the mode's functionality.
*/
typedef struct
{
    /** Mode application identifier.
     * TODO: Mode's ID might should be a fixed length array?
    */
    char *id;

    /** Mode application configuration.
     * This value is passed to each function. Should be initialized in the
     * mode's init() function.
    */
    // mode_config_t config;

    /** Mode application initialization function.
     * This function is called once every POR. This is the place to initilize
     * the blank config object that is passed in.
    */
    init_t init;

    /** Mode Application start function.
     * This function is called once everytime the mode is selected.
    */
    start_t start;

    /** Mode application run function.
     * This function is called for every event that happens.
     * Could be a tick, keypad event, etc.
    */
    run_t run;

    /** Mode application stop function.
     * This function is called once everytime the user selects another mode.
    */
    stop_t stop;
} mode_app_t;

void mode_init  (mode_app_t * mode_app, mode_config_t * mode_config);
void mode_start (mode_app_t * mode_app, mode_config_t * mode_config);
void mode_run   (mode_app_t * mode_app, mode_config_t * mode_config, unsigned int event);
void mode_stop  (mode_app_t * mode_app, mode_config_t * mode_config);


#if 0
int mode_add    (char *id, init_t init, start_t start, run_t run, run_t adj, stop_t stop);
int mode_total  (void);
int mode_init   (int mode);
int mode_start  (int mode);
int mode_run    (int mode, unsigned int event);
int mode_adj    (int mode, unsigned int event);
int mode_stop   (int mode);

int mode_tickrate (int mode);
int mode_keymap (int mode);
#endif

#endif

/*** EOF ***/
