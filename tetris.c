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

/* ====== MAIN TETRIS GAME ====== */

int main() {
    UINT32 timeThen;
    UINT32 timeNow;
    UINT32 timeElapsed;
    UINT8 quit = 0;         /* important: quit is set to FALSE */
    Model game_model;
    Tetromino temp_piece;
    Tetromino released_piece;
    char key;
    char scan;

    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT8 *) base);

    /* INITIALIZE GAME MODEL (Tetris State: START!) */
    init_model(&game_model);
        
        /* INITIALIZE Next + Hold Boxes */
    init_next_box(&game_model.nbox, peek_bag(&game_model));
    init_hold_box(&game_model.hbox);

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
            key = get_input(); /* store key press as a master key */
            printf("got Key: %d\n", (int) key);

            if (key == 'q') {
                quit = 1;
                printf("quit set to 1\n");
            }
        
            /* TETROMINO: MOVEMENT + ROTATE + SOFT DROP (4 KEYS) */
            else if (key == 0) {
                scan = get_scan_code();  /* update key with 2nd byte for correct comparison with arrow scan codes */
                printf("scan: %d\n", (int) scan); 

                if (scan == LEFT_ARROW) {
                    game_model.request_move_left = 1;
                }

                else if (scan == RIGHT_ARROW) {
                    game_model.request_move_right = 1;
                }

                else if (scan == UP_ARROW) {
                    game_model.request_rotate = 1;
                }

                else if (scan == DOWN_ARROW) {
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
                printf("processing left\n");
                printf("col: %d row: %d\n", game_model.piece.col, game_model.piece.row);

                if (!check_collision(&game_model.Matrix, &game_model.piece, game_model.piece.col - 1, game_model.piece.row)) 
                {
                    move_tetromino_left(&game_model.piece);
                }
                game_model.request_move_left = 0;  /* Clear request */
            }

            if (game_model.request_move_right) {
                printf("processing right\n");
                if (!check_collision(&game_model.Matrix, &game_model.piece, game_model.piece.col + 1, game_model.piece.row)) 
                {
                    move_tetromino_right(&game_model.piece);
                }
                game_model.request_move_right = 0;  /* Clear request */
            }

            if (game_model.request_rotate) {
                temp_piece = game_model.piece;
                rotate_tetromino_cw(&temp_piece);

                if (!check_collision(&game_model.Matrix, &temp_piece, temp_piece.col, temp_piece.row)) 
                {
                    game_model.piece = temp_piece;
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

            temp_piece = game_model.piece;

            if (game_model.request_hold) {
                game_model.request_hold = 0; /* Clear request*/

                /* check if holdbox is already full */
                if (hold_box_contains(&game_model.hbox)) {
                    released_piece = release_tetromino(&game_model.hbox);     /* release held tetromino first */
                    hold_tetromino(&game_model.hbox, &temp_piece);
                    game_model.piece = released_piece;                                  /* current game model piece is the just released piece */
                }

                /* not full, hold the 1st piece */
                else {
                    hold_tetromino(&game_model.hbox, &temp_piece);
                    /* game_model.piece = SPAWN IN A NEW PIECE since the current piece just got held */
                }
            }

            /* Trigger synchronous events based on timeElapsed */
            handle_tick(&game_model);
            /* update_state(&game_model.game_state, &game_model.game_state.lines_cleared); */

            if (check_game_over(&game_model.game_state)) {
                quit = 1;
            }

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
    printf("Game Over\n");
    return 0;
}