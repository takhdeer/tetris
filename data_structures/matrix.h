// header file for matrix.c
// author: Henry

// define required constants to set matrix boundaries
#define MATRIX_ROWS 20
#define MATRIX_COLS 10

// define Matrix struct
typedef struct {
    // initalize for the playable 2D matrix grid 20 rows x 10 columns
    unsigned char cell[20][10];
} Matrix;

// define any functions related to Matrix struct

/*----- Function: create_matrix -----

 PURPOSE: creates and initializes an EMPTY 20 row x 10 columns 2D Matrix grid

 INPUT: gameGrid ptr of "Matrix" struct type

 OUTPUT: returns a ptr pointing to an empty 2D matrix[20][10] i.e 2D array filled with 0s

*/  
void create_matrix(Matrix *gameGrid);

/*----- Function: output_matrix -----

 PURPOSE: outputs the 2D matrix grid at current state

 INPUT: gameGrid ptr of "Matrix" struct type

 OUTPUT: outputs 2D matrix grid to terminal

*/  
void output_matrix(Matrix *gameGrid);

/*----- Function: clear_full_lines -----

 PURPOSE: 

 INPUT: 

 OUTPUT:

*/ 

