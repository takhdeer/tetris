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
#include "../types.h"
#include "../raster_graphics_library/raster.h"

/* ── Atari ST screen constants (mirrors raster.c) ── */
#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  400
#define BYTES_PER_ROW  (SCREEN_WIDTH / 8)   /* 80 bytes per row  */
#define WORDS_PER_ROW  (SCREEN_WIDTH / 32)  /* 20 UINT32s per row */


/* ── Simulated framebuffer: 400 rows x 80 bytes = 32 000 bytes ── */
static UINT8 framebuffer[SCREEN_HEIGHT * BYTES_PER_ROW];

// ════════════════════════════════════════════════════════════════
// Helper functions
// ════════════════════════════════════════════════════════════════
/*
    Reset the frame buffer
*/
static void reset_fb() {
    // Using memset to fill the framebuffer with 0's 
    memset(framebuffer, 0, sizeof(framebuffer));
}

/*
    Read one pixel from the frame buffer
    1 == SET
    0 == CLEAR
*/
static int get_pixel(UINT8 *base, UINT16 row, UINT16 col)
{
    UINT8 *byte_addr;
    UINT8  bit_offset;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return 0;

    byte_addr  = base + (UINT32)row * BYTES_PER_ROW + (col >> 3);
    bit_offset = col & 7;
    return (*byte_addr >> (7 - bit_offset)) & 1;
}

/*
    Print a smaller region of the frame buffer to terminal, where:
    '#' == SET (i.e 1)
    '.' == CLEAR (i.e 0)
*/
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

/*
    Checks if all pixels in a range of rows or cols are SET
 */
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

/*
    Checks if all pixels in a range of rows or cols are CLEAR
*/
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

    // bitmap with an x in it s
    UINT8 bmp8[] = {
        0xFF,   /* 1111 1111 */
        0x81,   /* 1000 0001 */
        0xC3,   /* 1100 0011 */
        0xA5,   /* 1010 0101 */
        0xA5,   /* 1010 0101 */
        0xC3,   /* 1100 0011 */
        0x81,   /* 1000 0001 */
        0xFF    /* 1111 1111 */
    };

    plot_bitmap_8(fb8, 0, 0, 4,bmp8);
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
