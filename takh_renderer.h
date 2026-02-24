#include "cond.h"
#include "raster.h"
#include "data_structures/tetromino.h"

#define BLOCK_SIZE 16

void render_peice(const Tetromino *piece, UINT32 *base);

void uint_to_str(UINT32 value, char *buf, int start);

void render_score(const GameState *game, UINT8 *base);

void render_level(const GameState *game, UINT8 base);

void render_game_over(UINT8 *base, int score);
