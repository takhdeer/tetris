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
    // using clear_region "on the defined whole screen dimensions"
    clear_region(base, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
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
    // using plot_pixel to create a horizontal line
    UINT16 i;
    for (i = 0; i < length; i++) {
        if (row < SCREEN_HEIGHT && (col + i) < SCREEN_WIDTH) {
            plot_pixel((UINT8 *)base, row, col + i);
        }
    }
}

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length) {
    // Using plot_pixel to create vertical line
    UINT16 i;
    for (i = 0 ; i < length ; i++) {
        plot_pixel((UINT8 *)base, row + i, col);    // wraps around

    }
}

void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col, UINT16 end_row, UINT16 end_col) {
    // below are int types due to these values potentially holding negative value
    int dx = end_col - start_col;
    int dy = end_row - start_row;
    int sx, sy; // step direction
    int err, e2;

    UINT8 *byte_base = (UINT8 *)base;
    if (dx > 0) {
        sx = 1;
    }
    else {
        sx = -1;
    }

    if (dy > 0) {
        sy = 1;
    }
    else {
        sy = -1;
    }

    // ensure negative coordinates are negative numerically
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }

    err = dx - dy; // initial error

    while (1) {
        // clipping/boundary checking:
        if (start_col < SCREEN_WIDTH && start_row < SCREEN_HEIGHT) {
            plot_pixel(byte_base, start_row, start_col);
        }

        // @ here, reached end point
        if (start_col == end_col && start_row == end_row) {
            break;
        }

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            start_col += sx;
        }
        if (e2 < dx) {
            err += dx;
            start_row += sy;
        }
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
    // using plot_horizontal_line to create triangle
    UINT16 i;

    // '0' = plot horizontal line start from top left
    if (direction == 0) {
        for (i = 0; i < height; i++) {
            plot_horizontal_line(base, row + i, col, i + 1);
        }
    }
    // '1' = plot horizontal line start from top right
    else if (direction == 1) {
        for (i = 0; i < height; i++) {
            plot_horizontal_line(base, row + i, col - i, i + 1);
        }
    }
    // '2' = plot horizontal line start from bottom left
    else if (direction == 2) {
        for (i = 0; i < height; i++) {
            plot_horizontal_line(base, row + i, col, height - i);
        }
    }
    // '3' = plot horizontal line from bottom right
    else if (direction == 3) {
        for (i = 0; i < height; i++) {
            plot_horizontal_line(base, row + i, col + i, height - i);
        }
    }
}

void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height) {
    UINT8 *byte_base = base;
    UINT16 r, b;
    UINT8 byte;
    UINT8 mask;

    // no bitmap parameter in function, must hardcode a bitmap array:
    const UINT8 bitmap[] = {0xFF, 0x81, 0x81, 0xFF};

    for (r = 0; r < height; r++) {
        byte = bitmap[r];
        mask = 0x80;
        for (b = 0; b < 8; b++) {
            if (byte & mask) {
                if ( (row + r) < SCREEN_HEIGHT && (col + b) < SCREEN_WIDTH) {
                plot_pixel(byte_base, row + r, col + b);
                }
            }
            mask >>=1;
        }
    }
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
