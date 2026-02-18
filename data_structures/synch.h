#include "model.h"
#include "matrix.h"
#include "tetromino.h"
#include "types.h"

UINT16 can_move_down(Model *model);

UINT16 check_collision(const Matrix *board, const Tetromino *piece, UINT16 offset_x, UINT16 offset_y);

void handle_tick (Model *model);

