// header file for matrix.c
// author: Henry

#ifndef MATRIX_H
#define MATRIX_H

// define required constants to set matrix boundaries
#define MATRIX_ROWS 20
#define MATRIX_COLS 10
#include "tetromino.h"

// define Matrix struct
typedef struct {
    // initalize for the playable 2D matrix grid 20 rows x 10 columns
    unsigned char cell[MATRIX_ROWS][MATRIX_COLS];
} Matrix;

// define any functions related to Matrix struct

/*----- Function: create_matrix -----

 PURPOSE: creates and initializes an EMPTY 20 row x 10 columns 2D Matrix grid

 INPUT: gameGrid ptr of "Matrix" struct type

 OUTPUT: indirectly returns a ptr pointing to an empty 2D matrix[20][10] i.e 2D array filled with 0s

*/  
void create_matrix(Matrix *gameGrid);

/*----- Function: output_matrix -----

 PURPOSE: outputs the 2D matrix grid at current state

 INPUT: gameGrid ptr of "Matrix" struct type

 OUTPUT: outputs 2D matrix grid to terminal

*/  
void output_matrix(Matrix *gameGrid);

/*----- Function: clear_full_lines -----

 PURPOSE: detect if any rows are completely full of 1s (row of 1s = line clear)

 INPUT: current Tetris gameGrid i.e --> current Matrix state

 OUTPUT: unsigned integer that will communicate with update_score() and update_level()
            i.e return value here will add Tetris difficulty scaling based on line clears
*/ 

unsigned int clear_full_lines(Matrix *gameGrid);

/*----- Function: drop_rows_down -----

 PURPOSE: "squishes" non-Empty rows DOWN to the bottom of grid, fills the upper rows with 0s

 INPUT: Matrix state RIGHT after clear_full_lines() is finished running

 OUTPUT: Matrix that is has non-Empty rows shifted down, and the upper rows are empty
            
*/ 

void drop_rows_down(Matrix *gameGrid);

/*----- Function: check_game_over -----

 PURPOSE: in Matrix scope, detects if the game is over (yes if a new piece exceeds Matrix boundaries)

 INPUT: Current Matrix state of the Tetris gameGrid

 OUTPUT: true or false if the game is over or not
            
*/ 

// _Bool check_game_over(Matrix *gameGrid);

void lock_piece(Matrix *board, const Tetromino *piece);

#endif