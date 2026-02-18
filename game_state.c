#include "types.h"
#include "game_state.h"

GameState initial_game_state = {
    .score = 0,
    .level = 1,
    .lines_cleared = 0,
    .is_game_over = 0,
};

UINT16 map_line_base(const UINT16 lines_cleared) {
    if (lines_cleared == 1)
    {
        return SINGLE_LINE_BASE; 
    }
    else if (lines_cleared == 2)
    {
        return DOUBLE_LINE_BASE;
    }
    else if (lines_cleared == 3)
    {
        return TRIPLE_LINE_BASE;
    }
    else{
        return TETRIS_LINE_BASE;
    }
}

void update_state(GameState *state, UINT16 lines_just_cleared) {
    UINT16 base_points = map_line_base(lines_just_cleared);
    state->score += (base_points) * (state->level + 1);
    state->lines_cleared += lines_just_cleared;
    
    if (state->lines_cleared % 10 == 0) {
        state->level++;
    }
}

UINT8 check_game_over(const GameState *state) {
    if (state->is_game_over == 1) {
        return 1;
    }
    return 0;
}
