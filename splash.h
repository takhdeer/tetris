/**
 * file: splash.h - Splash screen (main menu)
 * Authors: Aaron, Takhdeer, Henry
 */

#ifndef SPLASH_H
#define SPLASH_H

#include "types.h"

/* -- Helper functions -- */

static void filled_rect(UINT32 *base, UINT16 top, UINT16 left, UINT16 bottom, UINT16 right);

static void draw_circle(UINT8 *base, UINT16 cy, UINT16 cx, UINT16 r);

/*
 * Draws the onion dome shape above a tower.
 * top_row/col: tip of the dome, dome_w: max width at base of dome
 */
static void draw_onion_dome(UINT32 *base, UINT16 top_row, UINT16 centre_col, UINT16 dome_h, UINT16 dome_w);

/*
 * Draws the full temple:
 *   - central tall tower with onion dome + spire
 *   - two flanking smaller towers with small domes
 *   - base connecting block
 * anchor_row: the ground line row
 * anchor_col: left edge of the whole structure
 */
static void draw_temple(UINT32 *base, UINT16 ground_row, UINT16 anchor_col);

static void draw_moon(UINT8 *base);

static void draw_stars(UINT8 *base);
/* Returns 1 if user wants to play, 0 if user wants to quit */
int show_splash_screen(UINT8 *base);

#endif