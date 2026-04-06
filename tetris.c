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

#include "music.h"
#include "splash.h"
#include "effects.h"

/* ====== REQUIRED CONSTANTS ====== */
#define CELL_SIZE 16

#define NEXT_BOX_ROW 50
#define NEXT_BOX_COL 500
#define NEXT_BOX_SIZE 80

#define HOLD_BOX_ROW 320
#define HOLD_BOX_COL 500
#define HOLD_BOX_SIZE 80

const char LEFT_ARROW = 0x4B;        /* move left */
const char RIGHT_ARROW = 0x4D;       /* move right */
const char UP_ARROW = 0x48;          /* rotate */
const char DOWN_ARROW = 0x50;        /* soft drop */

/* ===== DOUBLE BUFFERING SETUP =====*/
UINT8 buffer_space[32256]; /* 32000 + 256 for alignment*/

/* ====== HELPER FUNCTIONS ====== */

/* Get time function */
/*
UINT32 get_time() {
    long *timer = (long *)0x462;
    UINT32 timeNow;
    long old_ssp;

    old_ssp = Super(0);
    timeNow = *timer;
    Super(old_ssp);

    return timeNow;
}
    */

/* Wait for vertical blank function */
void wait_vbl() {
    UINT32 old_time = get_time();
    while (get_time() == old_time) {
        /* Busy wait until timer increment (next VBL) */
    }
}
/* ====== MAIN TETRIS GAME ====== */

int main() {
    UINT32 timeThen;
    UINT32 timeNow;
    UINT32 timeElapsed;
    UINT8 quit = 0;         /* important: quit is set to FALSE */
    Model game_model;
    char key;
    Tetromino temp_piece;
    Tetromino released_piece;

    int gravity_threshold;

    int pr, pc;
    UINT16 cell_row, cell_col;
    int old_lines;
    UINT16 lines_this_drop;

    int center_x;
    int center_y;
    int i;

    long old_ssp;

    /* Track what was rendered to each buffer */
    Tetromino last_piece_buf1;
    Tetromino last_piece_buf2;
    int which_buffer; /* 0 = rendering to buffer1, 1 = rendering to buffer2*/
    int first_render;

    /* Double buffering setup */
    UINT32 *buffer1;
    UINT32 *buffer2;
    UINT32 *back_buffer;
    UINT32 *front_buffer;
    UINT32 *original_screen;
    UINT32 *temp_buffer;

    /* Get original fram buffer */
    original_screen = (UINT32 *)get_video_base();

    /* Align second buffer to 256-byte boundary */
    buffer1 = original_screen;
    buffer2 = (UINT32 *)(((UINT32)buffer_space + 255) & 0xFFFFFF00);

    front_buffer = buffer1;
    back_buffer = buffer2;
    
    /* Clearing both buffers */
    clear_screen(buffer1);
    clear_screen(buffer2);

    /* Show splash screen */
    if (!show_splash_screen((UINT8 *)buffer1)) {
        /* (User quit) */
        return 0;
    }

    /* Clearing both buffers */
    clear_screen(buffer1);
    clear_screen(buffer2);

    /* INITIALIZE GAME MODEL (Tetris State: START!) */
    init_model(&game_model);
        
        /* INITIALIZE Next + Hold Boxes */
    init_next_box(&game_model.nbox, peek_bag(&game_model));
    init_hold_box(&game_model.hbox);

    /* RENDER GAME MODEL everything to screen (Render State: FIRST FRAME) */
    render_matrix(back_buffer, &game_model.Matrix);
    render_next_box((UINT32 *) back_buffer, &game_model.nbox);
    render_hold_box(back_buffer, &game_model.hbox);
    render_piece(&game_model.piece, back_buffer);
    render_score(&game_model.game_state, (UINT8 *)back_buffer);
    render_level(&game_model.game_state, (UINT8 *)back_buffer);

    /* Display first frame */
    old_ssp = Super(0);
    set_video_base((UINT16 *)back_buffer);
    Super(old_ssp);
    wait_vbl();

    /* Swap buffers */
    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    /* Initialize buffer tracking */
    which_buffer = 1;
    last_piece_buf1 = game_model.piece;
    last_piece_buf2 = game_model.piece;
    first_render = 1;

    timeThen = get_time();

    /* Start tetris music */
    start_music();
    
    /* MAIN TETRIS GAME LOOP */
    while (quit != 1) {
        /* If Input Pending = Update Model Change REQUESTS */
        if (has_input()) {
            key = get_input(); /* store key press as a master key */

            if (key == 0) {
                char scan = get_scan_code();  /* update key with 2nd byte for correct comparison with arrow scan codes */

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
                if (key == 'c') {
                    game_model.request_hold = 1;
                }

                else if(key == ' ') {
                    game_model.request_hard_drop = 1;
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
    /* Get the last piece that was rendered to THIS buffer */
    Tetromino old_piece;
    if (first_render) {
        old_piece = game_model.piece;
        first_render = 0;
    } else {
        old_piece = (which_buffer == 0) ? last_piece_buf1 : last_piece_buf2;
    }

    /* Process asynchronous requests */
    if (game_model.request_move_left) {
        if (!check_collision(&game_model.Matrix, &game_model.piece, 
                            game_model.piece.col - 1, game_model.piece.row)) {
            move_tetromino_left(&game_model.piece);
        }
        game_model.request_move_left = 0;
    }

    if (game_model.request_move_right) {
        if (!check_collision(&game_model.Matrix, &game_model.piece, 
                            game_model.piece.col + 1, game_model.piece.row)) {
            move_tetromino_right(&game_model.piece);
        }
        game_model.request_move_right = 0;
    }

    if (game_model.request_rotate) {
        temp_piece = game_model.piece;
        rotate_tetromino_cw(&temp_piece);

        if (!check_collision(&game_model.Matrix, &temp_piece, temp_piece.col, temp_piece.row)) {
            game_model.piece = temp_piece;
        }
        game_model.request_rotate = 0;
    }

    if (game_model.request_soft_drop) {
        if (can_move_down(&game_model)) {
            move_tetromino_down(&game_model.piece);
        }
        game_model.request_soft_drop = 0;
    }

    temp_piece = game_model.piece;

    if (game_model.request_hold) {
    
        game_model.request_hold = 0;

        /* TO ENSURE NO INFINITE HOLDING */
        if (!game_model.hold_used) {

            if (hold_box_contains(&game_model.hbox)) {
                released_piece = release_tetromino(&game_model.hbox);
                hold_tetromino(&game_model.hbox, &temp_piece);

                /* RESET released_piece to CENTER of grid */
                released_piece.row = 0;
                released_piece.col = 3;
                released_piece.rotation = 0;

                game_model.piece = released_piece;
            }
            else {
                hold_tetromino(&game_model.hbox, &temp_piece);
                
                init_tetromino(&game_model.piece, next_piece_from_bag(&game_model), 3);
                game_model.hold_used = 0;
                init_next_box(&game_model.nbox, peek_bag(&game_model));
                game_model.redraw_next_box = 1;
            }
            game_model.redraw_hold_box = 1;
            game_model.hold_used = 1;
            play_effect_hold();
        }
    }

    if (game_model.request_hard_drop) {
        while (can_move_down(&game_model)) {
            move_tetromino_down(&game_model.piece);
        }

        lock_piece(&game_model.Matrix, &game_model.piece);
        game_model.redraw_matrix = 1;


        old_lines = game_model.game_state.lines_cleared;
        game_model.game_state.lines_cleared = clear_full_lines(&game_model.Matrix, 
            game_model.game_state.lines_cleared);
        lines_this_drop = game_model.game_state.lines_cleared - old_lines;

        if (game_model.game_state.lines_cleared > old_lines) {
            game_model.redraw_matrix = 1;
            game_model.redraw_score = 1;
            update_state(&game_model.game_state, lines_this_drop);

            if (lines_this_drop < 4) {
                play_effect_line_clear();
            }
            else {
                play_effect_tetris_clear();
            }
        }

        init_tetromino(&game_model.piece, next_piece_from_bag(&game_model), 3);
        game_model.hold_used = 0;
        init_next_box(&game_model.nbox, peek_bag(&game_model));
        game_model.redraw_next_box = 1;

        if (game_model.game_state.is_game_over) {
            quit = 1;
        }

        game_model.request_hard_drop = 0;
    }
    else {
        game_model.gravity_counter++;

        gravity_threshold = 50 - (game_model.game_state.level * 5);
        if (gravity_threshold < 5) {
            gravity_threshold = 5;
        }

        if (game_model.gravity_counter >= gravity_threshold) {
            old_lines = game_model.game_state.lines_cleared;
            handle_tick(&game_model);
            game_model.gravity_counter = 0;

            lines_this_drop = game_model.game_state.lines_cleared - old_lines;
            
            if (lines_this_drop > 0) {
                game_model.redraw_matrix = 1;
                game_model.redraw_score = 1;

                if (lines_this_drop < 4) {
                    play_effect_line_clear();
                }
                else {
                    play_effect_tetris_clear();
                }
            }


            if(game_model.game_state.is_game_over == 1) {
                quit = 1;
            }
        }
    }

    /* Render static elements to both buffers if dirty */
    if (game_model.redraw_matrix) {
        UINT32 *back_ptr;
        UINT32 *front_ptr;  
        int row, word;
        int bytes_per_row = 80;           /* atari ST mono = 640px / 8 = 80 bytes per scanline */
        int matrix_pixel_rows = MATRIX_ROWS * CELL_SIZE;
        int words_per_matrix_row = (MATRIX_COLS * CELL_SIZE) / 32; /* 32 bits per UINT32 */

        /*since memset isn't available on atari built our own function*/
        for (row = 0; row < matrix_pixel_rows; row++) {
             back_ptr  = (UINT32 *)((UINT8 *)back_buffer  + ((row + ROW_OFFSET) * bytes_per_row) + (COL_OFFSET / 8));
            front_ptr = (UINT32 *)((UINT8 *)front_buffer + ((row + ROW_OFFSET) * bytes_per_row) + (COL_OFFSET / 8));

            for (word = 0; word < words_per_matrix_row; word++) {
                back_ptr[word]  = 0;
                front_ptr[word] = 0;
            }
        }

        render_matrix(back_buffer, &game_model.Matrix);
        render_matrix(front_buffer, &game_model.Matrix);
        game_model.redraw_matrix = 0;
    }

    if (game_model.redraw_next_box) {
        clear_region(back_buffer, NEXT_BOX_ROW, NEXT_BOX_COL, NEXT_BOX_SIZE, NEXT_BOX_SIZE);
        clear_region(front_buffer, NEXT_BOX_ROW, NEXT_BOX_COL, NEXT_BOX_SIZE, NEXT_BOX_SIZE);

        render_next_box(back_buffer, &game_model.nbox);
        render_next_box(front_buffer, &game_model.nbox);
        game_model.redraw_next_box = 0;
    }

    if (game_model.redraw_hold_box) {
        clear_region(back_buffer, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);
        clear_region(front_buffer, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);

        render_hold_box(back_buffer, &game_model.hbox);
        render_hold_box(front_buffer, &game_model.hbox);
        game_model.redraw_hold_box = 0;
    }

    if (game_model.redraw_score) {
        /* clear score/level text region on both buffers first */
        clear_region(back_buffer, SCORE_Y, SCORE_X, 16, 150);
        clear_region(front_buffer, SCORE_Y, SCORE_X, 16, 150);
        clear_region(back_buffer, LEVEL_Y, LEVEL_X, 16, 150);
        clear_region(front_buffer, LEVEL_Y, LEVEL_X, 16, 150);

        render_score(&game_model.game_state, (UINT8 *)back_buffer);
        render_level(&game_model.game_state, (UINT8 *)back_buffer);
        render_score(&game_model.game_state, (UINT8 *)front_buffer);
        render_level(&game_model.game_state, (UINT8 *)front_buffer);
        game_model.redraw_score = 0;
    }

    /* Clear and redraw matrix region where old piece was */
    for (pr = 0; pr < 4; pr++) {
        for (pc = 0; pc < 4; pc++) {
            if (get_cell(&old_piece, pr, pc) != 0) {  /* ← ADD THIS LINE */
                int matrix_row = old_piece.row + pr;
                int matrix_col = old_piece.col + pc;
                
                if (matrix_row >= 0 && matrix_row < MATRIX_ROWS &&
                    matrix_col >= 0 && matrix_col < MATRIX_COLS) {
                    
                    cell_row = (UINT16)(matrix_row * CELL_SIZE) + ROW_OFFSET;
                    cell_col = (UINT16)(matrix_col * CELL_SIZE) + COL_OFFSET;
                     
                    clear_region(back_buffer, cell_row, cell_col, 
                                (UINT16)CELL_SIZE, (UINT16)CELL_SIZE);
                    
                    if (game_model.Matrix.cell[matrix_row][matrix_col] == 1) {
                        plot_rectangle(back_buffer, cell_row, cell_col, CELL_SIZE, CELL_SIZE);
                        
                        center_x = cell_col + (CELL_SIZE / 2) - 6;
                        center_y = cell_row + (CELL_SIZE / 2) - 6;

                        for (i = 0; i < 12; i++) {
                            plot_horizontal_line(back_buffer, center_y + i, center_x, 12);
                        }
                    }
                }
            }
        }
    }
    

    /* Render current piece to back buffer only */
    render_piece(&game_model.piece, back_buffer);

    /* Page flip */
    old_ssp = Super(0);
    set_video_base((UINT16 *)back_buffer);
    Super(old_ssp);
    wait_vbl();

    /* Save what we just rendered to THIS buffer */
    if (which_buffer == 0) {
        last_piece_buf1 = game_model.piece;
    } else {
        last_piece_buf2 = game_model.piece;
    }

    /* Swap buffers */
    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    /* Toggle which buffer we'll render to next */
    which_buffer = 1 - which_buffer;

    /* Update music */
    update_music(timeElapsed);

    timeThen = timeNow;
    }
}
    stop_music();
    printf("Game Over\n");

    old_ssp = Super(0);
    set_video_base((UINT16 *)original_screen);
    Super(old_ssp);
    
    return 0;
}
