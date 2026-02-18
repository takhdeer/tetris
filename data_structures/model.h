#include "game_state.h"
#include "matrix.h"
#include "tetromino.h"
#include "types.h"

typedef struct
{
    GameState game_state;
    Matrix Matrix;
    Tetromino piece;
} Model;

extern Model initial_model;

