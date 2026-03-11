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
} Model;

extern Model initial_model;

/*
    Initializes the model for the game.
*/
void init_model(Model *model);

#endif