/** @file buzzer.h
 * 
 * This library implements functionality of the piezo buzzer on the watch.
 * 
 * It allows you to play a variable length tone with a given frequency and volume.
*/

#ifndef _buzzer_h_
#define _buzzer_h_


/**
 * Initialize the buzzer for use.
*/
void
buzzer_init (void);

/**
 * Play a tone.
 * 
 * @param[in]   frequency   The frequency of the tone in hertz.
 * @param[in]   volume      The volume of the tone as a percentage.
 * @param[in]   duration    The duration of the tone in ms.
*/
void
buzzer_tone (unsigned int frequency, unsigned char volume, unsigned int duration);


#endif

// EOF //
