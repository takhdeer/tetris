#include "synch.h"

UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y) {
    UINT16 piece_row;
    UINT16 piece_col; 
    int board_col;
    int board_row;

    for (piece_row = 0; piece_row < 4; piece_row++) {
        for (piece_col = 0; piece_col <4; piece_col++) {

            if(get_cell(piece, piece_row, piece_col) == 0) {
                continue;   // skip over that cell
            }

            board_row = offset_y + piece_row;
            board_col = offset_x + piece_col;

            if (board_col < 0 || board_col >= MATRIX_COLS) {
                return 1;   // there is a collision
            }
            else if (board_row >= MATRIX_ROWS) {
                return 1;   // hit the bottom
            }

            else if (board_row < 0) {
                continue;   // allows for the spawning of blocks aboove screen
            }

            else if (board->cell[board_row][board_col] == 1) {
                return 1;   //locked piece already present
            }
        }
    }
    return 0;   // no collision
};

UINT16 can_move_down(Model *model) {
    return (!check_collision(&model->Matrix,&model->piece,model->piece.col, model->piece.row + 1));
}


void handle_tick (Model *model) {
    if (can_move_down(model)) {
        move_tetromino_down(&model->piece);
    }
    else {
        lock_piece(&model->Matrix, &model->piece);
    }
}