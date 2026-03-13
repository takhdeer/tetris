/* this tetris.c file is the main source file where the Tetris game will run and live */
/* authors: Takhdeer, Henry, Aaron */

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
    UINT32 timeNow;
    UINT32 timeElapsed;
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

    while (quit != 1) {
        /* If Input Pending = Update Model Change REQUESTS */
        if (has_input()) {
            char key = get_input(); /* store key press as a master key */
            if (key == 'q') {
                quit = 1;
            }
            /* TETROMINO: MOVEMENT + ROTATE + SOFT DROP (4 KEYS) */
            if (key == 0) {

                key = get_input();  /* update key with 2nd byte for correct comparison with arrow scan codes */

                if (key == LEFT_ARROW) {
                    game_model.request_move_left = 1;
                }

                else if (key == RIGHT_ARROW) {
                    game_model.request_move_right = 1;
                }

                else if (key == UP_ARROW) {
                    game_model.request_rotate = 1;
                }

                else if (key == DOWN_ARROW) {
                    game_model.request_soft_drop = 1;
                }
            }

            /* TETROMINO: HOLD + QUIT (2 KEYS) */
            else {
                if (key == 'h') {
                    game_model.request_hold = 1;
                }
                else if (key == 'q') {
                    quit = 1;
                }
            }
        }

        /* If Clock Ticked = Update Model DATA */
        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0) {

            /* Process asynchronous requests */
            if (game_model.request_move_left) {
                if (!check_collision(&game_model.Matrix, &game_model.piece, game_model.piece.col - 1, game_model.piece.row)) 
                {
                    move_tetromino_left(&game_model.piece);
                }
                game_model.request_move_left = 0;  /* Clear request */
            }

            if (game_model.request_move_right) {
                if (!check_collision(&game_model.Matrix, &game_model.piece, game_model.piece.col + 1, game_model.piece.row)) 
                {
                    move_tetromino_right(&game_model.piece);
                }
                game_model.request_move_right = 0;  /* Clear request */
            }

            if (game_model.request_rotate) {
                Tetromino temp = game_model.piece;
                rotate_tetromino_cw(&temp);

                if (!check_collision(&game_model.Matrix, &temp, temp.col, temp.row)) 
                {
                    game_model.piece = temp;
                }
                game_model.request_rotate = 0;  /* Clear request */
            }

            if (game_model.request_soft_drop) {
                if (can_move_down(&game_model)) 
                {
                    move_tetromino_down(&game_model.piece);
                }
                game_model.request_soft_drop = 0;  /* Clear request */
            }

            if (game_model.request_hold) {
                /* TODO: implement hold logic */
                game_model.request_hold = 0; /* Clear request*/
            }

            /* Trigger synchronous events based on timeElapsed */
            handle_tick(&game_model);

            /* Render model */
            clear_screen((UINT8 *)base);
            render_matrix(base, &game_model.Matrix);
            render_next_box((UINT32 *) base, &game_model.nbox);
            render_hold_box(base, &game_model.hbox);
            render_piece(&game_model.piece, base);
            render_score(&game_model.game_state, (UINT8 *)base);
            render_level(&game_model.game_state, (UINT8 *)base);

            timeThen = timeNow;
        }
    }

    return 0;
}