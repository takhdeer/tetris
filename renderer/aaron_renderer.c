// TEMPORARY FILE, COPY INTO renderer.c file after.
// File: aaron_renderer.c

// Author: Aaron

#include "aaron_renderer.h" // change to renderer.c after
#include "../raster_graphics_library/raster.h"

#define CELL_SIZE 16
#define NEXT_BOX_ROW 50
#define NEXT_BOX_COL 500

void render_next_box(UINT8 *base, const NextBox *box) {
    Tetromino preview_piece;
    int next_type;

    // Get the next piece type
    next_type = get_next_type(box);

    init_tetromino(&preview_piece, next_type, NEXT_BOX_COL / CELL_SIZE);
    preview_piece.row = NEXT_BOX_ROW / CELL_SIZE;

    render_tetromino(base, &preview_piece);
}
