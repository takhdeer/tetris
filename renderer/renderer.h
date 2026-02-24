#ifndef RENDERER_H
#define RENDERER_H

// Aaron's Includes:
#include "../data_structures/tetromino.h"
#include "../data_structures/next_box.h"
#include "../types.h"

// Henry's Includes:
#include "../raster_graphics_library/raster.h"
#include "../data_structures/matrix.h"
#include "../data_structures/hold_box.h"

// Takhdeer's Includes:
#include "../cond.h"
#define BLOCK_SIZE 16

// === FUNCTION HEADERS ===

// Aaron's Functions:
/*----- Function: render_next_box -----

 PURPOSE: Renders the next box.

 INPUT: base: Pointer to screen memory.
        box: Pointer to the NextBox to render.

 OUTPUT: None

*/
void render_next_box(UINT8 *base, const NextBox *box);

// ===========================================

// Henry's Functions
/*----- Function: render_matrix -----

 PURPOSE: using raster library, render the Matrix to the frame buffer

 INPUT: frame buffer's base + the current state of the Tetris Matrix

 OUTPUT: no single return just visual output of rendering Matrix to FB

*/  
void render_matrix(UINT32 *base, const Matrix *gameGrid);

/*----- Function: render_hold_box -----

 PURPOSE: using raster library, render the HoldBox to the frame buffer

 INPUT: frame buffer's base + the current state of the HoldBox

 OUTPUT: no single return just visual output of rendering HoldBox to FB

*/ 

void render_hold_box(UINT32 *base, const HoldBox *heldbox);

// ===========================================

// Takhdeer's Functions
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
void render_level(const GameState *game, UINT8 *base);

/*
  Purpose: Master rendering function that draws the complete game state
 
  Input:
     - model: Pointer to the complete game model (board, piece, game_state)
     - base: Pointer to the frame buffer where rendering occurs
   
   Output: None (renders complete frame to frame buffer)
  
*/
void render_game_over(UINT8 *base, int score);

#endif