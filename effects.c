/* effects.c - sound effect implementations */
/* Authors: Henry */

#include "effects.h"
#include <osbind.h>

/* local helper to read 70Hz timer for busy wait */
static long read_timer() {
    long val;
    long old_ssp;
    old_ssp = Super(0);
    val = *((long *)0x462);
    Super(old_ssp);
    return val;
}

void play_effect_hold() {
    long start;

    set_tone(CHANNEL_B, 500);           /* as of 6:28pm 3/29/26, sound effect = low pitch */
    enable_channel(CHANNEL_B, 1, 0);
    set_envelope(9, 800);
    write_psg(9, 16);

    /* Busy Wait ~10 Ticks */
    start = read_timer();
    while ( (read_timer() - start) < 10);

    stop_sound();
}

void play_effect_line_clear() {
    long start;

    /* 1st tone */
    set_tone(CHANNEL_B, 300);
    enable_channel(CHANNEL_B, 1, 0);
    set_envelope(9, 600);
    write_psg(9, 16);

    start = read_timer();
    while ( (read_timer() - start) < 6);

    /* 2nd tone */
    set_tone(CHANNEL_B, 150);
    set_envelope(9, 500);
    write_psg(9, 16);

    start = read_timer();
    while ((read_timer() - start) < 6);

    stop_sound();

}

void play_effect_tetris_clear() {
    long start;

    /* Channel B: tone sweep step 1 - low */
    /* Channel C: noise burst kick */
    set_tone(CHANNEL_B, 400);
    enable_channel(CHANNEL_B, 1, 0);
    set_envelope(9, 700);
    write_psg(9, 16);

    set_noise(12);
    enable_channel(CHANNEL_C, 0, 1);
    set_envelope(9, 700);
    write_psg(10, 16);

    start = read_timer();
    while ((read_timer() - start) < 5);

    /* Step 2 - mid pitch, noise fading */
    set_tone(CHANNEL_B, 250);
    set_envelope(9, 600);
    write_psg(9, 16);

    start = read_timer();
    while ((read_timer() - start) < 5);

    /* Step 3 - high pitch finish */
    set_tone(CHANNEL_B, 120);
    set_envelope(9, 500);
    write_psg(9, 16);

    start = read_timer();
    while ((read_timer() - start) < 7);

    stop_sound();
}

