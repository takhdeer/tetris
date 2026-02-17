#include "types.h"
#ifndef GAME_STATE_H
#define GAME_STATE_H
#define LINES_PER_LEVEL 10
typedef struct GameState {
    UINT16  score;
    UINT16  level;
    UINT16  lines_cleared;
    UINT8   is_game_over;
} GameState;

extern GameState initial_game_state;

void update_state(GameState *state, UINT16 lines_cleared);
#endif