/*
 * test_henry_renderer.c
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

#include "henry_renderer.h"
#include "../data_structures/matrix.h"
#include "../data_structures/hold_box.h"
#include "../data_structures/tetromino.h"

int main()
{
    /* ── Get real Atari ST framebuffer via Physbase() ── */
    UINT32 *base = (UINT32 *)Physbase();

    Matrix  gameGrid;
    HoldBox heldbox;
    Tetromino piece_i, piece_t;

    /* initialize all data structures */
    create_matrix(&gameGrid);
    init_hold_box(&heldbox);
    init_tetromino(&piece_i, TETROMINO_I, 4);
    init_tetromino(&piece_t, TETROMINO_T, 4);

    /* ── Test 1: render_matrix() empty matrix ─────────────────
     * Expected visual: 10x20 grid of outlined squares on left
     * side of screen, all cells empty (outlines only)           */
    clear_screen(base);
    create_matrix(&gameGrid);
    render_matrix(base, &gameGrid);

    /* wait for keypress before next test */
    Cconin();

    /* ── Test 2: render_matrix() partially filled matrix ───────
     * Expected visual: bottom 3 rows fully filled (solid black
     * squares), rows above shown as outlines only               */
    clear_screen(base);
    create_matrix(&gameGrid);

    /* fill bottom 3 rows */
    {
        int r, c;
        for (r = 17; r < MATRIX_ROWS; r++)
            for (c = 0; c < MATRIX_COLS; c++)
                gameGrid.cell[r][c] = 1;
    }
    render_matrix(base, &gameGrid);

    Cconin();

    /* ── Test 3: render_matrix() checkerboard pattern ──────────
     * Expected visual: alternating filled/outline cells across
     * the entire matrix, visually confirming both cell states   */
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

    /* ── Test 4: render_hold_box() empty hold box ──────────────
     * Expected visual: empty bordered rectangle at bottom right
     * of screen, no tetromino inside                            */
    clear_screen(base);
    init_hold_box(&heldbox);
    render_hold_box(base, &heldbox);

    Cconin();

    /* ── Test 5: render_hold_box() holding TETROMINO_I ─────────
     * Expected visual: bordered rectangle at bottom right with
     * I-piece rendered inside it                                */
    clear_screen(base);
    init_hold_box(&heldbox);
    hold_tetromino(&heldbox, &piece_i);
    render_hold_box(base, &heldbox);

    Cconin();

    /* ── Test 6: render_hold_box() swap I -> T ─────────────────
     * Expected visual: bordered rectangle at bottom right with
     * T-piece rendered inside (I-piece was swapped out)         */
    clear_screen(base);
    hold_tetromino(&heldbox, &piece_t);
    render_hold_box(base, &heldbox);

    Cconin();

    /* ── Test 7: render_matrix() + render_hold_box() together ──
     * Expected visual: partially filled matrix on left side AND
     * hold box with T-piece on bottom right simultaneously      */
    clear_screen(base);
    create_matrix(&gameGrid);

    {
        int c;
        for (c = 0; c < MATRIX_COLS; c++)
            gameGrid.cell[19][c] = 1;   /* full bottom row */
        gameGrid.cell[18][3] = 1;
        gameGrid.cell[18][4] = 1;
        gameGrid.cell[17][4] = 1;
    }

    render_matrix(base, &gameGrid);
    render_hold_box(base, &heldbox);

    Cconin();

    return 0;
}