// implementations of functions found in "matrix.h" header file
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
