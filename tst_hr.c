/*
 * tst_hr.c
 * Renderer Test Driver - Matrix and HoldBox
 *
 * Per checkpoint 2 spec: copy of model test driver but replaces
 * printed output with calls to the render functions.
 *
 * NOTE: uses Physbase() to retrieve the actual Atari ST framebuffer.
 * This file must be compiled and run on the Atari ST / Steem Engine.
 * It will NOT run in a standard PC terminal environment.
 *
 * Author: Henry
 */

#include "renderer.h"
#include "matrix.h"
#include "hold_box.h"
#include "tetrom.h"
#include <osbind.h>

int main()
{
    UINT32 *base = (UINT32 *)Physbase();

    Matrix    gameGrid;
    HoldBox   heldbox;
    Tetromino piece_i, piece_t;

    /* initialize all data structures */
    create_matrix(&gameGrid);
    init_hold_box(&heldbox);
    init_tetromino(&piece_i, TETROMINO_I, 4);
    init_tetromino(&piece_t, TETROMINO_T, 4);

    /* Test 1: render_matrix() empty matrix
     * Expected: 10x20 grid of outlined squares, all cells empty */
    clear_screen(base);
    create_matrix(&gameGrid);
    render_matrix(base, &gameGrid);
    Cconin();

    /* Test 2: render_matrix() partially filled matrix
     * Expected: bottom 3 rows fully filled, rows above outlined */
    clear_screen(base);
    create_matrix(&gameGrid);
    {
        int r, c;
        for (r = 17; r < MATRIX_ROWS; r++)
            for (c = 0; c < MATRIX_COLS; c++)
                gameGrid.cell[r][c] = 1;
    }
    render_matrix(base, &gameGrid);
    Cconin();

    /* Test 3: render_matrix() checkerboard pattern
     * Expected: alternating filled/outline cells across matrix */
    clear_screen(base);
    create_matrix(&gameGrid);
    {
        int r, c;
        for (r = 0; r < MATRIX_ROWS; r++)
            for (c = 0; c < MATRIX_COLS; c++)
                gameGrid.cell[r][c] = (r + c) % 2;
    }
    render_matrix(base, &gameGrid);
    Cconin();

    /* Test 4: render_hold_box() empty hold box
     * Expected: empty bordered rectangle, no tetromino inside */
    clear_screen(base);
    init_hold_box(&heldbox);
    render_hold_box(base, &heldbox);
    Cconin();

    /* Test 5: render_hold_box() holding TETROMINO_I
     * Expected: bordered rectangle with I-piece inside */
    clear_screen(base);
    init_hold_box(&heldbox);
    hold_tetromino(&heldbox, &piece_i);
    render_hold_box(base, &heldbox);
    Cconin();

    /* Test 6: render_hold_box() swap I -> T
     * Expected: bordered rectangle with T-piece inside */
    clear_screen(base);
    hold_tetromino(&heldbox, &piece_t);
    render_hold_box(base, &heldbox);
    Cconin();

    /* Test 7: render_matrix() + render_hold_box() together
     * Expected: partially filled matrix AND hold box with T-piece */
    clear_screen(base);
    create_matrix(&gameGrid);
    {
        int c;
        for (c = 0; c < MATRIX_COLS; c++)
            gameGrid.cell[19][c] = 1;
        gameGrid.cell[18][3] = 1;
        gameGrid.cell[18][4] = 1;
        gameGrid.cell[17][4] = 1;
    }
    render_matrix(base, &gameGrid);
    render_hold_box(base, &heldbox);
    Cconin();

    return 0;
}