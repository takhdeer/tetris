// temp. file of Renderer Portion -> add this to master "renderer.h" file\
// this file is henry's Renderer Portion (Matrix, HoldBox)
// author: Henry

#include "raster.h"
#include "types.h"
#include "../data_structures/matrix.h"
#include "../data_structures/hold_box.h"

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
