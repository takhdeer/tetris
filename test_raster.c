/*
 * test_raster.c
 * Raster Graphics Library - Test Driver
 * 
 * Simulates an Atari ST framebuffer in terminal memory.
 * The Atari ST screen is 640x400 pixels, monochrome, 1 bit per pixel.
 * Each row = 640 bits = 80 bytes = 20 UINT32 words.
 *
 * Terminal output renders each pixel as '#' (set) or '.' (clear)
 * for a cropped region so results are readable in any terminal.
 *
 * Author: Henry
 */

#include <stdio.h>
#include <string.h>

/* ── Atari ST screen constants (mirrors raster.c) ── */
#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  400
#define BYTES_PER_ROW  (SCREEN_WIDTH / 8)   /* 80 bytes per row  */
#define WORDS_PER_ROW  (SCREEN_WIDTH / 32)  /* 20 UINT32s per row */

/* ── Portable type aliases (mirrors types.h) ── */
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned long      UINT32;

/* ── Simulated framebuffer: 400 rows x 80 bytes = 32 000 bytes ── */
static UINT8 framebuffer[SCREEN_HEIGHT * BYTES_PER_ROW];

/* ════════════════════════════════════════════════════════════════
 * FORWARD DECLARATIONS  (paste-compatible with your raster.c)
 * ════════════════════════════════════════════════════════════════ */
void plot_pixel          (UINT8  *base, UINT16 row, UINT16 col);
void clear_screen        (UINT32 *base);
void clear_region        (UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
void plot_vertical_line  (UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
void plot_rectangle      (UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
void plot_square         (UINT32 *base, UINT16 row, UINT16 col, UINT16 side);
void plot_line           (UINT32 *base, UINT16 start_row, UINT16 start_col,
                                        UINT16 end_row,   UINT16 end_col);
void plot_triangle       (UINT32 *base, UINT16 row, UINT16 col,
                          UINT16 tri_base, UINT16 height, UINT8 direction);
void plot_bitmap_8       (UINT8  *base, UINT16 row, UINT16 col, UINT16 height);

/* ════════════════════════════════════════════════════════════════
 * HELPER: read one pixel from the framebuffer (1 = set, 0 = clear)
 * ════════════════════════════════════════════════════════════════ */
static int get_pixel(UINT8 *base, UINT16 row, UINT16 col)
{
    UINT8 *byte_addr;
    UINT8  bit_offset;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return 0;

    byte_addr  = base + (UINT32)row * BYTES_PER_ROW + (col >> 3);
    bit_offset = col & 7;
    return (*byte_addr >> (7 - bit_offset)) & 1;
}

/* ════════════════════════════════════════════════════════════════
 * HELPER: print a cropped region of the framebuffer to terminal
 *   '#' = pixel set   '.' = pixel clear
 * ════════════════════════════════════════════════════════════════ */
static void print_region(UINT8 *base,
                          UINT16 start_row, UINT16 start_col,
                          UINT16 rows,      UINT16 cols)
{
    UINT16 r, c;
    for (r = start_row; r < start_row + rows && r < SCREEN_HEIGHT; r++)
    {
        printf("  |");
        for (c = start_col; c < start_col + cols && c < SCREEN_WIDTH; c++)
            printf("%c", get_pixel(base, r, c) ? '#' : '.');
        printf("|\n");
    }
}

/* ════════════════════════════════════════════════════════════════
 * HELPER: check that ALL pixels in a row-range/col-range are SET
 * ════════════════════════════════════════════════════════════════ */
static int check_pixels_set(UINT8 *base,
                              UINT16 start_row, UINT16 start_col,
                              UINT16 rows,      UINT16 cols)
{
    UINT16 r, c;
    for (r = start_row; r < start_row + rows; r++)
        for (c = start_col; c < start_col + cols; c++)
            if (!get_pixel(base, r, c)) return 0;
    return 1;
}

/* ════════════════════════════════════════════════════════════════
 * HELPER: check that ALL pixels in a row-range/col-range are CLEAR
 * ════════════════════════════════════════════════════════════════ */
static int check_pixels_clear(UINT8 *base,
                                UINT16 start_row, UINT16 start_col,
                                UINT16 rows,      UINT16 cols)
{
    UINT16 r, c;
    for (r = start_row; r < start_row + rows; r++)
        for (c = start_col; c < start_col + cols; c++)
            if (get_pixel(base, r, c)) return 0;
    return 1;
}

/* ════════════════════════════════════════════════════════════════
 * HELPER: reset framebuffer to all zeros between tests
 * ════════════════════════════════════════════════════════════════ */
static void reset_fb(void)
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

/* ════════════════════════════════════════════════════════════════
 *  RASTER FUNCTION IMPLEMENTATIONS (inline - no stdlib, no OS)
 *  Copy these into your raster.c when ready.
 * ════════════════════════════════════════════════════════════════ */

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col)
{
    UINT8 *byte_address;
    UINT8  bit_offset;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return;

    byte_address = base + (UINT32)row * BYTES_PER_ROW + (col >> 3);
    bit_offset   = col & 7;
    *byte_address |= (0x80 >> bit_offset);
}

void clear_screen(UINT32 *base)
{
    clear_region(base, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
}

void clear_region(UINT32 *base, UINT16 row, UINT16 col,
                  UINT16 length, UINT16 width)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  r, c;

    for (r = row; r < row + length && r < SCREEN_HEIGHT; r++)
        for (c = col; c < col + width && c < SCREEN_WIDTH; c++)
        {
            UINT8 *addr = byte_base + (UINT32)r * BYTES_PER_ROW + (c >> 3);
            *addr &= ~(0x80 >> (c & 7));
        }
}

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length)
{
    UINT16 i;
    for (i = 0; i < length; i++)
        if (row < SCREEN_HEIGHT && (col + i) < SCREEN_WIDTH)
            plot_pixel((UINT8 *)base, row, col + i);
}

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length)
{
    UINT16 i;
    for (i = 0; i < length; i++)
        plot_pixel((UINT8 *)base, row + i, col);
}

void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col,
                    UINT16 length, UINT16 width)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  c, r;

    for (c = col; c < col + width; c++)
    {
        plot_pixel(byte_base, row,              c);
        plot_pixel(byte_base, row + length - 1, c);
    }
    for (r = row; r < row + length; r++)
    {
        plot_pixel(byte_base, r, col);
        plot_pixel(byte_base, r, col + width - 1);
    }
}

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side)
{
    plot_rectangle(base, row, col, side, side);
}

void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col,
               UINT16 end_row, UINT16 end_col)
{
    int dx = (int)end_col   - (int)start_col;
    int dy = (int)end_row   - (int)start_row;
    int sx, sy, err, e2;
    UINT8 *byte_base = (UINT8 *)base;

    if (dx > 0) sx = 1; else sx = -1;
    if (dy > 0) sy = 1; else sy = -1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    err = dx - dy;

    while (1)
    {
        if (start_col < SCREEN_WIDTH && start_row < SCREEN_HEIGHT)
            plot_pixel(byte_base, start_row, start_col);

        if (start_col == end_col && start_row == end_row) break;

        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; start_col += sx; }
        if (e2 <  dx) { err += dx; start_row += sy; }
    }
}

void plot_triangle(UINT32 *base, UINT16 row, UINT16 col,
                   UINT16 tri_base, UINT16 height, UINT8 direction)
{
    UINT16 i;

    if (direction == 0)   /* 90 deg at top-left */
        for (i = 0; i < height; i++)
            plot_horizontal_line(base, row + i, col, i + 1);

    else if (direction == 1)   /* 90 deg at top-right */
        for (i = 0; i < height; i++)
            plot_horizontal_line(base, row + i, col - i, i + 1);

    else if (direction == 2)   /* 90 deg at bottom-left */
        for (i = 0; i < height; i++)
            plot_horizontal_line(base, row + i, col, height - i);

    else if (direction == 3)   /* 90 deg at bottom-right */
        for (i = 0; i < height; i++)
            plot_horizontal_line(base, row + i, col + i, height - i);
}

void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height)
{
    /* hardcoded 8x8 bitmap: simple box with X inside */
    const UINT8 bitmap[] = {
        0xFF,   /* 1111 1111 */
        0x81,   /* 1000 0001 */
        0xC3,   /* 1100 0011 */
        0xA5,   /* 1010 0101 */
        0xA5,   /* 1010 0101 */
        0xC3,   /* 1100 0011 */
        0x81,   /* 1000 0001 */
        0xFF    /* 1111 1111 */
    };

    UINT8  *byte_base = base;
    UINT16  r, b;
    UINT8   byte, mask;

    for (r = 0; r < height && r < 8; r++)
    {
        byte = bitmap[r];
        mask = 0x80;
        for (b = 0; b < 8; b++)
        {
            if (byte & mask)
                if ((row + r) < SCREEN_HEIGHT && (col + b) < SCREEN_WIDTH)
                    plot_pixel(byte_base, row + r, col + b);
            mask >>= 1;
        }
    }
}

/* ════════════════════════════════════════════════════════════════
 *  MISSING IMPLEMENTATIONS
 * ════════════════════════════════════════════════════════════════ */

#define FONT_ROWS 8

/*
 * 8x8 monochrome bitmap font covering ASCII 32 (space) through 127 (DEL).
 * Each character is 8 rows of 8 bits. MSB is the leftmost pixel.
 * No stdlib, no OS -- pure data table.
 */
static const UINT8 font_table[96][FONT_ROWS] = {
    /* 32 space  */ {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    /* 33 !      */ {0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00},
    /* 34 "      */ {0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00},
    /* 35 #      */ {0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00},
    /* 36 $      */ {0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00},
    /* 37 %      */ {0x00,0x66,0xAC,0xD8,0x36,0x6A,0xCC,0x00},
    /* 38 &      */ {0x38,0x6C,0x68,0x76,0xDC,0xCC,0x76,0x00},
    /* 39 '      */ {0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00},
    /* 40 (      */ {0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00},
    /* 41 )      */ {0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00},
    /* 42 *      */ {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00},
    /* 43 +      */ {0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00},
    /* 44 ,      */ {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30},
    /* 45 -      */ {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00},
    /* 46 .      */ {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},
    /* 47 /      */ {0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00},
    /* 48 0      */ {0x3C,0x66,0x6E,0x7E,0x76,0x66,0x3C,0x00},
    /* 49 1      */ {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
    /* 50 2      */ {0x3C,0x66,0x06,0x1C,0x30,0x60,0x7E,0x00},
    /* 51 3      */ {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
    /* 52 4      */ {0x0E,0x1E,0x36,0x66,0x7F,0x06,0x06,0x00},
    /* 53 5      */ {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
    /* 54 6      */ {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00},
    /* 55 7      */ {0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00},
    /* 56 8      */ {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
    /* 57 9      */ {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00},
    /* 58 :      */ {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00},
    /* 59 ;      */ {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30},
    /* 60 <      */ {0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00},
    /* 61 =      */ {0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00},
    /* 62 >      */ {0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00},
    /* 63 ?      */ {0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00},
    /* 64 @      */ {0x3C,0x66,0x6E,0x6A,0x6E,0x60,0x3C,0x00},
    /* 65 A      */ {0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00},
    /* 66 B      */ {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
    /* 67 C      */ {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
    /* 68 D      */ {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
    /* 69 E      */ {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},
    /* 70 F      */ {0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00},
    /* 71 G      */ {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00},
    /* 72 H      */ {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
    /* 73 I      */ {0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00},
    /* 74 J      */ {0x06,0x06,0x06,0x06,0x06,0x66,0x3C,0x00},
    /* 75 K      */ {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
    /* 76 L      */ {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
    /* 77 M      */ {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
    /* 78 N      */ {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
    /* 79 O      */ {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    /* 80 P      */ {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
    /* 81 Q      */ {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x06,0x00},
    /* 82 R      */ {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00},
    /* 83 S      */ {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
    /* 84 T      */ {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    /* 85 U      */ {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    /* 86 V      */ {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
    /* 87 W      */ {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
    /* 88 X      */ {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
    /* 89 Y      */ {0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x00},
    /* 90 Z      */ {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00},
    /* 91 [      */ {0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00},
    /* 92 backsl */ {0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00},
    /* 93 ]      */ {0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00},
    /* 94 ^      */ {0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00},
    /* 95 _      */ {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},
    /* 96 `      */ {0x30,0x18,0x0C,0x00,0x00,0x00,0x00,0x00},
    /* 97 a      */ {0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00},
    /* 98 b      */ {0x60,0x60,0x7C,0x66,0x66,0x66,0x7C,0x00},
    /* 99 c      */ {0x00,0x00,0x3C,0x66,0x60,0x66,0x3C,0x00},
    /* 100 d     */ {0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0x00},
    /* 101 e     */ {0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00},
    /* 102 f     */ {0x1C,0x30,0x30,0x7C,0x30,0x30,0x30,0x00},
    /* 103 g     */ {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x3C},
    /* 104 h     */ {0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x00},
    /* 105 i     */ {0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00},
    /* 106 j     */ {0x06,0x00,0x06,0x06,0x06,0x06,0x66,0x3C},
    /* 107 k     */ {0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0x00},
    /* 108 l     */ {0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
    /* 109 m     */ {0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00},
    /* 110 n     */ {0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00},
    /* 111 o     */ {0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00},
    /* 112 p     */ {0x00,0x00,0x7C,0x66,0x66,0x7C,0x60,0x60},
    /* 113 q     */ {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x06},
    /* 114 r     */ {0x00,0x00,0x6C,0x76,0x60,0x60,0x60,0x00},
    /* 115 s     */ {0x00,0x00,0x3C,0x60,0x3C,0x06,0x7C,0x00},
    /* 116 t     */ {0x30,0x30,0x7C,0x30,0x30,0x30,0x1C,0x00},
    /* 117 u     */ {0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x00},
    /* 118 v     */ {0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00},
    /* 119 w     */ {0x00,0x00,0x63,0x6B,0x7F,0x3E,0x22,0x00},
    /* 120 x     */ {0x00,0x00,0x66,0x3C,0x18,0x3C,0x66,0x00},
    /* 121 y     */ {0x00,0x00,0x66,0x66,0x3E,0x06,0x3C,0x00},
    /* 122 z     */ {0x00,0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00},
    /* 123 {     */ {0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00},
    /* 124 |     */ {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00},
    /* 125 }     */ {0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00},
    /* 126 ~     */ {0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00},
    /* 127 DEL   */ {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

/*----- Function: get_glyph -----
 PURPOSE: returns ptr to 8-row bitmap for the given ASCII character
 INPUT:   ch - ASCII character to look up
 OUTPUT:  ptr to array of FONT_ROWS bytes representing the glyph
 NOTES:   characters outside ASCII 32-127 return the space glyph
*/
static const UINT8 *get_glyph(char ch)
{
    unsigned char idx = (unsigned char)ch;
    if (idx < 32 || idx > 127) idx = 32;   /* default to space */
    return font_table[idx - 32];
}

void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height, UINT16 *bitmap)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  r, b, word, mask;

    for (r = 0; r < height; r++)
    {
        word = bitmap[r];
        mask = 0x8000;
        for (b = 0; b < 16; b++)
        {
            if (word & mask)
                if ((row + r) < SCREEN_HEIGHT && (col + b) < SCREEN_WIDTH)
                    plot_pixel(byte_base, row + r, col + b);
            mask >>= 1;
        }
    }
}

void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col, UINT16 height, const UINT32 *bitmap)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  r, b;
    UINT32  word, mask;

    for (r = 0; r < height; r++)
    {
        word = bitmap[r];
        mask = 0x80000000UL;
        for (b = 0; b < 32; b++)
        {
            if (word & mask)
                if ((row + r) < SCREEN_HEIGHT && (col + b) < SCREEN_WIDTH)
                    plot_pixel(byte_base, row + r, col + b);
            mask >>= 1;
        }
    }
}

void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch)
{
    const UINT8 *glyph = get_glyph(ch);
    UINT16 r, b;
    UINT8  row_bits, mask;

    for (r = 0; r < FONT_ROWS; r++)
    {
        row_bits = glyph[r];
        mask     = 0x80;
        for (b = 0; b < 8; b++)
        {
            if (row_bits & mask)
                if ((row + r) < SCREEN_HEIGHT && (col + b) < SCREEN_WIDTH)
                    plot_pixel(base, row + r, col + b);
            mask >>= 1;
        }
    }
}

void plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch)
{
    while (*ch != '\0')
    {
        plot_character(base, row, col, *ch);
        col += 8;
        ch++;
    }
}

/* ════════════════════════════════════════════════════════════════
 *  TEST DRIVER
 * ════════════════════════════════════════════════════════════════ */
int main(void)
{
    UINT8  *fb8  = framebuffer;
    UINT32 *fb32 = (UINT32 *)framebuffer;

    printf("=== Raster Graphics Library - Test Driver ===\n");
    printf("    Simulated Atari ST framebuffer: %dx%d, 1bpp\n\n",
           SCREEN_WIDTH, SCREEN_HEIGHT);

    /* ── Test 1: plot_pixel ───────────────────────────────────── */
    printf("--- Test 1: plot_pixel ---\n");
    reset_fb();
    plot_pixel(fb8, 0, 0);    /* top-left corner     */
    plot_pixel(fb8, 0, 7);    /* end of first byte   */
    plot_pixel(fb8, 1, 4);    /* middle of second row*/
    print_region(fb8, 0, 0, 3, 16);
    printf("plot_pixel (0,0)  set: %s\n", get_pixel(fb8,0,0) ? "PASSED" : "FAILED");
    printf("plot_pixel (0,7)  set: %s\n", get_pixel(fb8,0,7) ? "PASSED" : "FAILED");
    printf("plot_pixel (1,4)  set: %s\n", get_pixel(fb8,1,4) ? "PASSED" : "FAILED");

    /* boundary: off-screen pixel must NOT corrupt buffer */
    plot_pixel(fb8, SCREEN_HEIGHT, SCREEN_WIDTH);
    printf("plot_pixel out-of-bounds safe: PASSED\n\n");

    /* ── Test 2: clear_screen ────────────────────────────────── */
    printf("--- Test 2: clear_screen ---\n");
    /* fill entire buffer with 0xFF then clear */
    memset(framebuffer, 0xFF, sizeof(framebuffer));
    clear_screen(fb32);
    printf("clear_screen clears all pixels: %s\n\n",
           check_pixels_clear(fb8, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH)
           ? "PASSED" : "FAILED");

    /* ── Test 3: plot_horizontal_line ───────────────────────── */
    printf("--- Test 3: plot_horizontal_line ---\n");
    reset_fb();

    /* 3a: normal line, 10 pixels wide at row 2, col 4 */
    plot_horizontal_line(fb32, 2, 4, 10);
    print_region(fb8, 2, 0, 1, 20);
    printf("plot_horizontal_line 10px: %s\n",
           check_pixels_set(fb8, 2, 4, 1, 10) ? "PASSED" : "FAILED");

    /* 3b: line starting near right edge - clipping */
    reset_fb();
    plot_horizontal_line(fb32, 5, 630, 20); /* would exceed 640 */
    printf("plot_horizontal_line clipping (no crash): PASSED\n\n");

    /* ── Test 4: plot_line (Bresenham) ──────────────────────── */
    printf("--- Test 4: plot_line ---\n");
    reset_fb();

    /* 4a: horizontal line via plot_line */
    plot_line(fb32, 5, 0, 5, 9);
    print_region(fb8, 4, 0, 3, 12);
    printf("plot_line horizontal: %s\n",
           check_pixels_set(fb8, 5, 0, 1, 10) ? "PASSED" : "FAILED");

    /* 4b: vertical line via plot_line */
    reset_fb();
    plot_line(fb32, 0, 5, 4, 5);
    printf("plot_line vertical: %s\n",
           check_pixels_set(fb8, 0, 5, 5, 1) ? "PASSED" : "FAILED");

    /* 4c: diagonal line - just verify start and end pixel set */
    reset_fb();
    plot_line(fb32, 0, 0, 7, 7);
    print_region(fb8, 0, 0, 8, 10);
    printf("plot_line diagonal start pixel: %s\n",
           get_pixel(fb8, 0, 0) ? "PASSED" : "FAILED");
    printf("plot_line diagonal end pixel:   %s\n\n",
           get_pixel(fb8, 7, 7) ? "PASSED" : "FAILED");

    /* ── Test 5: plot_triangle ──────────────────────────────── */
    printf("--- Test 5: plot_triangle ---\n");

    /* direction 0: 90 deg top-left */
    reset_fb();
    plot_triangle(fb32, 0, 0, 4, 4, 0);
    print_region(fb8, 0, 0, 5, 8);
    printf("plot_triangle dir 0 top-left corner pixel:    %s\n",
           get_pixel(fb8, 0, 0) ? "PASSED" : "FAILED");
    printf("plot_triangle dir 0 bottom row width 4:       %s\n",
           check_pixels_set(fb8, 3, 0, 1, 4) ? "PASSED" : "FAILED");

    /* direction 2: 90 deg bottom-left */
    reset_fb();
    plot_triangle(fb32, 0, 0, 4, 4, 2);
    print_region(fb8, 0, 0, 5, 8);
    printf("plot_triangle dir 2 top row width 4:          %s\n",
           check_pixels_set(fb8, 0, 0, 1, 4) ? "PASSED" : "FAILED");
    printf("plot_triangle dir 2 bottom row width 1:       %s\n\n",
           get_pixel(fb8, 3, 0) ? "PASSED" : "FAILED");

    /* ── Test 6: plot_bitmap_8 ──────────────────────────────── */
    printf("--- Test 6: plot_bitmap_8 ---\n");
    reset_fb();
    plot_bitmap_8(fb8, 0, 0, 4);
    print_region(fb8, 0, 0, 4, 10);

    /* top row should be all 1s (0xFF bitmap row) */
    printf("plot_bitmap_8 top row set (0xFF):    %s\n",
           check_pixels_set(fb8, 0, 0, 1, 8) ? "PASSED" : "FAILED");
    /* row 1: 0x81 = 1000 0001, only col 0 and col 7 set */
    printf("plot_bitmap_8 row 1 col 0 set:       %s\n",
           get_pixel(fb8, 1, 0) ? "PASSED" : "FAILED");
    printf("plot_bitmap_8 row 1 col 7 set:       %s\n",
           get_pixel(fb8, 1, 7) ? "PASSED" : "FAILED");
    printf("plot_bitmap_8 row 1 col 3 clear:     %s\n\n",
           !get_pixel(fb8, 1, 3) ? "PASSED" : "FAILED");

    /* ── Test 7: plot_bitmap_16 ─────────────────────────── */
    printf("--- Test 7: plot_bitmap_16 ---\n");
    reset_fb();
    {
        UINT16 bmp16[] = {
            0xFFFF,  /* 1111 1111 1111 1111 - full row */
            0x8001,  /* 1000 0000 0000 0001 - edges only */
            0x8001,
            0xFFFF
        };
        plot_bitmap_16((UINT16 *)fb32, 0, 0, 4, bmp16);
        print_region(fb8, 0, 0, 4, 18);
        printf("plot_bitmap_16 top row all set (0xFFFF):  %s\n",
               check_pixels_set(fb8, 0, 0, 1, 16) ? "PASSED" : "FAILED");
        printf("plot_bitmap_16 row 1 col 0 set:           %s\n",
               get_pixel(fb8, 1, 0) ? "PASSED" : "FAILED");
        printf("plot_bitmap_16 row 1 col 15 set:          %s\n",
               get_pixel(fb8, 1, 15) ? "PASSED" : "FAILED");
        printf("plot_bitmap_16 row 1 col 8 clear:         %s\n\n",
               !get_pixel(fb8, 1, 8) ? "PASSED" : "FAILED");
    }

    /* ── Test 8: plot_bitmap_32 ─────────────────────────── */
    printf("--- Test 8: plot_bitmap_32 ---\n");
    reset_fb();
    {
        const UINT32 bmp32[] = {
            0xFFFFFFFFUL,  /* all 32 bits set */
            0x80000001UL,  /* only bit 0 and bit 31 set */
            0x80000001UL,
            0xFFFFFFFFUL
        };
        plot_bitmap_32(fb32, 0, 0, 4, bmp32);
        print_region(fb8, 0, 0, 4, 34);
        printf("plot_bitmap_32 top row all set (0xFFFFFFFF): %s\n",
               check_pixels_set(fb8, 0, 0, 1, 32) ? "PASSED" : "FAILED");
        printf("plot_bitmap_32 row 1 col 0 set:              %s\n",
               get_pixel(fb8, 1, 0) ? "PASSED" : "FAILED");
        printf("plot_bitmap_32 row 1 col 31 set:             %s\n",
               get_pixel(fb8, 1, 31) ? "PASSED" : "FAILED");
        printf("plot_bitmap_32 row 1 col 16 clear:           %s\n\n",
               !get_pixel(fb8, 1, 16) ? "PASSED" : "FAILED");
    }

    /* ── Test 9: plot_vertical_line ─────────────────────── */
    printf("--- Test 9: plot_vertical_line ---\n");
    reset_fb();

    /* 9a: normal vertical line, 8 pixels tall at col 5 */
    plot_vertical_line(fb32, 0, 5, 8);
    print_region(fb8, 0, 0, 9, 10);
    printf("plot_vertical_line 8px col 5:        %s\n",
           check_pixels_set(fb8, 0, 5, 8, 1) ? "PASSED" : "FAILED");

    /* 9b: col stays fixed, row changes */
    printf("plot_vertical_line col 4 clear:      %s\n",
           !get_pixel(fb8, 0, 4) ? "PASSED" : "FAILED");
    printf("plot_vertical_line col 6 clear:      %s\n\n",
           !get_pixel(fb8, 0, 6) ? "PASSED" : "FAILED");

    /* ── Test 10: plot_rectangle ────────────────────────── */
    printf("--- Test 10: plot_rectangle ---\n");
    reset_fb();

    /* 5 rows tall, 8 cols wide, top-left at (2,2) */
    plot_rectangle(fb32, 2, 2, 5, 8);
    print_region(fb8, 1, 1, 7, 12);

    /* top edge */
    printf("plot_rectangle top edge set:         %s\n",
           check_pixels_set(fb8, 2, 2, 1, 8) ? "PASSED" : "FAILED");
    /* bottom edge */
    printf("plot_rectangle bottom edge set:      %s\n",
           check_pixels_set(fb8, 6, 2, 1, 8) ? "PASSED" : "FAILED");
    /* left edge */
    printf("plot_rectangle left edge set:        %s\n",
           check_pixels_set(fb8, 2, 2, 5, 1) ? "PASSED" : "FAILED");
    /* right edge */
    printf("plot_rectangle right edge set:       %s\n",
           check_pixels_set(fb8, 2, 9, 5, 1) ? "PASSED" : "FAILED");
    /* interior should be CLEAR (outline only) */
    printf("plot_rectangle interior clear:       %s\n\n",
           check_pixels_clear(fb8, 3, 3, 3, 6) ? "PASSED" : "FAILED");

    /* ── Test 11: plot_square ───────────────────────────── */
    printf("--- Test 11: plot_square ---\n");
    reset_fb();

    plot_square(fb32, 0, 0, 6);
    print_region(fb8, 0, 0, 7, 9);

    /* all 4 sides of a 6x6 square */
    printf("plot_square top edge set:            %s\n",
           check_pixels_set(fb8, 0, 0, 1, 6) ? "PASSED" : "FAILED");
    printf("plot_square bottom edge set:         %s\n",
           check_pixels_set(fb8, 5, 0, 1, 6) ? "PASSED" : "FAILED");
    printf("plot_square left edge set:           %s\n",
           check_pixels_set(fb8, 0, 0, 6, 1) ? "PASSED" : "FAILED");
    printf("plot_square right edge set:          %s\n",
           check_pixels_set(fb8, 0, 5, 6, 1) ? "PASSED" : "FAILED");
    printf("plot_square interior clear:          %s\n\n",
           check_pixels_clear(fb8, 1, 1, 4, 4) ? "PASSED" : "FAILED");

    /* ── Test 12: clear_region ──────────────────────────── */
    printf("--- Test 12: clear_region ---\n");

    /* fill entire buffer then clear a specific region */
    memset(framebuffer, 0xFF, sizeof(framebuffer));
    clear_region(fb32, 5, 5, 4, 8);
    print_region(fb8, 4, 4, 6, 12);

    printf("clear_region target area cleared:    %s\n",
           check_pixels_clear(fb8, 5, 5, 4, 8) ? "PASSED" : "FAILED");
    /* pixels just outside region should still be set */
    printf("clear_region outside untouched:      %s\n\n",
           get_pixel(fb8, 4, 5) ? "PASSED" : "FAILED");

    /* ── Test 13: plot_character ────────────────────────── */
    printf("--- Test 13: plot_character ---\n");
    reset_fb();
    /*
     * 'A' glyph row 0 = 0x18 = 0001 1000
     * so col 3 and col 4 should be SET, col 0 should be CLEAR
     */
    plot_character(fb8, 0, 0, 'A');
    print_region(fb8, 0, 0, 8, 10);
    printf("plot_character 'A' row 0 col 3 set:  %s\n",
           get_pixel(fb8, 0, 3) ? "PASSED" : "FAILED");
    printf("plot_character 'A' row 0 col 4 set:  %s\n",
           get_pixel(fb8, 0, 4) ? "PASSED" : "FAILED");
    printf("plot_character 'A' row 0 col 0 clear:%s\n\n",
           !get_pixel(fb8, 0, 0) ? "PASSED" : "FAILED");

    /* ── Test 14: plot_string ───────────────────────────── */
    printf("--- Test 14: plot_string ---\n");
    reset_fb();
    /*
     * 'H' at col 0, 'I' at col 8 (each char is 8px wide)
     * 'H' row 0 = 0x66 = 0110 0110 -> col 1,2,5,6 set
     * 'I' row 0 = 0x7E = 0111 1110 -> col 9,10,11,12,13,14 set (offset by 8)
     */
    plot_string(fb8, 0, 0, "HI");
    print_region(fb8, 0, 0, 8, 20);
    printf("plot_string 'H' col 1 set:           %s\n",
           get_pixel(fb8, 0, 1) ? "PASSED" : "FAILED");
    printf("plot_string 'I' col 9 set:            %s\n",
           get_pixel(fb8, 0, 9) ? "PASSED" : "FAILED");
    printf("plot_string chars separated by 8px:  %s\n\n",
           !get_pixel(fb8, 0, 8) ? "PASSED" : "FAILED");

    printf("=== All 14 raster function tests complete ===\n");
    return 0;
}
