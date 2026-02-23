#include "cond.h"
#include "game_state.h"
#include "data_structures/tetromino.h"
#include "data_structures/synch.h"
#include <stdio.h>


int spawn_piece(Model *model) {
    // spawn peice
    init_tetromino(&model->piece, TETROMINO_I, 2);

    if (check_collision(&model->Matrix,&model->piece,model->piece.row,model->piece.col)) {
        model->game_state.is_game_over = 1;
        return 1;
    }

    return 0;
}