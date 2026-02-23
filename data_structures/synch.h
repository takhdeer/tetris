#include "model.h"
#include "matrix.h"
#include "tetromino.h"
#include "../types.h"

/*
    Will check the specific tetrimino against a specific offset (for x,y) on the matrix board
    Will return 1 for a collision happened
    Will return 0 otherwise
*/
UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y);

/*
    Will just call check_collisions
    Helper function for handle_tick() to reduce cllutering 
 */
UINT16 can_move_down(Model *model);

/*
    Check's for collisions
    Will move the tetrimino down
    amd will call lock_piece()
*/
void handle_tick (Model *model);
