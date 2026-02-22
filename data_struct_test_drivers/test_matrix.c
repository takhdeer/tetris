// individual test driver for Matrix data structure
// directly testing matrix.h and matrix.c implementations
// author: Henry

#include <stdio.h>
#include "../data_structures/matrix.h"

int main() {
    printf("=== Testing 'Matrix' Data Structure ===\n");

    Matrix gameGrid;

    /* --- Test 1: create_matrix() --- */
    printf(" --- Test 1: create_matrix() creates an empty 20 rows x 10 cols Matrix ---\n");
    create_matrix(&gameGrid);
    output_matrix(&gameGrid);

    int passed = 1;
    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            if (gameGrid.cell[r][c] != 0) {
                passed = 0;
            }
        }
    }
    printf("create_matrix test: %s\n\n", passed ? "PASSED" : "FAILED");


    /* --- Test 2: clear_full_lines() + drop_rows_down() --- */
    printf(" --- Test 2a: clear_full_lines() single line clear + drop_rows_down() works for 1 row drop ---\n");
    create_matrix(&gameGrid); // important! -> manually reset to an empty matrix first before running a Matrix test
    // HARD CODE: BottomMost Row is a Full Row
    for (int c = 0; c < MATRIX_COLS; c++) {
        gameGrid.cell[19][c] = 1;
    }

    printf("Before clear_fill_lines() is called:\n");
    output_matrix(&gameGrid);

    unsigned int lines_a = clear_full_lines(&gameGrid);

    printf("\nAfter clear_full_lines() is called:\n");
    output_matrix(&gameGrid);
    printf("Lines cleared: %i (expected 1): %s\n\n", lines_a, lines_a == 1 ? "PASSED" : "FAILED");

    printf(" --- Test 2b: clear_full_lines() no line clear ---\n");
    create_matrix(&gameGrid);
    // HARD CODE: Some Cells are Filled but not Complete Full Rows
    for (int c = 0; c < MATRIX_COLS; c+=2) {
        gameGrid.cell[19][c] = 1;
    }
    for (int c = 0; c < MATRIX_COLS; c+=3) {
        gameGrid.cell[18][c] = 1;
    }
    for (int c = 0; c < MATRIX_COLS; c+=5) {
        gameGrid.cell[16][c] = 1;
    }

    printf("Before clear_fill_lines() is called:\n");
    output_matrix(&gameGrid);

    unsigned int lines_b = clear_full_lines(&gameGrid);

    printf("\nAfter clear_full_lines() is called:\n");
    output_matrix(&gameGrid);
    printf("Lines cleared: %i (expected 0): %s\n\n", lines_b, lines_b == 0 ? "PASSED" : "FAILED");

    printf(" --- Test 2c: clear_full_lines() mulitple line clears + drop_rows_down() works for 4 rows drop ---\n");
    create_matrix(&gameGrid);
    // HARD CODE:
    for (int c = 0; c < MATRIX_COLS; c++) {
        gameGrid.cell[19][c] = 1;
    }
    for (int c = 0; c < MATRIX_COLS; c++) {
        gameGrid.cell[18][c] = 1;
    }
    for (int c = 0; c < MATRIX_COLS; c++) {
        gameGrid.cell[17][c] = 1;
    }
     for (int c = 0; c < MATRIX_COLS; c++) {
        gameGrid.cell[16][c] = 1;
    }

    printf("Before clear_fill_lines() is called:\n");
    output_matrix(&gameGrid);

    unsigned int lines_c = clear_full_lines(&gameGrid);

    printf("\nAfter clear_full_lines() is called:\n");
    output_matrix(&gameGrid);
    printf("Lines cleared: %i (expected 4): %s\n\n", lines_c, lines_c == 4 ? "PASSED" : "FAILED");

    // --- Test 3: check_game_over() ? ---
    // idk if i can test this because Aaron commented it out for the "test_collison.c" test driver
    
    // --- Test 4: lock_piece() ? ---
    // i also dk if i can test this since Aaron wrote the function and the code is in "test_collision.c" ?

    return 0;
}

