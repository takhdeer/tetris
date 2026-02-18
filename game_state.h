#ifndef GAME_STATE_H
#define GAME_STATE_H
#define LINES_PER_LEVEL 10
#define SINGLE_LINE_BASE 40
#define DOUBLE_LINE_BASE 100
#define TRIPLE_LINE_BASE 300
#define TETRIS_LINE_BASE 1200
#include "types.h"

typedef struct GameState {
    UINT16  score;
    UINT16  level;
    UINT16  lines_cleared;
    UINT8   is_game_over;
} GameState;

extern GameState initial_game_state;

UINT16 map_line_base(const UINT16 lines_cleared);
void update_state(GameState *state, UINT16 lines_cleared);
UINT8 check_game_over(const GameState *state);
#endif