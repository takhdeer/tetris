#include "game_state.h"
#include "data_structures/tetromino.h"
#include "data_structures/synch.h"


#include <stdio.h>

/*
    Spawns a new peice at the top
    By calling init_tetrimino()
    Checks for collisions
    Updates if the game is over
 */

int handle_spawn_piece(Model *model);

/*
    Locks current peice to board
    By calling lock_piece() from matrix.c

*/
void handle_piece_lock(Model *model);

/*
    Cleares completed lines and updates the score
    By calling clear_full_lines from matrix.c
    Returns # of lines cleared
*/
void  handle_line_clear(Model *model);
