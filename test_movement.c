// #include "data_structures/tetromino.h"
#include "data_structures/model.h"
#include "data_structures/tetromino.h"
#include <stdio.h>

int main() {
    Model test_model;
    Model *model_pointer = &test_model;
    printf("\n === Initializing Movement Testing \n");
    
    init_tetromino(&model_pointer->piece,TETROMINO_I,5);   // initializing tetramino to col 5

    printf("=== Testing Move Left ===\n");
    printf("Initial position - Row: %d, Col: %d\n", model_pointer->piece.row, model_pointer->piece.col);
    // move left 
    move_tetromino_left(&model_pointer->piece);
    printf("After move_left() - Row: %d, Col: %d\n", model_pointer->piece.row, model_pointer->piece.col);
}