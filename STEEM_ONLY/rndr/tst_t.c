#include "renderer.h"
#include "model.h"
#include "gstate.h"
#include "tetrom.h"
#include "raster.h"
#include <osbind.h>

int main() {
    UINT32 *base = (UINT32 *)Physbase(); /* Get actual frame buffer */
    Model test_model;

    /* Clear screen first */
    clear_screen((UINT8 *)base);

    /* Initialize model */
    init_model(&test_model);

    /* Set up test scenario */
    init_tetromino(&test_model.piece, TETROMINO_I, 3);
    test_model.piece.row = 2;
    test_model.game_state.score = 1200;
    test_model.game_state.level = 3;
    test_model.game_state.lines_cleared = 15;

    /* Render everything to screen */
    render_matrix(base, &test_model.Matrix);
    render_piece(&test_model.piece, base);
    render_score(&test_model.game_state, (UINT8 *)base);
    render_level(&test_model.game_state, (UINT8 *)base);

    /* Wait for user to see the result */
    Crawcin();

    /* Clear and test different piece */
    clear_screen((UINT8 *)base);

    init_tetromino(&test_model.piece, TETROMINO_T, 5);
    test_model.piece.row = 8;
    test_model.game_state.score = 2400;
    test_model.game_state.level = 5;
    test_model.game_state.lines_cleared = 25;

    /* Render everything to screen */
    render_matrix(base, &test_model.Matrix);
    render_piece(&test_model.piece, base);
    render_score(&test_model.game_state, (UINT8 *)base);
    render_level(&test_model.game_state, (UINT8 *)base);

    Crawcin();

    clear_screen((UINT8 *)base);

    init_tetromino(&test_model.piece, TETROMINO_T, 5);

    /*Roatating tetrimino clockwise */
    rotate_tetromino_cw(&test_model.piece);
    test_model.piece.row = 8;
    test_model.game_state.score = 2400;
    test_model.game_state.level = 5;
    test_model.game_state.lines_cleared = 25;

    /* Render to screen */
    render_matrix(base, &test_model.Matrix);
    render_piece(&test_model.piece, base);
    render_score(&test_model.game_state, (UINT8 *)base);
    render_level(&test_model.game_state, (UINT8 *)base);

    /* Press any key to exit */
    Crawcin();

    clear_screen((UINT8 *)base);

    init_tetromino(&test_model.piece, TETROMINO_T, 5);
    rotate_tetromino_cw(&test_model.piece);

    /* Move tetrimino down*/
    move_tetromino_down(&test_model.piece);
    test_model.piece.row = 8;
    test_model.game_state.score = 2400;
    test_model.game_state.level = 5;
    test_model.game_state.lines_cleared = 25;

    /* Render to screen */
    render_matrix(base, &test_model.Matrix);
    render_piece(&test_model.piece, base);
    render_score(&test_model.game_state, (UINT8 *)base);
    render_level(&test_model.game_state, (UINT8 *)base);


    return 0;
}