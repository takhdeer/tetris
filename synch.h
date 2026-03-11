/* synch.h */

#ifndef SYNCH_H
#define SYNCH_H

#include "model.h"
#include "matrix.h"
#include "tetrom.h"
#include "types.h"

/*
    Purpose: Determines if a tetromino collides with board boundaries
             or locked pieces.

    Input:
       - board: Pointer to the game board matrix
       - piece: Pointer to the tetromino being tested
       - offset_x: The column position to test
       - offset_y: The row position to test

    Output:
       Returns 1 if collision detected.
       Returns 0 if no collision.
*/
UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y);

/*
    Helper function for handle_tick().
    Returns 1 if piece can move down, 0 if it cannot.
*/
UINT16 can_move_down(Model *model);

/*
    Purpose: Synchronous event handler for clock tick (gravity).

    Input:
       - model: Pointer to the complete game model

    Output: None (modifies model state directly)
*/
void handle_tick(Model *model);

#endif