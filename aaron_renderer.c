// TEMPORARY FILE, COPY INTO renderer.c file after.
// File: aaron_renderer.c

// Author: Aaron

#include "aaron_renderer.h" // change to renderer.c after
#include "raster.h"

#define CELL_SIZE 16
#define NEXT_BOX_ROW 50
#define NEXT_BOX_COL 500

void render_tetromino(UINT8 *base, const Tetromino *piece) {
    int screen_row;
    int screen_col;

    // Looping through 4x4 box
    for (int local_row = 0 ; local_row < 4 ; local_row++) {
        for (int local_col = 0 ; local_col < 4 ; local_col++) {

             //Check if cell is occupied
             if (get_cell(piece, local_row, local_col)) {

                // Calculating screen position
                screen_row = (piece->row + local_row) * CELL_SIZE;
                screen_col = (piece->col + local_col) * CELL_SIZE;

                plot_square((UINT32 *)base, screen_row, screen_col, CELL_SIZE);
             }
        }
    }
}

void render_next_box(UINT8 *base, const NextBox *box) {
    Tetromino preview_piece;
    int next_type;

    // Get the next piece type
    next_type = get_next_type(box);

    init_tetromino(&preview_piece, next_type, NEXT_BOX_COL / CELL_SIZE);
    preview_piece.row = NEXT_BOX_ROW / CELL_SIZE;

    render_tetromino(base, &preview_piece);
}
