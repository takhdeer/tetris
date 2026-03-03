/* implementations of functions found in "matrix.h" header file */
/* vocabulary:                                                    */
/*   - empty cell = 0                                             */
/*   - filled cell = 1                                            */
/* author: Henry                                                  */

#include "matrix.h"
#include "tetrom.h"
#include "types.h"
#include <stdio.h>

void create_matrix(Matrix *gameGrid) {
    int r, c;
    for (r = 0; r < MATRIX_ROWS; r++) {
        for (c = 0; c < MATRIX_COLS; c++) {
            gameGrid->cell[r][c] = 0;
        }
    }
}

void output_matrix(Matrix *gameGrid) {
    int r, c;
    for (r = 0; r < MATRIX_ROWS; r++) {
        for (c = 0; c < MATRIX_COLS; c++) {
            printf("%i", gameGrid->cell[r][c]);
        }
        printf("\n");
    }
}

UINT16 clear_full_lines(Matrix *gameGrid) {
    UINT16 lines_cleared = 0;
    UINT16 filled_cell_count = 0;
    int r, c;

    for (r = 0; r < MATRIX_ROWS; r++) {
        for (c = 0; c < MATRIX_COLS; c++) {
            if (gameGrid->cell[r][c] == 1) {
                filled_cell_count++;
            }
        }
        if (filled_cell_count == MATRIX_COLS) {
            lines_cleared++;
            for (c = 0; c < MATRIX_COLS; c++) {
                gameGrid->cell[r][c] = 0;
            }
        }
        filled_cell_count = 0;
    }

    drop_rows_down(gameGrid);

    return lines_cleared;
}

void drop_rows_down(Matrix *gameGrid) {
    int write = MATRIX_ROWS - 1;
    int read, c;
    unsigned char is_empty;

    for (read = MATRIX_ROWS - 1; read >= 0; read--) {
        is_empty = 1;
        for (c = 0; c < MATRIX_COLS; c++) {
            if (gameGrid->cell[read][c] != 0) {
                is_empty = 0;
                break;
            }
        }

        if (!is_empty) {
            for (c = 0; c < MATRIX_COLS; c++) {
                gameGrid->cell[write][c] = gameGrid->cell[read][c];
            }
            write--;
        }
    }

    for (; write >= 0; write--) {
        for (c = 0; c < MATRIX_COLS; c++) {
            gameGrid->cell[write][c] = 0;
        }
    }
}

void lock_piece(Matrix *board, const Tetromino *piece) {
    int pRow, pCol;
    int board_row, board_col;

    for (pRow = 0; pRow < 4; pRow++) {
        for (pCol = 0; pCol < 4; pCol++) {
            if (get_cell(piece, pRow, pCol) == 0) {
                continue;
            }
            board_row = pRow + piece->row;
            board_col = pCol + piece->col;
            board->cell[board_row][board_col] = 1;
        }
    }
}