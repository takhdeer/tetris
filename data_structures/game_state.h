#ifndef GAME_STATE_H
#define GAME_STATE_H
#define LINES_PER_LEVEL 10
#define SINGLE_LINE_BASE 40
#define DOUBLE_LINE_BASE 100
#define TRIPLE_LINE_BASE 300
#define TETRIS_LINE_BASE 1200
#include "../types.h"

// Initializing GameState Structure 
typedef struct GameState {
    UINT16  score;
    UINT16  level;
    UINT16  lines_cleared;
    UINT8   is_game_over;
} GameState;

/*
    Initializes the Gamestate with the provided score, and if the Game is over or not
*/
void init_game_state(GameState *gameState, UINT16 score, UINT8 game_over);

/* 
    Returns the line base depending on the # of lines cleared.
*/
UINT16 map_line_base(const UINT16 lines_cleared);

/*
    Updates the score based on the # of lines cleared, using map_line_base() as a helper function
*/

void update_state(GameState *state, UINT16 lines_cleared);

/*
    Checks if the game is over, by checking the is_game_over field
*/
UINT8 check_game_over(const GameState *state);
#endif