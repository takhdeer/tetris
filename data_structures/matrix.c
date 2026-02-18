// implementations of functions found in "matrix.h" header file
// vocabulary:
//  - empty cell = 0 
//  - fill cell = 1
// author: Henry

// necessary includes:
#include "matrix.h"
#include <stdio.h>

int main() {
    printf("hello world\n");
    
    Matrix gameGrid;                // create instance of Matrix struct "gameGrid" on stack  

    create_matrix(&gameGrid);       // initializes an EMPTY Matrix grid
    output_matrix(&gameGrid);





    return 0;
}

// function implementations related to Matrix struct

void create_matrix(Matrix *gameGrid) { 
    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0 ; c < MATRIX_COLS; c++) {
            gameGrid->cell[r][c] = 0;
        }
    }
}

void output_matrix(Matrix *gameGrid) {
      for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            printf("%i", gameGrid->cell[r][c]);
        }
        printf("\n");
    }
}

unsigned int clear_full_lines(Matrix *gameGrid) {
    // Tetris game difficulty scaling based on # of lines were cleared in this Current Matrix
    unsigned int lines_cleared = 0;
    
    // detect if an individual cell == filled cell --> i.e cell = 1
    unsigned int filled_cell_count = 0;

    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            // Individual cell-by-cell comparison
            if (gameGrid->cell[r][c] == 1) {
                filled_cell_count++; // for every filled cell, increment filled cell count
            }
        }
        // comparison of filled cell count, detect if this row was full
        if (filled_cell_count == MATRIX_COLS) {
            lines_cleared++;                            // a) row is indeed full, line clear!
            for (int c = 0; c < MATRIX_COLS; c++) {     // b) set CURRENT row back to all 0s
                gameGrid->cell[r][c] = 0;
            }

        }
        filled_cell_count = 0; // reset filled_cell_count for next row
    }

    drop_rows_down(gameGrid);  // after done running, capture Matrix grid at this state 
                                // and shift nonEmpty rows down, fill upper rows with 0s

    return lines_cleared;
}

void drop_rows_down(Matrix *gameGrid) {

    int write = MATRIX_ROWS - 1;                            // write = write index

    // Traverse Matrix Rows from BottomMost Row -> TopMost Row
    for (int read = MATRIX_ROWS - 1; read >=0; read--) {    // read = read index

        // check if current read row is non empty
        unsigned char is_empty = 1;
        for (int c = 0; c < MATRIX_COLS; c++) {
            if (gameGrid->cell[read][c] != 0) {
                is_empty = 0;
                break;
            }
        }

        if (!is_empty) {
            // copy the Read row INTO Write row
            for (int c = 0; c < MATRIX_COLS; c++) {
                gameGrid->cell[write][c] = gameGrid->cell[read][c];
            }
            write--; // move Write row up the Matrix by 1
        }
    }

    // now fill remaining upper rows with 0s

    for (; write >= 0; write-- ) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            gameGrid->cell[write][c] = 0;
        }
    }
}

_Bool check_game_over(Matrix *gameGrid) {
    for (int c = 0; c < MATRIX_COLS; c++) {
        if (gameGrid->cell[0][c] == 1) {
            return 1; // game over
        }
    }
    return 0; // game continues
}
