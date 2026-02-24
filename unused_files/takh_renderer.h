#include "cond.h"
#include "raster_graphics_library/raster.h"
#include "../data_structures/tetromino.h"

#define BLOCK_SIZE 16

/*
    Purpose: Renders all locked pieces on the game board
     
      Input:
        - board: Pointer to the game board matrix (20x10 grid)
        - base: Pointer to the frame buffer where rendering occurs
      
      Output: None (renders directly to frame buffer)
      
*/
void render_piece(const Tetromino *piece, UINT32 *base);

/*
    Helper function designed to convert unsigned int to string
*/
void uint_to_str(UINT32 value, char *buf, int start);

/*
  Purpose: Master rendering function that draws the complete game state
 
  Input:
     - model: Pointer to the complete game model (board, piece, game_state)
     - base: Pointer to the frame buffer where rendering occurs
   
   Output: None (renders complete frame to frame buffer)
  
*/
void render_score(const GameState *game, UINT8 *base);

/*
  Purpose: Master rendering function that draws the complete game state
 
  Input:
     - model: Pointer to the complete game model (board, piece, game_state)
     - base: Pointer to the frame buffer where rendering occurs
   
   Output: None (renders complete frame to frame buffer)
  
*/
void render_level(const GameState *game, UINT8 base);

/*
  Purpose: Master rendering function that draws the complete game state
 
  Input:
     - model: Pointer to the complete game model (board, piece, game_state)
     - base: Pointer to the frame buffer where rendering occurs
   
   Output: None (renders complete frame to frame buffer)
  
*/
void render_game_over(UINT8 *base, int score);
