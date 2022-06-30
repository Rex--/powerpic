/** @file test.h
 * 
 * This mode application implements various tests.
 * 
 * Currently, it's just a stub to showcase the mode application interface.
*/

#ifndef _test_h_
#define _test_h_

// Define our mode functions
void            test_init  (void);
void            test_start (void);
signed char     test_run   (unsigned int event);
void            test_stop  (void);


// Define our mode application object.
// Although these values are constant, we don't declare it as such to make sure
// it's assigned a place in RAM for faster access.
// We declare this static so the initilization values only get set once at
// startup. This hopefully also speeds up access time.
static mode_app_t test_mode = {
        "test",
        &test_init,
        &test_start,
        &test_run,
        &test_stop
};

#endif

// EOF //
