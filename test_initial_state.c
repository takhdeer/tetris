#include "data_structures/model.h"
#include "game_state.h"
#include "data_structures/tetromino.h"
#include <stdio.h>

int main () {
    Model *test_model;
    printf("=== Testing Initial Game state ===\n");
    printf("Score: %d\n", test_model->game_state.score);
    printf("Level: %d\n", test_model->game_state.level);
    printf("Lines Cleared: %d\n", test_model->game_state.lines_cleared);
    printf("Game over: %d\n", test_model->game_state.is_game_over);
}
