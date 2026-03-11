/* Author: Aaron */

#include "tetrom.h"

const int tetromino_shapes[7][4][4][4] = {

    /* tetromino_shapes[type][rotation][row][col] */

    /* TETROMINO_I */
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0} }, /* rotation: 2 */
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }  /* rotation: 3 */
    },
    /* TETROMINO_O */
    {
        { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} }  /* rotation: 3 */
    },
    /* TETROMINO_T */
    {
        { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,0,1,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }  /* rotation: 3 */
    },
    /* TETROMINO_Z */
    {
        { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {1,0,0,0} }  /* rotation: 3 */
    },
    /* TETROMINO_S */
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,1,0} }  /* rotation: 3 */
    },
    /* TETROMINO_L */
    {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }  /* rotation: 3 */
    },
    /* TETROMINO_J */
    {
        { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, /* rotation: 0 */
        { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }, /* rotation: 1 */
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} }, /* rotation: 2 */
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }  /* rotation: 3 */
    }
};

void init_tetromino(Tetromino *piece, int type, int spawn_col) {
    piece->type     = type;
    piece->row      = 0;
    piece->col      = spawn_col;
    piece->rotation = 0;
}

void move_tetromino_left(Tetromino *piece) {
    piece->col -= 1;
}

void move_tetromino_right(Tetromino *piece) {
    piece->col += 1;
}

void move_tetromino_down(Tetromino *piece) {
    piece->row += 1;
}

void rotate_tetromino_cw(Tetromino *piece) {
    piece->rotation = (piece->rotation + 1) % 4;
}

int get_cell(const Tetromino *piece, int local_row, int local_col) {
    return tetromino_shapes[piece->type][piece->rotation][local_row][local_col];
}