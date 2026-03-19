/* synch.c */

#include "synch.h"

UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y) {
    UINT16 piece_row;
    UINT16 piece_col;
    int board_col;
    int board_row;

    for (piece_row = 0; piece_row < 4; piece_row++) {
        for (piece_col = 0; piece_col < 4; piece_col++) {

            if (get_cell(piece, piece_row, piece_col) == 0) {
                continue; /* skip empty cell */
            }

            board_row = offset_y + piece_row;
            board_col = offset_x + piece_col;

            if (board_col < 0 || board_col >= MATRIX_COLS) {
                return 1; /* out of bounds horizontally */
            }
            else if (board_row >= MATRIX_ROWS) {
                return 1; /* hit the bottom */
            }
            else if (board_row < 0) {
                continue; /* allows spawning above screen */
            }
            else if (board->cell[board_row][board_col] == 1) {
                return 1; /* locked piece already present */
            }
        }
    }
    return 0; /* no collision */
}

UINT16 can_move_down(Model *model) {
    return (!check_collision(&model->Matrix, &model->piece, model->piece.col, model->piece.row + 1));
}

void handle_tick(Model *model) {
    int old_lines;

    if (can_move_down(model)) {
        move_tetromino_down(&model->piece);
    }
    else {
        /* Lock piece to matrix */
        lock_piece(&model->Matrix, &model->piece);
        model->redraw_matrix = 1;

        /* Clearing lines */
        old_lines = model->game_state.lines_cleared;
        model->game_state.lines_cleared = clear_full_lines(&model->Matrix, model->game_state.lines_cleared);

        /*If lines were cleared, matrix + score change*/
        if (model->game_state.lines_cleared > old_lines) {
            model->redraw_matrix = 1;
            model->redraw_score = 1;
        }

        /* Spawn new piece from bag */
        init_tetromino(&model->piece, next_piece_from_bag(model), 3);
        init_next_box(&model->nbox, peek_bag(model));
        model->redraw_next_box = 1;

        /* Check if the game should end*/
        if(check_collision(&model->Matrix, &model->piece, &model->piece.row, &model->piece.col)) {
            model->game_state.is_game_over = 1;
        }
    }
}