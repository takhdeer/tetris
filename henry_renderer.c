// render function defintions from "henry_renderer.h"
// author: Henry

#include "henry_renderer.h" // temp change this to "renderer.h"
#include "aaron_renderer.h"

#define CELL_SIZE 16
#define HOLD_BOX_ROW 320
#define HOLD_BOX_COL 500
#define HOLD_BOX_SIZE (4 * CELL_SIZE)

void render_matrix(UINT32 *base, const Matrix *gameGrid) {  
    UINT16 pixel_row, pixel_col;
    
    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            pixel_row = r * CELL_SIZE;
            pixel_col = c * CELL_SIZE;
            // Case 1: cell in Matrix holds a 1 = filled cell
            if (gameGrid->cell[r][c] == 1) {
                // plot SOLID filled block to FrameBuffer
                for (int i = 0; i < CELL_SIZE; i++) {
                    plot_horizontal_line(base, pixel_row + i, pixel_col, CELL_SIZE);
                }
            }
            // Case 2: cell in Matrix holds a 0 = empty cell
            else {
                // plot OUTLINE block only to FrameBuffer
                plot_square(base, pixel_row, pixel_col, CELL_SIZE); 
            }
        }
    }
}

void render_hold_box(UINT32 *base, const HoldBox *heldbox) {
    Tetromino temp_piece;
    
    // Case 1: render an occupied HoldBox with its current Tetromino
    if (heldbox->contains == 1) {
        // extract held piece and its coordinates
        temp_piece = heldbox->piece_held;
        temp_piece.row = HOLD_BOX_ROW / CELL_SIZE;
        temp_piece.col = HOLD_BOX_COL / CELL_SIZE;

        // plot to FrameBuffer
        plot_rectangle(base, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);
        render_tetromino((UINT8 *)base, &temp_piece);
    }

    // Case 2: render an empty HoldBox
    else {
        // plot to FrameBuffer
        plot_rectangle(base, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);
    }
}
