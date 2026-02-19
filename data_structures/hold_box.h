// header file for hold_box.c
// author: Henry

#ifndef HOLD_BOX_H
#define HOLD_BOX_H

#include "tetromino.h"

typedef struct {
    unsigned int contains;  // HoldBox boolean flag detects if contains a piece, 0 = false, nonZero = true
    Tetromino piece_held;   // HoldBox holds a the specific Tetromino piece
} HoldBox;

// behaviour functions

/*----- Function: hold_box_contains -----

 PURPOSE: check if HoldBox actually contains a Tetromino or not

 INPUT: HoldBox, accessed directly via pass by ref

 OUTPUT: a "boolean" flag i.e a unsigned integer, 0 = false and 1 = true

*/  

unsigned int hold_box_contains(HoldBox *heldbox);

/*----- Function: hold_tetromino -----

 PURPOSE: primary use for HoldBox, via a specific keystroke from user, hold the current tetromino piece

 INPUT: HoldBox and current Tetromino

 OUTPUT: returns previously held Tetromino (for proper piece swap behaivour)

*/ 

Tetromino hold_tetromino(HoldBox *heldbox, Tetromino *piece);

/*----- Function: release_tetromino  -----

 PURPOSE: via specific keystroke from user, release the current held Tetromino piece inside HoldBox

 INPUT: HoldBox and current Tetromino

 OUTPUT: returns previously held Tetromino (for proper piece swap behaviour)

*/ 

Tetromino release_tetromino(HoldBox *heldbox, Tetromino *piece);

#endif
