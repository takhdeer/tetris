/* model.c */

#include "model.h"

Model initial_model;

void init_model(Model *model) {

    /* Initializing game state to starting values */
    model->game_state.score = 0;
    model->game_state.level = 1;
    model->game_state.lines_cleared = 0;
    model->game_state.is_game_over = 0;

    /* Initializing empty board */
    create_matrix(&model->Matrix);

    /* Spawn Tetromino piece at the top-center */
    init_tetromino(&model->piece, TETROMINO_I, 3);

    /* Initialize request fields */
    model->request_move_left = 0;
    model->request_move_right = 0;
    model->request_rotate = 0;
    model->request_soft_drop = 0;
    model->request_hold = 0;
}