/** @file post.h
 * 
 * Power on self tests after a reset event.
 *
 * This should be called after all libs are initialized, but before intializing
 * any modes. It does a quick self test that includes visual and audile
 * pass/fail notifications. 
*/

#ifndef POST_H
#define POST_H

/**
 * Do the POST.
*/
void
post_bios (void);

#endif

// EOF //
