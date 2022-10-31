/** @file buzzer.c
 * 
 * This library implements functionality of the piezo buzzer on the watch.
*/

#include <xc.h>
#include <math.h>

#include "drivers/pwm.h"
#include "lib/isr.h"
#include "lib/logging.h"

#include "lib/buzzer.h"


#undef  LOG_TAG
#define LOG_TAG "lib.buzzer"


static volatile unsigned int tone_duration = 0;
static volatile unsigned int played_duration = 0;

static void buzzer_isr (void);

void
buzzer_init (void)
{
    // Initialize our pwm driver. This configures it for our buzzer.
    pwm_init();

    // Register ISR
    // Should this be done only when we are playing a tone?
    isr_register(4, _PIR4_TMR2IF_MASK, &buzzer_isr);
}

void
buzzer_tone (unsigned int frequency, unsigned char volume, unsigned int duration)
{
    pwm_freq_set(frequency);

    pwm_duty_set(volume/2);

    pwm_enable();

    // This delay loop blocks for the given duration. It would be preferrable
    // to use an interrupt. Perhaps use the pwm timer interrupt? Would have to
    // service it at 1/4 the frequency, but it might be better than sitting
    // around twiddling our cycles.
    //
    int delay_count = 0;
    while (delay_count++ < duration)
    {
        __delay_us(990);
    }

    pwm_disable();
}


void
buzzer_tone2 (unsigned int frequency, unsigned char volume, unsigned int duration)
{
    pwm_freq_set(frequency);

    pwm_duty_set(volume/2);

    // Duration of 1 interrupt in ms
    float timer_duration = 1.0F/((frequency / 3.0F) / 1000);
    
    // Figure out how many interrupt we need to delay
    tone_duration = (unsigned int)lroundf(duration / timer_duration);

    played_duration = 0;

    LOG_DEBUG("Timer Duration: %f", timer_duration);
    LOG_DEBUG("Tone Duration: %i", tone_duration);
    
    // enable interrupt
    PIE4bits.TMR2IE = 1;

    // start tone
    pwm_enable();
}

#define IS_ALPHA(c) (('A' <= (c) && (c) <= 'Z') || ('a' <= (c) && (c) <= 'z'))
#define IS_NUM(c)   ('0' <= (c) && (c) <= '9')

#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

const int Notes[] = { NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7,
    NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7 };

void
buzzer_play_rtttl(char * rtttl_str)
{
    unsigned char song_title[10];
    unsigned int char_index = 0;

    // Get first 10 characters of title
    while (rtttl_str[char_index] != ':')
    {
        if (char_index < sizeof(song_title))
        {
            song_title[char_index] = rtttl_str[char_index];
        }
        char_index++;
    }
    char_index++;   // Skip colon

    // Get note defaults
    unsigned char song_def_duration = 4;
    unsigned char song_def_octave = 6;
    unsigned int song_def_bpm = 63;
    while (rtttl_str[char_index] != ':')
    {
        switch (rtttl_str[char_index])
        {
        case 'd':
            char_index += 2;    // Consume 'b='
            // Set duration
            song_def_duration = 0;
            while (IS_NUM(rtttl_str[char_index]))
            {
                song_def_duration = (song_def_duration * 10) \
                    + (rtttl_str[char_index++] - 48);
            }
        break;

        case 'o':
            char_index += 2;    // Consume 'o='
            // Set octave
            song_def_octave = rtttl_str[char_index++] - 48;
        break;

        case 'b':
            char_index += 2;    // Consume 'b='
            // Set bpm
            song_def_bpm = 0;
            while (IS_NUM(rtttl_str[char_index]))
            {
                song_def_bpm = (song_def_bpm * 10) \
                    + (rtttl_str[char_index++] - 48);
            }
        break;
        
        default:
            // Unrecognized command, we'll skip it in a few loops.
            char_index++;
        break;
        }
    }
    char_index++;   // Skip colon

    // Print info
    LOG_INFO("Now Playing: %s", song_title);
    LOG_DEBUG("in octave: %u", song_def_octave);
    LOG_DEBUG("note len: %u", song_def_duration);
    LOG_DEBUG("BPM: %u", song_def_bpm);

    // Needed values
    unsigned int note_length_ms = (unsigned)((60000U/song_def_bpm)*4);
    unsigned char note_duration;
    unsigned char note_pitch;
    unsigned char note_sharp;
    unsigned char note_octave;

    // Play notes until end of string is reached
    while (rtttl_str[char_index])
    {
        // Check if duration is present
        if (IS_NUM(rtttl_str[char_index]))
        {
            // Get duration of note
            note_duration = rtttl_str[char_index++] - 48;
            if (IS_NUM(rtttl_str[char_index]))
            {
                note_duration = (note_duration * 10) \
                    + (rtttl_str[char_index++] - 48);
            }
        }
        else
        {
            // Go with default duration
            note_duration = song_def_duration;
        }

        // Get the pitch
        // note_pitch = rtttl_str[char_index++];
        switch (rtttl_str[char_index++])
        {
            case 'c':
            case 'C':
                note_pitch = 0;
            case 'd':
            case 'D':
                note_pitch = 2;
            break;

            case 'e':
            case 'E':
                note_pitch = 4;
            break;

            case 'f':
            case 'F':
                note_pitch = 5;
            break;

            case 'g':
            case 'G':
                note_pitch = 7;
            break;

            case 'a':
            case 'A':
                note_pitch = 9;
            break;

            case 'b':
            case 'h':
            case 'B':
            case 'H':
                note_pitch = 11;
            break;

            default:
                note_pitch = 50;
            break;
        }
        // Check for sharps
        if (rtttl_str[char_index] == '#')
        {
            note_sharp = '#';
            note_pitch++;
            char_index++;
        }
        else
        {
            note_sharp = ' ';
        }

        // Check for period for special-duration
        if (rtttl_str[char_index] == '.')
        {
            // Increase duration by 50%
            note_duration += note_duration / 2;
            char_index++;   // Consume '.'
        }

        // Check for octave specifier
        if (IS_NUM(rtttl_str[char_index]))
        {
            note_octave = rtttl_str[char_index++] - 48;
        }
        else
        {
            // Go with default octave
            note_octave = song_def_octave;
        }

        // Skip comma
        if (rtttl_str[char_index] == ',')
        {
            char_index++;   // Consume ','
        }

        // Play note
        if (note_pitch < 13)
        {
            unsigned int note_freq = (unsigned)((Notes[note_pitch] >> (7 - note_octave)) + 1);
            unsigned int pause_duration = (unsigned)((note_length_ms / note_duration) >> 4);
            unsigned int tone_duration = (unsigned)((note_length_ms / note_duration) - pause_duration);
            LOG_DEBUG("Playing note: %uhz for %ums, pause %ums", note_freq, tone_duration, pause_duration);
            buzzer_tone(note_freq, 25, tone_duration);
            int delay_count = 0;
            while (delay_count++ < pause_duration)
            {
                __delay_us(990);
            }
        }
        else
        {
            // Rest
            LOG_DEBUG("Resting for %ums", (note_length_ms / note_duration));
            int delay_count = 0;
            while (delay_count++ < (note_length_ms / note_duration))
            {
                __delay_us(990);
            }
        }
    }
}


static void
buzzer_isr (void)
{
    // This gets called at ~1/3 the PWM frequency

    if (tone_duration <= played_duration)
    {
        // stop tone
        // pwm_disable();
        PWM4CONbits.PWM4EN = 0;

        // Disable interrupt
        PIE4bits.TMR2IE = 0;
    }
    else
    {
        played_duration++;
    }

    // Clear interrupt flag
    PIR4bits.TMR2IF = 0;
}


// EOF //
