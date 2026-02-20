// TEMPORARY FILE!
// Copy and paste to actual raster.c file afterwards

// Author: Aaron

#include "raster.h"

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length) {
    // Using plot_pixel to create vertical line
    UINT16 i;
    for (i = 0 ; i < length ; i++) {
        plot_pixel((UINT8 *)base, row + i, col);    // wraps around

    }
}

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side)
{
    // Using plot_rectangle to create square
    plot_rectangle(base, row, col, side, side); // length == width
}

// Global bitnmap data
extern const UINT16 bitmap_16_data[];
void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height)
{
    // Screen is 640px wide (40 words (UINT16s) per row)
    UINT16 i;
    UINT16 *screen_pos;

    for (i = 0 ; i < height ; i++) {
        screen_pos = base + (row + i) * 40 + (col >> 4); // converts pixel column to word offset
        *screen_pos |= bitmap_16_data[i]; // OR the bitmap data onto the screen
    }
}

extern const UINT8 font[256][16];
void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch)
{
    // 8x16 px character from the font table
    // Screen (640 / 8 = 80) is 80 bytes per row
    UINT16 i;
    UINT8 *screen_pos;

    for (i = 0 ; i < 16 ; i++) {
        screen_pos = base + (row + i) * 80 + (col >> 3); // converts pixel col to byte offset
        *screen_pos |= font[(unsigned char)ch][i]; // OR the font row onto screen
    }
}