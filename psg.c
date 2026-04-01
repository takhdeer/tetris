/*  psg.c - PSG hardware control implementation
    Authors: Aaron, Takhdeer, Henry
*/

#include <osbind.h>
#include "psg.h"

#define PSG_REG_SELECT 0xFF8800
#define PSG_REG_WRITE 0xFF8802

/* PSG register numbers */
#define PSG_FINE_TUNE_A     0
#define PSG_COARSE_TUNE_A   1
#define PSG_FINE_TUNE_B     2
#define PSG_COARSE_TUNE_B   3
#define PSG_FINE_TUNE_C     4
#define PSG_COARSE_TUNE_C   5

#define PSG_NOISE_TUNE      6
#define PSG_MIXER           7
#define PSG_VOLUME_A        8
#define PSG_VOLUME_B        9
#define PSG_VOLUME_C        10

/**
 * Writes the given byte value (0-255) to the given PSG register (0-15). This is a helper routine to be used by the other functions in this module.
 */
void write_psg(int reg, UINT8 val)
{
    volatile char *PSG_reg_select = (volatile char *)PSG_REG_SELECT;
    volatile char *PSG_reg_write = (volatile char *)PSG_REG_WRITE;
    long old_ssp;

    if (reg < 0 || reg > 15) {
        return;
    }

    old_ssp = Super(0);
    *PSG_reg_select = reg;
    *PSG_reg_write = val;
    Super(old_ssp);
}

/**
 * Useful for testing purposes, but unlikely to be used in either the music or effects modules.
 */
UINT8 read_psg(int reg)
{
    volatile char *PSG_reg_select = (volatile char *)PSG_REG_SELECT;
    UINT8 value;
    long old_ssp;

    if (reg < 0 || reg > 15) {
        return 0;
    }

    old_ssp = Super(0);
    *PSG_reg_select = reg;
    value = *PSG_reg_select;
    Super(old_ssp);

    return value;
}

/**
 * Loads the tone registers (coarse and fine) for the given channel (0=A, 1=B, 2=C) with the given 12-bit tuning.
 */
void set_tone(int channel, int tuning)
{
    int fine_reg;
    int coarse_reg;

    if (channel < 0 || channel > 2) {
        return;
    }

    if (tuning < 0 || tuning > 4095) {
        return;
    }

    fine_reg = PSG_FINE_TUNE_A + (channel * 2);
    coarse_reg = PSG_COARSE_TUNE_A + (channel * 2);

    write_psg(fine_reg, tuning & 0xFF);
    write_psg(coarse_reg, (tuning >> 8) & 0x0F);
}

/**
 * Loads the volume register for the given channel.
 */
void set_volume(int channel, int volume)
{
    if (channel < 0 || channel > 2) {
        return;
    }

    if (volume < 0 || volume > 15) {
        return;
    }

    write_psg(PSG_VOLUME_A + channel, volume);
}

/**
 * Turns the given channel’s tone/noise signals on/off (0=off, 1=on).
 */
void enable_channel(int channel, int tone_on, int noise_on)
{
    UINT8 mixer;
    UINT8 mask;

    if (channel < 0 || channel > 2) {
        return;
    }

    mixer = read_psg(PSG_MIXER);

    /* Mixer: 0=enabled, 1=disabled */
    mask = (1 << channel) | (1 << (channel + 3));
    mixer |= mask; /* disable both */

    if (tone_on) {
        mixer &= ~(1 << channel); /* '~' flips bits*/
    }

    if (noise_on) {
        mixer &= ~(1 << (channel + 3));
    }

    write_psg(PSG_MIXER, mixer);
}

/**
 * Silences all PSG sound production.
 */
void stop_sound()
{
    write_psg(PSG_VOLUME_A, 0);
    write_psg(PSG_VOLUME_B, 0);
    write_psg(PSG_VOLUME_C, 0);
    write_psg(PSG_MIXER, 0xFF);
}

void set_noise(int tuning) {
    /* guard clause: invalid tuning level given*/
    if (tuning < 0 || tuning > 31) {
        return;
    }

    /* else, it's a valid tuning level */
    write_psg(PSG_NOISE_TUNE, tuning);
}

void set_envelope(int shape, unsigned int sustain) {
    /* guard clause: invalid shape */
    if (shape < 0 || shape > 15) {
        return;
    }

    /* else, it's a valid shape */

    write_psg(11, sustain & 0xFF);          /* envelope fine */
    write_psg(12, (sustain >> 8) & 0xFF);   /* envelope coarse */
    write_psg(13, shape);                   /* envelope shape */
}
