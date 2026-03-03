#ifndef COND_H
#define COND_H

#include "gstate.h"
#include "tetrom.h"
#include "synch.h"

/*
    Spawns a new piece at the top.
    By calling init_tetromino()
    Checks for collisions.
    Updates if the game is over.
*/
int handle_spawn_piece(Model *model);

/*
    Locks current piece to board.
    By calling lock_piece() from matrix.c
*/
void handle_piece_lock(Model *model);

/*
    Clears completed lines and updates the score.
    By calling clear_full_lines from matrix.c
*/
void handle_line_clear(Model *model);

#endif