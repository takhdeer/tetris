/**
 * splash.c - Splash screen implementation
 * Authors: Aaron, Takhdeer, Henry
 */
#include "splash.h"
#include "input.h"
#include <osbind.h>
#include "renderer.h"
#include "raster.h"

#define SCREEN_W 640
#define SCREEN_H 400

/* ── Helpers ── */

static void filled_rect(UINT32 *base, UINT16 top, UINT16 left, UINT16 bottom, UINT16 right) {
    UINT16 row;
    for (row = top; row <= bottom; row++)
        plot_horizontal_line(base, row, left, right - left);
}

static void draw_circle(UINT8 *base, UINT16 cy, UINT16 cx, UINT16 r) {
    int x = 0, y = (int)r, d = 1 - (int)r;
    while (x <= y) {
        plot_pixel(base, cy - y, cx + x);
        plot_pixel(base, cy - y, cx - x);
        plot_pixel(base, cy + y, cx + x);
        plot_pixel(base, cy + y, cx - x);
        plot_pixel(base, cy - x, cx + y);
        plot_pixel(base, cy - x, cx - y);
        plot_pixel(base, cy + x, cx + y);
        plot_pixel(base, cy + x, cx - y);
        if (d < 0) d += 2 * x + 3;
        else { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

/*
 * Draws the onion dome shape above a tower.
 * top_row/col: tip of the dome, dome_w: max width at base of dome
 */
static void draw_onion_dome(UINT32 *base, UINT16 top_row, UINT16 centre_col, UINT16 dome_h, UINT16 dome_w) {
    UINT16 row;
    int half;
    for (row = 0; row < dome_h; row++) {
        /* Bulge profile: wide in middle, narrow at top and base */
        if (row < dome_h / 2)
            half = (int)dome_w * row / (dome_h / 2) / 2;
        else
            half = (int)dome_w * (dome_h - row) / (dome_h / 2) / 2;
        if (half < 1) half = 1;
        plot_horizontal_line(base, top_row + row, centre_col - half, half * 2);
    }
}

/*
 * Draws the full Kremlin-style temple:
 *   - central tall tower with onion dome + spire
 *   - two flanking smaller towers with small domes
 *   - base connecting block
 * anchor_row: the ground line row
 * anchor_col: left edge of the whole structure
 */
static void draw_temple(UINT32 *base, UINT16 ground_row, UINT16 anchor_col) {
    UINT16 centre_col = anchor_col + 60;  /* centre of main tower */

    /* Base platform */
    filled_rect(base, ground_row - 15, anchor_col,      ground_row, anchor_col + 120);

    /* Left flanking tower */
    filled_rect(base, ground_row - 55, anchor_col + 5,  ground_row - 15, anchor_col + 30);
    draw_onion_dome(base, ground_row - 75, anchor_col + 17, 20, 18);

    /* Right flanking tower */
    filled_rect(base, ground_row - 55, anchor_col + 90, ground_row - 15, anchor_col + 115);
    draw_onion_dome(base, ground_row - 75, anchor_col + 102, 20, 18);

    /* Main centre tower */
    filled_rect(base, ground_row - 100, centre_col - 20, ground_row - 15, centre_col + 20);

    /* Main onion dome */
    draw_onion_dome(base, ground_row - 135, centre_col, 35, 28);

    /* Spire above main dome */
    plot_vertical_line(base, ground_row - 160, centre_col,     25);
    plot_vertical_line(base, ground_row - 160, centre_col - 1, 20);
    plot_vertical_line(base, ground_row - 160, centre_col + 1, 20);
}

static void draw_moon(UINT8 *base) {
    draw_circle(base, 130, 100, 30);
    draw_circle(base, 130, 100, 29);
}

static void draw_stars(UINT8 *base) {
    /* 2x2 pixel stars in the sky region */
    plot_pixel(base, 120, 220); plot_pixel(base, 121, 220);
    plot_pixel(base, 120, 221); plot_pixel(base, 121, 221);

    plot_pixel(base, 145, 310); plot_pixel(base, 146, 310);
    plot_pixel(base, 145, 311); plot_pixel(base, 146, 311);

    plot_pixel(base, 160, 180);
    plot_pixel(base, 175, 390);
    plot_pixel(base, 135, 450);
    plot_pixel(base, 200, 260);
    plot_pixel(base, 155, 500);
}

int show_splash_screen(UINT8 *base) {
    char key;

    clear_screen((UINT32 *)base);

    /* Outer border */
    plot_rectangle((UINT32 *)base, 5, 5, 390, 630);

    /* Title band top/bottom lines (double for thickness) */
    plot_horizontal_line((UINT32 *)base, 10, 10, 620);
    plot_horizontal_line((UINT32 *)base, 11, 10, 620);
    plot_horizontal_line((UINT32 *)base, 95, 10, 620);
    plot_horizontal_line((UINT32 *)base, 96, 10, 620);

    /* Title - bold effect */
    plot_string((UINT8 *)base, 40, 287, "TETRONAS");
    plot_string((UINT8 *)base, 41, 287, "TETRONAS");
    plot_string((UINT8 *)base, 40, 288, "TETRONAS");

    /* Subtitle */
    plot_string((UINT8 *)base, 73, 262, "COMP 2659  2025");

    /* Ground line */
    plot_horizontal_line((UINT32 *)base, 295, 10, 620);
    plot_horizontal_line((UINT32 *)base, 296, 10, 620);

    /* Temple on the right */
    draw_temple((UINT32 *)base, 295, 470);

    /* Moon and stars on the left */
    draw_moon((UINT8 *)base);
    draw_stars((UINT8 *)base);

    /* Menu divider */
    plot_horizontal_line((UINT32 *)base, 310, 10, 620);

    /* Menu */
    plot_string((UINT8 *)base, 330, 256, "PRESS 1 TO START");
    plot_string((UINT8 *)base, 358, 260, "PRESS Q TO QUIT");

    while (1) {
        if (has_input()) {
            key = get_input();
            if (key == '1') return 1;
            if (key == 'q') return 0;
        }
    }

    return 0;
}