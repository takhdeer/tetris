/* effects.h file - sound effects using low level psg.c subroutines */
/* Authors: Henry */

#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"
#include "psg.h"

/* Sound Effects */

/*------ Function: play_effect_hold ----

PURPOSE: ASYNCHRONOUS Sound Effect for Holding a Tetromino

INPUT: non

Output: non

*/
void play_effect_hold();

/*------ Function: play_effect_line_clear ----

PURPOSE: SYNCHRONOUS Sound Effect for a Line Clear

INPUT: non

Output: non

*/
void play_effect_line_clear();

/*------ Function: play_effect_tetris_clear ----

PURPOSE: SYNCHRONOUS Sound Effect for a a Tetris line clear (woosh)

INPUT: non

Output: non

*/

void play_effect_tetris_clear();

#endif