#include "types.h"
#include "gstate.h"

void init_game_state(GameState *gameState, UINT16 score, UINT8 game_over) {
    gameState->score = score;
    gameState->level = 1;
    gameState->lines_cleared = 0;
    gameState->is_game_over = game_over;
}

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
    else
    {
        return TETRIS_LINE_BASE;
    }
}

void update_state(GameState *state, UINT16 lines_just_cleared) {
    UINT16 base_points;
    if (lines_just_cleared == 0) return;
    
    base_points = map_line_base(lines_just_cleared);
    state->score += base_points * (state->level + 1);

    /* use total lines_cleared from model, not lines_just_cleared */
    if (state->lines_cleared % 10 == 0) {
        state->level++;
    }
}