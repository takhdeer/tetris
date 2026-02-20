
// Author: Aaron

// CONSTANTS
#ifndef TETROMINO_H
#define TETROMINO_H

// Tetromino types
#define TETROMINO_I 0
#define TETROMINO_O 1
#define TETROMINO_T 2
#define TETROMINO_Z 3
#define TETROMINO_S 4
#define TETROMINO_L 5
#define TETROMINO_J 6
 
// Tetromino struct
typedef struct {
    int type;
    int row;
    int col;
    int rotation;

} Tetromino;

extern const int tetromino_shapes[7][4][4][4];
// For reference: tetromino_shapes[type][rotation][row][col]

//Behaviour functions
void init_tetromino(Tetromino *piece, int type, int spawn_col);
void move_tetromino_left(Tetromino *piece);
void move_tetromino_right(Tetromino *piece);
void move_tetromino_down(Tetromino *piece);
void rotate_tetromino_cw(Tetromino *piece);
int get_cell(const Tetromino *piece, int local_row, int local_col);

#endif 