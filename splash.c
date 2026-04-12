/**
 * splash.c - Splash screen implementation
 * Authors: Aaron, Takhdeer, Henry
 */

#include "splash.h"
#include "input.h"
#include <osbind.h>
#include "renderer.h"
#include "raster.h"
#include "ikbd.h"

#define SCREEN_W 640
#define SCREEN_H 400

/* Button dimensions */
#define BTN_W 160
#define BTN_H 30
#define BTN_COL 240
#define BTN1_ROW 325 /* 1 PLAYER button */
#define BTN2_ROW 362 /* QUIT button */

/* Mouse pointer: simple arrow, 8x8 */
#define PTR_W 8
#define PTR_H 8

static const UINT8 mouse_pointer[PTR_H] = {
    0x80, /* 1000 0000 */
    0xC0, /* 1100 0000 */
    0xE0, /* 1110 0000 */
    0xF0, /* 1111 0000 */
    0xF8, /* 1111 1000 */
    0xE0, /* 1110 1000 */
    0xA0, /* 1010 0000 */
    0X00
};

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

static void draw_background(UINT8 *base) {
    clear_screen((UINT32 *)base);

    /* Outer border*/
    plot_rectangle((UINT32 *)base, 5, 5, 390, 630);

    /* Title band top/bottom lines (double for thickness) */
    plot_horizontal_line((UINT32 *)base, 10, 10, 620);
    plot_horizontal_line((UINT32 *)base, 11, 10, 620);
    plot_horizontal_line((UINT32 *)base, 95, 10, 620);
    plot_horizontal_line((UINT32 *)base, 96, 10, 620);

    /* Title - bold effect */
    plot_string(base, 40, 287, "TETRONAS");
    plot_string(base, 41, 287, "TETRONAS");
    plot_string(base, 40, 288, "TETRONAS");

    /* Subtitle */
    plot_string(base, 73, 262, "COMP 2659  2025");

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

    /* 1 PLAYER button */
    plot_rectangle((UINT32 *)base, BTN1_ROW, BTN_COL, BTN_H, BTN_W);
    plot_string(base, BTN1_ROW + 11, BTN_COL + 40, "1 PLAYER");

    /* QUIT button */
    plot_rectangle((UINT32 *)base, BTN2_ROW, BTN_COL, BTN_H, BTN_W);
    plot_string(base, BTN2_ROW + 11, BTN_COL + 56, "QUIT");
}

static void draw_pointer(UINT8 *base, UINT16 px, UINT16 py) {
    int r, b;
    UINT8 byte, mask;
    UINT8 *addr;
    for (r = 0; r < PTR_H; r++) {
        byte = mouse_pointer[r];
        mask = 0x80;
        for (b = 0; b < PTR_W; b++) {
            if (byte & mask) {
                addr = base + ((UINT32)(py + r) * 80) + ((px + b) >> 3);
                *addr ^= (0x80 >> ((px + b) & 7));
            }
            mask >>= 1;
        }
    }
}

static void erase_pointer(UINT8 *base, UINT16 px, UINT16 py) {
    draw_pointer(base, px, py);
}

static int in_button(int px, int py, UINT16 btn_row, UINT16 btn_col, UINT16 btn_h, UINT16 btn_w) {
    return (px >= btn_col && px <= btn_col + btn_w && py >= btn_row && py <=btn_row + btn_h);
}

/**
 * Displays splash screen wit a mouse-driven main menu.
 * Returns: 1 to start game, 0 to quit
 */
int show_splash_screen(UINT8 *base) {
    UINT16 ptr_x;
    UINT16 ptr_y;
    UINT16 prev_x;
    UINT16 prev_y;
    int left_prev;

    draw_background(base);

    prev_x = (UINT16)mouse_x;
    prev_y = (UINT16)mouse_y;
    left_prev = 0;

    draw_pointer(base, prev_x, prev_y);


    while (1) {
        ptr_x = (UINT16)mouse_x;
        ptr_y = (UINT16)mouse_y;

        /* redraw pointer if it moves */
        if (ptr_x != prev_x || ptr_y != prev_y) {
            erase_pointer(base, prev_x, prev_y);
            draw_pointer(base, ptr_x, ptr_y);
            prev_x = ptr_x;
            prev_y = ptr_y;
        }

        /* Checking for left click*/
        if (mouse_buttons == 0xFA) {
            if (!left_prev) {
                if (in_button(ptr_x, ptr_y, BTN1_ROW, BTN_COL, BTN_H, BTN_W)) {
                    return 1;
                }
                if (in_button(ptr_x, ptr_y, BTN2_ROW, BTN_COL, BTN_H, BTN_W)) {
                    return 0;
                }
            }
            left_prev = 1;
        } 
        else {
            left_prev = 0;
        }

        /* allow spacebar / Q still (for fallback) */
        if (has_input()) {
            char key = get_input();
            if (key == ' ') {
                return 1;
            }
            if (key == 'q') {
                return 0;
            }
        }
    }
    return 0;
}