// File test_tetromino_renderer.c
// Author: Aaron

#include <stdio.h>
#include "data_structures/tetromino.h"
#include "data_structures/matrix.h"

void render_tetrommino_to_matrix(Matrix *matrix, const Tetromino *piece) {
    for (int local_row = 0 ; local_row < 4 ; local_row++) {
        for (int local_col = 0 ; local_col < 4 ; local_col++) {

            if (get_cell(piece, local_row, local_col)) {
                int grid_row = piece->row + local_row;
                int grid_col = piece->col + local_col;

                if (grid_row >= 0 && grid_row < MATRIX_ROWS && grid_col >= 0 && grid_col < MATRIX_COLS) {
                    matrix->cell[grid_row][grid_col] = 1;
                }
            }
        }
    }
}

int main() {
    Matrix matrix;
    Tetromino piece;

    printf("=== Testing Tetromino Renderer ===\n\n");

    // Test 1: I-piece at spawn
    create_matrix(&matrix);
    init_tetromino(&piece, TETROMINO_I, 3);
    render_tetrommino_to_matrix(&matrix, &piece);
    printf("Test 1: I-piece at spawn (row=0, col=3):\n");
    output_matrix(&matrix);
    printf("\n");
    //row = -1 for the I piece to spawn at the very top.

    // Test 2: T-piece at position (5,4)
    create_matrix(&matrix);
    init_tetromino(&piece, TETROMINO_T, 4);
    piece.row = 5;
    render_tetrommino_to_matrix(&matrix, &piece);
    printf("Test 2: T-piece (row=5, col=4)\n");
    output_matrix(&matrix);
    printf("\n");

    // Test 3: Rotate piece
    create_matrix(&matrix);
    init_tetromino(&piece, TETROMINO_J, 8);
    piece.row = 17;
    rotate_tetromino_cw(&piece);
    render_tetrommino_to_matrix(&matrix, &piece);
    printf("Test 3: Rotated tetromino\n");
    output_matrix(&matrix);
    printf("\n");

    return 0;
}