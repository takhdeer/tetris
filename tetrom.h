/* File: tetrom.h          */
/* Header file for tetrom. */
/* Author: Aaron           */

#ifndef TETROMINO_H
#define TETROMINO_H

/* Tetromino types */
#define TETROMINO_I 0
#define TETROMINO_O 1
#define TETROMINO_T 2
#define TETROMINO_Z 3
#define TETROMINO_S 4
#define TETROMINO_L 5
#define TETROMINO_J 6

/* Tetromino structure */
typedef struct {
    int type;
    int row;
    int col;
    int rotation;
} Tetromino;

extern const int tetromino_shapes[7][4][4][4];
/* For reference: tetromino_shapes[type][rotation][row][col] */

/* BEHAVIOUR FUNCTIONS */

/*----- Function: init_tetromino -----
 PURPOSE: Initializes a tetromino.

 INPUT: piece: pointer to the tetromino being initialized.
        type: the tetromino type.
        spawn_col: the column position to spawn.

 OUTPUT: None
*/
void init_tetromino(Tetromino *piece, int type, int spawn_col);

/*----- Function: move_tetromino_left -----
 PURPOSE: Moves tetromino to the left by one cell.

 INPUT: piece: pointer to the tetromino.

 OUTPUT: None
*/
void move_tetromino_left(Tetromino *piece);

/*----- Function: move_tetromino_right -----
 PURPOSE: Moves tetromino to the right by one cell.

 INPUT: piece: pointer to the tetromino.

 OUTPUT: None
*/
void move_tetromino_right(Tetromino *piece);

/*----- Function: move_tetromino_down -----
 PURPOSE: Moves tetromino down by one cell.

 INPUT: piece: pointer to the tetromino.

 OUTPUT: None
*/
void move_tetromino_down(Tetromino *piece);

/*----- Function: rotate_tetromino_cw -----
 PURPOSE: Rotates the tetromino 90 degrees clockwise.

 INPUT: piece: pointer to the tetromino.

 OUTPUT: None
*/
void rotate_tetromino_cw(Tetromino *piece);

/*----- Function: get_cell -----
 PURPOSE: Returns whether a specified cell is empty or occupied.

 INPUT: piece: pointer to the Tetromino.
        local_row: row within the 4x4 box.
        local_col: column within the 4x4 box.

 OUTPUT: 1 = occupied, 0 = empty.
*/
int get_cell(const Tetromino *piece, int local_row, int local_col);

#endif