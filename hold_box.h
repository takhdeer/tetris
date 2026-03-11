/* header file for hold_box.c */
/* author: Henry */

#ifndef HOLD_BOX_H
#define HOLD_BOX_H

#include "tetrom.h"

typedef struct {
    unsigned int contains; /* 0 = empty, 1 = contains a piece */
    Tetromino piece_held;  /* the held Tetromino piece */
} HoldBox;

/*----- Function: init_hold_box -----

 PURPOSE: initializes the HoldBox to an empty state

 INPUT: HoldBox variable

 OUTPUT: modifies HoldBox directly via pass by ref

*/
void init_hold_box(HoldBox *heldbox);

/*----- Function: hold_box_contains -----

 PURPOSE: check if HoldBox contains a Tetromino

 INPUT: HoldBox, accessed via pass by ref

 OUTPUT: 0 = empty, 1 = contains a piece

*/
unsigned int hold_box_contains(HoldBox *heldbox);

/*----- Function: hold_tetromino -----

 PURPOSE: hold the current tetromino piece

 INPUT: HoldBox and current Tetromino

 OUTPUT: returns previously held Tetromino (for proper piece swap behaviour)

*/
Tetromino hold_tetromino(HoldBox *heldbox, Tetromino *piece);

/*----- Function: release_tetromino -----

 PURPOSE: release the currently held Tetromino from the HoldBox

 INPUT: HoldBox

 OUTPUT: returns the previously held Tetromino

*/
Tetromino release_tetromino(HoldBox *heldbox);

#endif