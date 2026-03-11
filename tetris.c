/* this tetris.c file is the main source file where the Tetris game will run and live */
/* authors: Takhdeer, Henry */

/* ====== REQUIRED INCLUDES ====== */
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

/* ====== REQUIRED CONSTANTS ====== */
const char LEFT_ARROW = 0x4B;        /* move left */
const char RIGHT_ARROW = 0x4D;       /* move right */
const char UP_ARROW = 0x48;          /* rotate */
const char DOWN_ARROW = 0x50;        /* soft drop */

/* ====== HELPER FUNCTIONS ====== */

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

/* ====== MAIN TETRIS GAME ====== */

int main() {
    UINT32 timeThen;
    UINT32 timerNow;
    UINT8 quit = 0; /* important: quit is set to FALSE */
    Model game_model;

    UINT32 *base = (UINT32 *)Physbase(); /* HM added entire line */
    clear_screen((UINT8 *) base);

    /* INITIALIZE GAME MODEL (Tetris State: START!) */
    init_model(&game_model); /* HM: added & */
    
    init_tetromino(&game_model.piece, TETROMINO_I, 3);
    init_next_box(&game_model.nbox, TETROMINO_S);
    init_hold_box(&game_model.hbox);
    game_model.piece.row = 2;
    game_model.game_state.score = 1200;
    game_model.game_state.level = 3;
    game_model.game_state.lines_cleared = 15;

    /* RENDER GAME MODEL everything to screen (Render State: FIRST FRAME) */
    render_matrix(base, &game_model.Matrix);
    render_next_box((UINT32 *) base, &game_model.nbox);
    render_hold_box(base, &game_model.hbox);
    render_piece(&game_model.piece, base);
    render_score(&game_model.game_state, (UINT8 *)base);
    render_level(&game_model.game_state, (UINT8 *)base);

    timeThen = get_time();

    /* MAIN TETRIS GAME LOOP */
    /* Crawcin(); commented out temporarily */
    while (quit != 1) {
        /* If Input Pending = Update Model Change REQUESTS */
        if (has_input()) {
            char key = get_input(); /* store key press as a master key */
        
            /* TETROMINO: MOVEMENT + ROTATE + SOFT DROP (4 KEYS) */
            if (key == 0) {

                key = get_input();  /* update key with 2nd byte for correct comparison with arrow scan codes */

                if (key == LEFT_ARROW) {
                    /* GAME LOGIC: move left */
                }

                else if (key == RIGHT_ARROW) {
                    /* GAME LOGIC: move right */
                }

                else if (key == UP_ARROW) {
                    /* GAME LOGIC: rotate */
                }

                else if (key == DOWN_ARROW) {
                    /* GAME LOGIC: soft drop */
                }
            }

            /* TETROMINO: HOLD + QUIT (2 KEYS) */
            else {
                if (key == 'h') {
                    /* GAME LOGIC: hold */
                }
                else if (key == 'q') {
                    quit = 1;
                }
            }
        }

        /* If Clock Ticked = Update Model DATA */
    }

    return 0;
}