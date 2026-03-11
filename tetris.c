#include <stdio.h>
#include <osbind.h>
#include "input.h"
#include "synch.h"
#include "cond.h"
#include "renderer.h"
#include "model.h"

#include "gstate.h"
#include "hold_box.h"
#include "next_box.h"
#include "matrix.h"
/* Helper functions */

/* Get time function */
UINT32 get_time() {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = Super(0);
    timeNow = *timer;
    Super(old_ssp);

    return timeNow;
}

int main() {
    UINT32 timeThen;
    UINT32 timerNow;
    UINT8 quit = 0;
    Model game_model;

    UINT32 *base = (UINT32 *)Physbase(); /* HM added entire line */
    clear_screen((UINT8 *) base);

    init_model(&game_model); /* HM: added & */
    
    init_tetromino(&game_model.piece, TETROMINO_I, 3);
    init_next_box(&game_model.nbox, TETROMINO_S);
    init_hold_box(&game_model.hbox);
    game_model.piece.row = 2;
    game_model.game_state.score = 1200;
    game_model.game_state.level = 3;
    game_model.game_state.lines_cleared = 15;

    /* Render everything to screen */
    render_matrix(base, &game_model.Matrix);
    render_next_box((UINT32 *) base, &game_model.nbox);
    render_hold_box(base, &game_model.hbox);
    render_piece(&game_model.piece, base);
    render_score(&game_model.game_state, (UINT8 *)base);
    render_level(&game_model.game_state, (UINT8 *)base);

    timeThen = get_time();

    Crawcin();  /* Replace with main game loop*/

    return 0;
}