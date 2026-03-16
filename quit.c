/* tetris.c - q to quit test */
/* Authors: Takhdeer */

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
    UINT32 timeNow;
    UINT32 timeElapsed;
    UINT8 quit = 0; 
    Model game_model;
    Tetromino temp_piece;
    Tetromino released_piece;
    UINT32 *base;

    base = (UINT32 *)Physbase();
    clear_screen((UINT8 *)base);

    init_model(&game_model);
    init_tetromino(&game_model.piece, TETROMINO_I, 3);
    game_model.piece.row = 2;
    game_model.game_state.score = 0;
    game_model.game_state.level = 1;
    game_model.game_state.lines_cleared = 0;


    printf("Game starting...\n");

    timeThen = get_time();

    while (quit != 1) {

        if (has_input()) {
            char key = get_input();
            printf("Key: %d\n", (int)key);

            if (key == 'q') {
                printf("Quitting...\n");
                quit = 1;
            }
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0) {
            handle_tick(&game_model);
            clear_screen((UINT8 *)base);
            render_matrix(base, &game_model.Matrix);
            render_piece(&game_model.piece, base);
            render_score(&game_model.game_state, (UINT8 *)base);
            render_level(&game_model.game_state, (UINT8 *)base);
        }
    }

    printf("Exited loop cleanly\n");
    return 0;
}

