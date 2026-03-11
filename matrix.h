/* header file for matrix.c */
/* author: Henry             */

#ifndef MATRIX_H
#define MATRIX_H

#define MATRIX_ROWS 20
#define MATRIX_COLS 10

#include "tetrom.h"
#include "types.h"

/* 2D playable matrix grid: 20 rows x 10 columns */
typedef struct {
    UINT8 cell[MATRIX_ROWS][MATRIX_COLS];
} Matrix;

/*----- Function: create_matrix -----

 PURPOSE: creates and initializes an empty 20x10 Matrix grid

 INPUT: gameGrid ptr of Matrix struct type

 OUTPUT: fills all cells with 0

*/
void create_matrix(Matrix *gameGrid);

/*----- Function: output_matrix -----

 PURPOSE: outputs the 2D matrix grid at its current state

 INPUT: gameGrid ptr of Matrix struct type

 OUTPUT: prints grid to terminal

*/
void output_matrix(Matrix *gameGrid);

/*----- Function: clear_full_lines -----

 PURPOSE: detects and clears any fully filled rows

 INPUT: current Matrix state

 OUTPUT: number of lines cleared

*/
UINT16 clear_full_lines(Matrix *gameGrid);

/*----- Function: drop_rows_down -----

 PURPOSE: shifts non-empty rows down, fills upper rows with 0s

 INPUT: Matrix state after clear_full_lines() has run

 OUTPUT: updated Matrix with rows shifted down

*/
void drop_rows_down(Matrix *gameGrid);

/*----- Function: lock_piece -----

 PURPOSE: locks a Tetromino into the Matrix at its current position

 INPUT: board - current Matrix, piece - Tetromino to lock

 OUTPUT: Matrix updated with piece locked in place

*/
void lock_piece(Matrix *board, const Tetromino *piece);

#endif