#include "raster.h"
#include "types.h"

// Screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define WORDS_PER_ROW (SCREEN_WIDTH / 32)
#define BYTES_PER_ROW (SCREEN_WIDTH / 8)

// required font table goes here
#define FONT_ROWS 8
const UINT8 font_table[][FONT_ROWS] = {};

void clear_screen(UINT32 *base) {

}

void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width) {
    
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  r, c;
    UINT8  *row_ptr;

    for (r = row; r < row + length && r < SCREEN_HEIGHT; r++)
    {
        row_ptr = byte_base + (UINT32)r * BYTES_PER_ROW;
        for (c = col; c < col + width && c < SCREEN_WIDTH; c++)
        {
            UINT8 *byte_addr = row_ptr + (c >> 3);
            *byte_addr &= ~(0x80 >> (c & 7));
        }
    }
}

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col) {
    UINT8 *byte_address;
    UINT8 bit_offset;

    // Byte address calculation
    byte_address = base + (UINT32)row * BYTES_PER_ROW + (col >> 3);

    // Bit offset calculation
    bit_offset = col % 8;

    //Setting pixel
    *byte_address |= (0x80 >> bit_offset);
}

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length) {

}

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length) {
    // Using plot_pixel to create vertical line
    UINT16 i;
    for (i = 0 ; i < length ; i++) {
        plot_pixel((UINT8 *)base, row + i, col);    // wraps around

    }
}

void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width) {

    UINT8  *byte_base = (UINT8 *)base;
    UINT16  c, r;

    /* Top and bottom edges */
    for (c = col; c < col + width; c++)
    {
        plot_pixel(byte_base, row,              c);
        plot_pixel(byte_base, row + length - 1, c);
    }

    /* Left and right edges */
    for (r = row; r < row + length; r++)
    {
        plot_pixel(byte_base, r, col);
        plot_pixel(byte_base, r, col + width - 1);
    }
}

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side) {
    // Using plot_rectangle to create square
    plot_rectangle(base, row, col, side, side); // length == width
}

void plot_triangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 base, UINT16 height, UINT8 direction) {

}

void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height) {

}

void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height, UINT16 *bitmap){

    UINT8 *byte_base = (UINT8 *) base;
    UINT16  r, b;
    UINT16  word;
    UINT16  mask;

    for (r = 0 ; r < height ; r++) {
        word = bitmap[r];
        mask = 0x8000; // leftmost bit

        for (b = 0 ; b < 16 ; b++) {
            if (word & mask) 
                plot_pixel(byte_base, row + r, col + b);
            mask >>= 1;
        }
    }
}

void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col, UINT16 height, const UINT32 *bitmap){
    
    UINT8  *byte_base = (UINT8) *base;
    UINT16  r, b;
    UINT32  word;
    UINT32  mask;

    for (r = 0; r < height; r++)
    {
        word = bitmap[r];
        mask = 0x80000000UL;

        for (b = 0; b < 32; b++)
        {
            if (word & mask)
                plot_pixel(byte_base, row + r, col + b);
            mask >>= 1;
        }
    }
}

void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch) {
    
    const UINT8 *glyph = get_glyph(ch);
    UINT16 r, b;
    UINT8  row_bits;
    UINT8  mask;

    for (r = 0; r < FONT_ROWS; r++)
    {
        row_bits = glyph[r];
        mask     = 0x80;
        for (b = 0; b < 8; b++)
        {
            if (row_bits & mask)
                plot_pixel(base, row + r, col + b);
            mask >>= 1;
        }
    }

}

void plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch){

    while (*ch != '\0')
    {
        plot_character(base, row, col, *ch);
        col += 8;   /* advance one character width */
        ch++;
    }
}
