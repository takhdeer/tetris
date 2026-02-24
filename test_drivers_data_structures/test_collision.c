// File: test_collision.c
// Test driver for collisions.
// Author: Aaron

#include <stdio.h>
#include "../data_structures/matrix.h"
#include "../data_structures/tetromino.h"
#include "../data_structures/synch.h"

int main() {
    Matrix board;
    Tetromino piece;

    printf("=== Testing Collisions ===\n");

    // Empty matrix
    create_matrix(&board);

    // Left collision test:
    init_tetromino(&piece, TETROMINO_I, 0); // col = 0
    UINT16 result = check_collision(&board, &piece, -1, 0);

    printf("Left collision test:\n");
    if (result == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n"); 
    }

    // Right collision test:
    init_tetromino(&piece, TETROMINO_I, 6); // rightmost valid position
    UINT16 result2 = check_collision(&board, &piece, 7, 0);

    printf("Right collision test:\n");
    if (result2 == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n"); 
    }

    // Bottom collision test:
    init_tetromino(&piece, TETROMINO_I, 3); // middle col
    UINT16 result3 = check_collision(&board, &piece, 3, 19); // row 19

    printf("Bottom collision test:\n");
    if (result3 == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n"); 
    }

    // Locked piece collision test:
    
    // Filling in cells in the middle of the board
    board.cell[18][4] = 1;
    board.cell[18][5] = 1;
    board.cell[18][6] = 1;
    board.cell[18][7] = 1;

    init_tetromino(&piece, TETROMINO_I, 4);
    UINT16 result4 = check_collision(&board, &piece, 4, 17);
    printf("Locked piece collision test:\n");
    if (result4 == 1) {
        printf("PASS\n");
    } else {
        printf("FAIL\n"); 
    }

    return 0;
}