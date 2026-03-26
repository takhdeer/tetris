/* psg. h file - low level PSG control */
/* Authors: Aaron, Takhdeer, Henry */

#ifndef PSG_H
#define PSG_H

#include "types.h"

/* Musical notes */
#define A_NOTE 254
#define B_NOTE 226
#define C_NOTE 214
#define D_NOTE 190
#define E_NOTE 170

/* PSG channels */
#define CHANNEL_A 0
#define CHANNEL_B 1
#define CHANNEL_C 2

/* Music Functions */
void write_psg(int reg, UINT8 val);
UINT8 read_psg(int reg);
void set_tone(int channel, int tuning);
void set_volume(int channel, int volume);
void enable_channel(int channel, int tone_on, int noise_on);
void stop_sound();

/* TO DO: Sound Effect Functions */

#endif