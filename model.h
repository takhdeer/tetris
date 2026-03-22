/* model.h */

#ifndef MODEL_H
#define MODEL_H

#include "gstate.h"
#include "matrix.h"
#include "tetrom.h"
#include "hold_box.h"
#include "next_box.h"
#include "types.h"

typedef struct {
    GameState game_state;
    Matrix    Matrix;
    Tetromino piece;
    NextBox   nbox;
    HoldBox   hbox;

    /* for input */
    int request_move_left;
    int request_move_right;
    int request_rotate;
    int request_soft_drop;
    int request_hold;

    /* for grab bag spawning system */
    int bag[7];
    int bag_index;

    int request_hard_drop;

    int gravity_counter;

    /* redraw flags (tracking what needs to be redrawn) */
    int redraw_matrix;
    int redraw_next_box;
    int redraw_hold_box;
    int redraw_score;
} Model;

extern Model initial_model;

/*
    Initializes the model for the game.
*/
void init_model(Model *model);

/*------ Function: check_game_over ----

PURPOSE: checks if the top 2 rows of the matrix are filled 

INPUT: Game Model

Output: Returns 1 for game is over
        Returns 2 for game is NOT over

*/
UINT8 check_game_over (Model *model);

/*----- Function: get_time -----

 PURPOSE: gets the time of the 70 Hz clock at that clock tick

 INPUT: Game Model

 OUTPUT: Clock Time

*/

UINT32 get_time();

/*----- Function: init_bag -----

 PURPOSE: initalize the grab bag for tetrominos in spawn system

 INPUT: current game state of model as ptr

 OUTPUT: no output but the grab bag is initialized (called on game launch)

*/

void init_bag(Model *model);

/*----- Function: next_piece_from_bag -----

 PURPOSE: returns the next Tetromino type from grab bag,
          REFILLS and RESHUFFLES (via Fisher-Yates Alg.) the grab bag when all 7 have been used

 INPUT: current game state of model as ptr

 OUTPUT: returns Tetromino type (int) i.e TETROMINO_I, TETROMIN_S, etc

*/

int next_piece_from_bag(Model *model);

/*----- Function: peek_bag -----

 PURPOSE: literally peek at the next piece in grab bag WITHOUT consuming it
          
 INPUT: current game state of model as ptr

 OUTPUT: returns Tetromino type (int) but NOT consumed

*/

int peek_bag(Model *model);

#endif