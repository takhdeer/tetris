// TEMPORARY renderer.h FILE 
// Author: Aaron

#ifndef AARON_RENDERER_H // change to RENDERER_H after
#define AARON_RENDERER_H // change to RENDERER_H after

#include "data_structures/tetromino.h"
#include "data_structures/next_box.h"
#include "types.h"

// Functions

/*----- Function: render_tetromino -----
 PURPOSE: Renders a tetromino to the screen.

 INPUT: base: Pointer to screen memory.
        piece: Pointer to the Tetromino to render.

 OUTPUT: None
*/
void render_tetromino(UINT8 *base, const Tetromino *piece);

/*----- Function: render_next_box -----

 PURPOSE: Renders the next box.

 INPUT: base: Pointer to screen memory.
        box: Pointer to the NextBox to render.

 OUTPUT: None

*/
void render_next_box(UINT8 *base, const NextBox *box);

#endif