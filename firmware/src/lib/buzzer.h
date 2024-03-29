/** @file buzzer.h
 * 
 * This library implements functionality of the piezo buzzer on the watch.
 * 
 * It allows you to play a variable length tone with a given frequency and
 * volume. You can also play songs in the RTTTL ringtone format, although the
 * only one I've found that sounds good is the Super Mario Bros theme.
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


/**
 * Play a tone 2.
 * 
 * This function does not block like _tone, however time is only counted if the
 * cpu is awake. The cpu sleeps most of the time so tones can go on alot longer
 * than expected and are not always on. A fix for this would be to keep the pic
 * awake when we are playing a tone.
 * 
 * We currently use the same timer to generate interrupts as we do to drive the
 * buzzer pwm. A side effect of this is we only count time when the buzzer is
 * actually sounding, a drawback is above.
*/
void
buzzer_tone2 (unsigned int frequency, unsigned char volume, unsigned int duration);

/**
 * Play a song defined as an RTTTL string.
 * 
 * This will block for the entire song.
 * Currently this might not handle all of the various formats, but it seems to
 * handle most standard songs. The logic to determine the pitch frequency is
 * based on PlayRTTL: <https://github.com/ArminJo/PlayRtttl>.
 * 
 * @param[in]   rtttl_str   A string representing a song in RTTTL format.
*/
void
buzzer_play_rtttl(char * rtttl_str);


#endif

// EOF //
