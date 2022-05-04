/** @file test_mode.h
 * This Mode is for testing and development.
*/

#ifndef TEST_MODE_H
#define TEST_MODE_H

#define TEST_MODE_ID    "TEST"

void     test_mode_init      (mode_config_t *cfg);
void     test_mode_start     (mode_config_t *cfg);
void     test_mode_thread    (mode_config_t *cfg, unsigned int event);
void     test_mode_stop      (mode_config_t *cfg);

#define TEST_MODE  \
    TEST_MODE_ID, \
    test_mode_init, \
    test_mode_start, \
    test_mode_thread, \
    test_mode_stop

#endif

/*** EOF ***/
