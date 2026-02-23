#include "game_state.h"
#include "data_structures/matrix.h"
#include "data_structures/tetromino.h"
#include "types.h"

typedef struct
{
    GameState game_state;
    Matrix Matrix;
    Tetromino piece;
} Model;

extern Model initial_model;

void init_model(Model *model);
