/* model.c */

#include "model.h"
#include <osbind.h>

Model initial_model;

void init_model(Model *model) {

    UINT32 wait;

    /* Initializing game state to starting values */
    model->game_state.score = 0;
    model->game_state.level = 1;
    model->game_state.lines_cleared = 0;
    model->game_state.is_game_over = 0;
    model->hold_used = 0;

    /* Initializing empty board + grab bag */
    create_matrix(&model->Matrix);


    wait = get_time();
    while (get_time() - wait < 3);  /* wait 3 ticks */
    init_bag(model);

    /* SPAWN Tetromino piece at the top-center */
    init_tetromino(&model->piece, next_piece_from_bag(model), 3);

    /* Initialize request fields */
    model->request_move_left = 0;
    model->request_move_right = 0;
    model->request_rotate = 0;
    model->request_soft_drop = 0;
    model->request_hold = 0;

    model->request_hard_drop = 0;

    /* Initialize gravity counter */
    model->gravity_counter = 0;

    /* Initialize redraw flags */
    model->redraw_matrix = 1;
    model->redraw_next_box = 1;
    model->redraw_hold_box = 1;
    model->redraw_score = 1;
}

UINT8 check_game_over (Model *model) {
    int c;
    for (c = 0; c < MATRIX_COLS; c++) {
        if (model->Matrix.cell[0][c] == 1 ||
            model->Matrix.cell[1][c] == 1) 
        return 1; 
    }
    return 0;
}

UINT32 get_time() {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = Super(0);
    timeNow = *timer;
    Super(old_ssp);

    return timeNow;
}

void init_bag(Model *model) {
    /* variables related to Fisher-Yates Shuffle*/
    int i;
    int j;
    int temp_bag;
    UINT32 seed;

    /* FILL: grab bag wil 7 tetrmomino types*/
    model->bag[0] = TETROMINO_I;
    model->bag[1] = TETROMINO_O;
    model->bag[2] = TETROMINO_T;
    model->bag[3] = TETROMINO_Z;
    model->bag[4] = TETROMINO_S;
    model->bag[5] = TETROMINO_L;
    model->bag[6] = TETROMINO_J;

    seed = get_time();
    
    /* Fisher-Yates Shuffle Alg. */
    for (i = 6; i >= 1; i--) {
        seed = seed * 1664525 + 1013904223;

        j = (int)(seed % (i + 1)); /* based on the clock, get a RANDOM TETROMINO from grab bag*/
                                                        /* range is 0 to i INCLUSIVE */

        temp_bag = model->bag[i];           /* create temp bag */
        model->bag[i] = model->bag[j];
        model->bag[j] = temp_bag;
    }

    model->bag_index = 0;
}

int next_piece_from_bag(Model *model) {
    int next_piece = model->bag[model->bag_index];
    model->bag_index++;

    /* check if grab bag is empty */
    if (model->bag_index == 7) {
        init_bag(model);    /* if empty, BAG REFILL! */
    }

    return next_piece;
}

int peek_bag(Model *model) {
    return model->bag[model->bag_index];
}


