#include "cond.h"
#include "gstate.h"
#include "tetrom.h"
#include "synch.h"
#include <stdio.h>

int handle_spawn_piece(Model *model) {
    /* spawn piece */
    init_tetromino(&model->piece, TETROMINO_I, 2);

    if (check_collision(&model->Matrix, &model->piece, model->piece.row, model->piece.col)) {
        model->game_state.is_game_over = 1;
        return 1;
    }

    return 0;
}

void handle_piece_lock(Model *model) {
    lock_piece(&model->Matrix, &model->piece);
    handle_line_clear(model);
}

void handle_line_clear(Model *model) {
    UINT16 lines_cleared = clear_full_lines(&model->Matrix);
    if (lines_cleared > 0) {
        update_state(&model->game_state, lines_cleared);
    }
}