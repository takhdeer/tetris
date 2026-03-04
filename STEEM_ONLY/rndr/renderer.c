
#include "renderer.h"
#include "raster.h"

#define CELL_SIZE 16
#define NEXT_BOX_ROW 50
#define NEXT_BOX_COL 500
#define NEXT_BOX_SIZE 80

#define HOLD_BOX_ROW 320
#define HOLD_BOX_COL 500
#define HOLD_BOX_SIZE (4 * CELL_SIZE)

#define SCORE_X 200   /* score x position */
#define SCORE_Y 10    /* score y position */
#define LEVEL_X 200
#define LEVEL_Y 30
#define LINES_X 200
#define LINES_Y 50

void render_next_box(UINT32 *base, const NextBox *box) { 
    Tetromino preview_piece;
    int next_type;

    next_type = get_next_type(box);
    
    init_tetromino(&preview_piece, next_type, (NEXT_BOX_COL + 16) / CELL_SIZE);
    preview_piece.row = (NEXT_BOX_ROW + 16) / CELL_SIZE;

    plot_rectangle(base, NEXT_BOX_ROW, NEXT_BOX_COL, NEXT_BOX_SIZE, NEXT_BOX_SIZE);
    render_piece(&preview_piece, base);
}

/* =========================================== */

void render_matrix(UINT32 *base, const Matrix *gameGrid) {
    UINT16 pixel_row, pixel_col;
    int r, c, i;

    for (r = 0; r < MATRIX_ROWS; r++) {
        for (c = 0; c < MATRIX_COLS; c++) {
            pixel_row = r * CELL_SIZE;
            pixel_col = c * CELL_SIZE;
            /* Case 1: cell in Matrix holds a 1 = filled cell */
            if (gameGrid->cell[r][c] == 1) {
                /* plot SOLID filled block to FrameBuffer */
                for (i = 0; i < CELL_SIZE; i++) {
                    plot_horizontal_line(base, pixel_row + i, pixel_col, CELL_SIZE);
                }
            }
            /* Case 2: cell in Matrix holds a 0 = empty cell */
            else {
                /* plot OUTLINE block only to FrameBuffer */
                plot_square(base, pixel_row, pixel_col, CELL_SIZE);
            }
        }
    }
}

void render_hold_box(UINT32 *base, const HoldBox *heldbox) {
    Tetromino temp_piece;

    /* Case 1: render an occupied HoldBox with its current Tetromino */
    if (heldbox->contains == 1) {
        /* extract held piece and its coordinates */
        temp_piece = heldbox->piece_held;
        temp_piece.row = HOLD_BOX_ROW / CELL_SIZE;
        temp_piece.col = HOLD_BOX_COL / CELL_SIZE;

        /* plot to FrameBuffer */
        plot_rectangle(base, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);
        render_piece(&temp_piece, (UINT32*)base);
    }
    /* Case 2: render an empty HoldBox */
    else {
        /* plot to FrameBuffer */
        plot_rectangle(base, HOLD_BOX_ROW, HOLD_BOX_COL, HOLD_BOX_SIZE, HOLD_BOX_SIZE);
    }
}

/* =========================================== */

void render_piece(const Tetromino *piece, UINT32 *base) {
    int piece_row;
    int piece_col;
    int screen_x;
    int screen_y;
    int line;

    for (piece_row = 0; piece_row < 4; piece_row++) {
        for (piece_col = 0; piece_col < 4; piece_col++) {
            /* If cell is part of the piece */
            if (get_cell(piece, piece_row, piece_col) != 0) {
                screen_x = (piece->col + piece_col) * BLOCK_SIZE;
                screen_y = (piece->row + piece_row) * BLOCK_SIZE;

                for (line = 0; line < BLOCK_SIZE; line++) {
                    plot_horizontal_line(base, screen_y + line, screen_x, BLOCK_SIZE);
                }
            }
        }
    }
}

void uint_to_str(UINT32 value, char *buf, int start) {
    int i = start + 5;
    buf[i] = '\0';

    if (value == 0) {
        buf[--i] = '0';
    }
    else {
        while (value > 0 && i > start) {
            buf[--i] = '0' + (value % 10);
            value /= 10;
        }
    }

    /* Shift digits left to sit right after the prefix */
    {
        int src = i, dst = start;
        while (buf[src] != '\0')
            buf[dst++] = buf[src++];
        buf[dst] = '\0';
    }
}

void render_score(const GameState *game, UINT8 *base) {
    char score_text[20];

    score_text[0] = 'S';
    score_text[1] = 'C';
    score_text[2] = 'O';
    score_text[3] = 'R';
    score_text[4] = 'E';
    score_text[5] = ':';
    score_text[6] = ' ';

    uint_to_str(game->score, score_text, 7);

    plot_string(base, SCORE_Y, SCORE_X, score_text);
}

void render_lines(const GameState *game, UINT8 *base) {
    char lines_text[20];

    lines_text[0] = 'L';
    lines_text[1] = 'I';
    lines_text[2] = 'N';
    lines_text[3] = 'E';
    lines_text[4] = 'S';
    lines_text[5] = ':';
    lines_text[6] = ' ';

    uint_to_str(game->lines_cleared, lines_text, 7);

    plot_string(base, LINES_Y, LINES_X, lines_text);
}

void render_level(const GameState *game, UINT8 *base) {
    char level_text[20];

    level_text[0] = 'L';
    level_text[1] = 'E';
    level_text[2] = 'V';
    level_text[3] = 'E';
    level_text[4] = 'L';
    level_text[5] = ':';
    level_text[6] = ' ';

    uint_to_str(game->level, level_text, 7);

    plot_string(base, LEVEL_Y, LEVEL_X, level_text);
}

void render_game_over(UINT8 *base, int score) {
    char score_str[16];

    /* rendering border for game over box */
    plot_rectangle((UINT32 *) base, 170, 220, 50, 200);

    /* rendering game over text */
    plot_string(base, 180, 240, "GAME OVER");

    /* Convert score to integer to string manually */
    score_str[0] = 'S';
    score_str[1] = 'C';
    score_str[2] = 'O';
    score_str[3] = 'R';
    score_str[4] = 'E';
    score_str[5] = ':';
    score_str[6] = ' ';

    uint_to_str((UINT32) score, score_str, 7);

    plot_string(base, 200, 256, score_str);
}