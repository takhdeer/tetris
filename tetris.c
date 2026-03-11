#include <stdio.h>
#include <osbind.h>
#include "input.h"
#include "synch.h"
#include "cond.h"
#include "renderer.h"
#include "model.h"
/*Helper functions */

/*Get time function*/
UINT32 get_time() {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = super(0);
    timeNow = *timer;
    super(old_ssp);

    return timeNow;
}


int main() {
    UINT32 timeThen;
    UINT32 timerNow;
    UINT8 quit = 0;
    Model game_model;

    clear_screen((UINT8 *) base);

    init_model(game_model);
    
    init_tetromino(&test_model.piece, TETROMINO_I, 3);
    init_next_box(&test_model.nbox, TETROMINO_S);
    init_hold_box(&test_model.hbox);
    test_model.piece.row = 2;
    test_model.game_state.score = 1200;
    test_model.game_state.level = 3;
    test_model.game_state.lines_cleared = 15;

    /* Render everything to screen */
    render_matrix(base, &test_model.Matrix);
    render_next_box((UINT32 *) base, &test_model.nbox);
    render_hold_box(base, &test_model.hbox);
    render_piece(&test_model.piece, base);
    render_score(&test_model.game_state, (UINT8 *)base);
    render_level(&test_model.game_state, (UINT8 *)base);

    timeThen = get_time();

    Crawcin();  /* Replace with main game loop*/

    return 0;
}