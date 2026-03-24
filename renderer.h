#ifndef RENDERER_H
#define RENDERER_H

/* Aaron's Includes: */
#include "tetrom.h"
#include "next_box.h"
#include "types.h"

/* Henry's Includes: */
#include "raster.h"
#include "matrix.h"
#include "hold_box.h"

/* Takhdeer's Includes: */
#include "cond.h"
#define BLOCK_SIZE 16

#define COL_OFFSET 240  /* For centering the matrix + pixels*/
#define ROW_OFFSET 40

/* === FUNCTION HEADERS === */

/* Aaron's Functions: */
/*----- Function: render_next_box -----

 PURPOSE: Renders the next box.

 INPUT: base: Pointer to screen memory.
        box: Pointer to the NextBox to render.

 OUTPUT: None

*/
void render_next_box(UINT32 *base, const NextBox *box);  

/* =========================================== */

/* Henry's Functions */
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

/* =========================================== */

/* Takhdeer's Functions */
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
  Purpose: Renders the score to the frame buffer

  Input:
     - game: Pointer to the GameState
     - base: Pointer to the frame buffer

  Output: None
*/
void render_score(const GameState *game, UINT8 *base);

/*
  Purpose: Renders the level to the frame buffer

  Input:
     - game: Pointer to the GameState
     - base: Pointer to the frame buffer

  Output: None
*/
void render_level(const GameState *game, UINT8 *base);

/*
  Purpose: Renders the game over screen to the frame buffer

  Input:
     - base: Pointer to the frame buffer
     - score: Final score to display

  Output: None
*/
void render_game_over(UINT8 *base, int score);

#endif