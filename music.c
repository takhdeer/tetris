/* File: music.c - implementation of music (shorter version of the Tetris theme) */

/* Authors: Aaron, Takhdeer, Henry */

#include "music.h"
#include "psg.h"

#define NUM_NOTES 26
#define NOTE_REST 0

typedef struct {
    int pitch;      /* PSG tuning value*/
    int duration;   /* Duration in clock ticks (70 Hz) */
} Note;

static const Note song[NUM_NOTES] = 
{
    /* E B C D */
    {E_NOTE, 40}, {B_NOTE, 20}, {C_NOTE, 20}, {D_NOTE, 40}, 

    /* C B A */
    {C_NOTE, 20}, {B_NOTE, 20}, {A_NOTE, 36}, 

    /* A C E */
    {A_NOTE, 20}, {C_NOTE, 20}, {E_NOTE, 40}, 

    /* D C B */
    {D_NOTE, 20}, {C_NOTE, 20}, {B_NOTE, 36}, 

    /* B C D E */
    {B_NOTE, 20}, {C_NOTE, 20}, {D_NOTE, 40}, {E_NOTE, 40}, 

    /* C A A */
    {C_NOTE, 40}, {A_NOTE, 36}, {A_NOTE, 40},

    /* short pause */
    {NOTE_REST, 20}
};

static int current_note = 0;
static UINT32 note_time = 0;
static int gap_active = 0;
static UINT32 gap_time = 0;

/**
 * Begins the playing of the song by loading the data for the first note into the PSG.
 */
void start_music(void) {
    current_note = 0;
    note_time = 0;
    gap_active = 0;
    gap_time = 0;

    /* Set up channel A for music */
    enable_channel(CHANNEL_A, 1, 0); /* Tone on, noise off */
    set_volume(CHANNEL_A, 11);
    set_tone(CHANNEL_A, song[0].pitch);
}

/**
 * Advances to the next note of the song if necessary, as determined by the amount of
 * time elapsed since the previous call. The time elapsed is determined by the caller. It is
 * intended that this value is equal to the current value of the vertical blank clock, minus
 * its value when the function was last called.
 */
void update_music(UINT32 time_elapsed) {
    int next_note;
    int prev_note;

    /* gap delay if active */
    if (gap_active) {
        gap_time += time_elapsed;
        if (gap_time >= 3) {
            gap_active = 0;
            gap_time = 0;

            /* resume playing the note */
            set_tone(CHANNEL_A, song[current_note].pitch);
            set_volume(CHANNEL_A, 11);
        }
        return; /* Do not advance note yet */
    }

    note_time += time_elapsed;

    /* check if current note durration has elapsed */
    if (note_time >= song[current_note].duration) {
        note_time = 0;

        prev_note = current_note;

        current_note++;

        /* Loop back to beginning */
        if (current_note >= NUM_NOTES) {
            current_note = 0;
        }

        next_note = current_note;

        /* Play next note */
        if (song[current_note].pitch == NOTE_REST) {
            set_volume(CHANNEL_A, 0); /* Silence */
        }
        else {
            /* check if next note is the same as previous */
            if (song[next_note].pitch == song[prev_note].pitch && song[prev_note].pitch != NOTE_REST) {

                set_volume(CHANNEL_A, 0);
                gap_active = 1;
                gap_time = 0;
            }
            else {
            set_tone(CHANNEL_A, song[current_note].pitch);
            set_volume(CHANNEL_A, 10);
            }
        }
    }
}

/**
 * Stops music playback, silences channel A.
 */
void stop_music(void) {
    set_volume(CHANNEL_A, 0);
    enable_channel(CHANNEL_A, 0, 0);
}