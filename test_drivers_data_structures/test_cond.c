#include "cond.h"
#include "../data_structures/tetromino.h"

#include <stdio.h>
int main() {
    Model test_model;
    printf("\n === Initializing Tetrimino I === \n");
    int game_over = spawn_piece(&test_model);

    if (game_over == 1) {
        printf("=== Game Over ===\n");
    }
}   