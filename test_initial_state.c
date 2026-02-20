#include "data_structures/model.h"
#include "game_state.h"
#include "data_structures/tetromino.h"
#include <stdio.h>

int main () {
    Model test_model;
    Model *test_pointer = &test_model;

    init_game_state(&test_pointer->game_state,0,1);

    printf("\n=== Testing Initial Game state ===\n");
    printf("Score: %u\n", test_pointer->game_state.score);
    printf("Level: %u\n", test_pointer->game_state.level);
    printf("Lines Cleared: %u\n", test_pointer->game_state.lines_cleared);
    // game over
    if (test_pointer->game_state.is_game_over == 1) {
        printf("Game Over\n");
    }
    else {
        printf("Game is currently running\n");
    }
}
