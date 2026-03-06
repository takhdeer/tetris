; ======= RASTER C LIBRARY TRANSLATED TO ASSEMBLY ======

;CONSTANTS
BYTES_PER_ROW   EQU 80
SCREEN_HEIGHT   EQU 400
SCREEN_WIDTH    EQU 640

    xref fnt_tbl     ; import the font table from raster.c

;=============================================================================
; plot_pixel(UINT8 *base, UINT16 row, UINT16 col)
; stack on entry: base(4 bytes), row(2 bytes), col(2 bytes)
;=============================================================================

    xdef plt_pxl             ; exporting the symbol

plt_pxl:
    movem.l d0-d2/a0, -(sp)     ;save registers

    ;Arguments
    move.l 20(sp), a0           ;a0 = base
    move.w 20(sp), d0           ;d0 = row
    move.w 20(sp), d1           ;d1 = col

    ;byte_address = base + row * BYTES_PER_ROW + (col >> 3)
    mulu    #BYTES_PER_ROW, d0  
    move.w  d1,d2
    lsr.w   #3, d2              ;d2 = col >> 3
    ext.l   d2
    add.l   d2,d0               
    lea     (a0, d0.l),a0       ; a0 = byte_address

    ; bit_offset = col % 0
    move.w  d1,d2
    and.w   #7,d2               ; d2 = col & 7

    ; *byte_address |= (0x80 >> bit_offset)
    move.b  #80, d0
    lsr.b   d2,d0               ; d0 = 0x80 >> bit_offset
    or.b    d0,(a0)             ; set the pixel

    move.l (sp)+. d0-d2/a0
    rts

;=============================================================================
;=============================================================================
;  plot_square - draws a square
; C: void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side)
;=============================================================================
;=============================================================================
	xdef plt_sqr

plt_sqr:
	move.w	16(sp),-(sp)		;push width (side)
	move.w	18(sp),-(sp)			;push length (side)
	move.w	16(sp),-(sp)		;push col 
	move.w	16(sp),-(sp)		;push row 
	move.l	16(sp),-(sp)		;push base

	jsr		plt_rec
	
	lea		24(sp),sp			;clean up stack
	rts

;=============================================================================
;=============================================================================
;  plot_vertical_line - Plots a vertical line on the screen.
; C: void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
;=============================================================================
;=============================================================================
    xdef    plt_vl


plt_vl:
	movem.l d3-d4,-(sp)
	
	move.w	20(sp),d3	; d3 = length
	beq		vl_done		; if length == 0, exit
	
	clr.w	d4			; d4: i = 0

vl_loop:
	cmp.w	d3,d4		; compare i with length
	bge		vl_done		; if i >= length, exit
	
	; calling plt_pxl(base, row + i, col)
	move.w	18(sp),-(sp)	;push col
	move.w	16(sp),d0		;d0 = row
	add.w	d4,d0			;d0 = row + i
	move.w 	d0,-(sp)		;push row + i
	move.l 	16(sp),-(sp)	;push base
	
	jsr		plt_pxl
	
	lea		8(sp),sp		
	
	addq.w 	#1,d4			;i++
	bra		vl_loop
	
vl_done:
	movem.l (sp)+,d3-d4		;restoring registers
	rts
	
;=============================================================================
;=============================================================================
; plot_bitmap_16 - draws a 16-pixel wide bitmap
; C: plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height, UINT16 *bitmap)
;=============================================================================
;=============================================================================

	xdef bitm_16

bitm_16:
	movem.l d3-d7/a2-a3,-(sp)	;save registers (32 bytes)
	
	move.l 	68(sp),a3			;a3 = bitmap pointer
	move.l 	52(sp),a2			;a2 = base
	move.w 	56(sp),d3			;d3 = row
	move.w 	60(sp),d4			;d4 = col
	move.w 	64(sp),d5			;d5 = height
	
	clr.w	d6					;d6: r = 0

ht_loop:
	cmp.w	d5,d6				
	bge		b16_dne
	
	move.w	(a3)+,d7			;d7 = bitmap[r]
	move.w 	#$8000,d0			;mask
	clr.w	d1 					;b = 0
	
wd_loop:
	cmp.w	#16,d1
	bge		b16_nxt
	
	btst	d1,d7				;test bit
	beq		b16_skp
	
	move.w	d4,-(sp)			;push col + bx
	move.w 	d1,(sp)				
	move.w	d3,-(sp)			;push row + r
	move.w 	d6,(sp)
	move.l 	a2,-(sp)			;push base
	
	jsr		plt_pxl
	lea		8(sp),sp
	
b16_skp:
	addq.w	#1,d1
	bra		wd_loop
	
b16_nxt:
	addq.w	#1,d6
	bra		ht_loop
	
b16_dne:
	movem.l	(sp)+,d3-d7/a2-a3	;restoring registers
	rts

;=============================================================================
; plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col, UINT16 height, const UINT32 *bitmap)
; stack: base(4), row(2), col(2), height(2), pad(2), bitmap(4)
;=============================================================================
bitm_32:
    movem.l d0-d7/a0-a2, -(sp)  ; save registers (11 regs = 44 bytes)

    ; Arguments offset from sp == 44 + 4 = 48
    move.l  48(sp), a0          ;a0 = base 
    move.w  52(sp), d0          ; d0 = row
    move.w  54(sp), d1          ; d1 = col
    move.w  56(sp), d2          ; d2 = height
    move.l  60(sp), a1          ; a1 = bitmap

    clr.w   d3                  ; r = 0

out_lp:
    cmp.w   d2, d3              ; r < height?
    bge     .b32_dne

    move.l  (a1, d3.w*4), d5   ; word = bitmap[r]  -- note: d3 is word index
    ; Actually bitmap[r] offset = r*4
    move.w  d3, d6
    lsl.w   #2, d6              ; d6 = r * 4 (byte offset)
    ext.l   d6
    move.l  (a1, d6.l), d5     ; d5 = bitmap[r]

    move.l  #$80000000, d4      ; mask = 0x80000000

    clr.w   d6                  ; b = 0

in_lp:
    cmp.w   #32, d6             ; b < 32?
    bge     .nxt_row

    move.l  d5, d7
    and.l   d4, d7              ; word & mask
    beq     .skp_pxl

    ; call plt_pxl(base, row+r, col+b)
    move.w  d1, d7
    add.w   d6, d7              ; col + b
    move.w  d0, -(sp)
    add.w   d3, (sp)            ; row + r  (push row+r)
    move.w  d7, -(sp)           ; col + b
    subq.l  #2, sp              ; align / padding
    move.l  a0, -(sp)           ; base

    ; Rebuild push correctly:
    ; plt_pxl(base, row+r, col+b)
    addq.l  #8, sp              ; undo the messy above

    ; Clean push:
    move.w  d1, d7
    add.w   d6, d7              ; col + b
    move.w  d7, -(sp)           ; push col+b
    move.w  d0, d7
    add.w   d3, d7              ; row + r
    move.w  d7, -(sp)           ; push row+r
    move.l  a0, -(sp)           ; push base
    jsr     plt_pxl
    addq.l  #8, sp              ; pop args (4 + 2 + 2)

skp_pxl:
    lsr.l   #1, d4              ; mask >>= 1
    addq.w  #1, d6              ; b++
    bra     .in_lp

nxt_row:
    addq.w  #1, d3              ; r++
    bra     .out_lp

b32_dne:
    movem.l (sp)+, d0-d7/a0-a2
    
    rts

;=============================================================================
; plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, 
; UINT16 length, UINT16 width)
; stack: base(4), row(2), col(2), length(2), width(2)
;=============================================================================
plt_rec:
    movem.l d0-d6/a0, -(sp)    ; 8 regs = 32 bytes

    ; Args at offset 32 + 4 = 36
    move.l  36(sp), a0          ; a0 = base (byte_base)
    move.w  40(sp), d0          ; d0 = row
    move.w  42(sp), d1          ; d1 = col
    move.w  44(sp), d2          ; d2 = length
    move.w  46(sp), d3          ; d3 = width

    ; top/bottom edge: for c = col; c < col + width; c++
    move.w  d1, d4              ; c = col
    move.w  d1, d5
    add.w   d3, d5              ; d5 = col + width

tb_loop:
    cmp.w   d5, d4              ; c < col+width?
    bge     .tb_done

    ; plt_pxl(base, row, c)
    move.w  d4, -(sp)
    move.w  d0, -(sp)
    move.l  a0, -(sp)
    jsr     plt_pxl
    addq.l  #8, sp

    ; plt_pxl(base, row+length-1, c)
    move.w  d4, -(sp)
    move.w  d0, d6
    add.w   d2, d6
    subq.w  #1, d6              ; row + length - 1
    move.w  d6, -(sp)
    move.l  a0, -(sp)
    jsr     plt_pxl
    addq.l  #8, sp

    addq.w  #1, d4              ; c++
    bra     .tb_loop

tb_done:
    ; left/right edge: for r = row; r < row + length; r++
    move.w  d0, d4              ; r = row
    move.w  d0, d5
    add.w   d2, d5              ; d5 = row + length

lr_loop:
    cmp.w   d5, d4              ; r < row+length?
    bge     .rec_dne

    ; plot_pixel(base, r, col)
    move.w  d1, -(sp)
    move.w  d4, -(sp)
    move.l  a0, -(sp)
    jsr     plt_pxl
    addq.l  #8, sp

    ; plot_pixel(base, r, col+width-1)
    move.w  d1, d6
    add.w   d3, d6
    subq.w  #1, d6              ; col + width - 1
    move.w  d6, -(sp)
    move.w  d4, -(sp)
    move.l  a0, -(sp)
    jsr     plt_pxl
    addq.l  #8, sp

    addq.w  #1, d4              ; r++
    bra     .lr_loop

rec_dne:
    movem.l (sp)+, d0-d6/a0
    
    rts

;=============================================================================
; clear_region(UINT32 *base, UINT16 row, UINT16 col
; ,UINT16 length, UINT16 width)
; stack: base(4), row(2), col(2), length(2), width(2)
;=============================================================================
    xdef clr_rgn

clr_rgn:
    movem.l d0-d7/a0-a1, -(sp) ; 10 regs = 40 bytes

    ; Args at offset 40 + 4 = 44
    move.l  44(sp), a0          ; a0 = byte_base
    move.w  48(sp), d0          ; d0 = row
    move.w  50(sp), d1          ; d1 = col
    move.w  52(sp), d2          ; d2 = length
    move.w  54(sp), d3          ; d3 = width

    ; r loop: r = row; r < row+length && r < SCREEN_HEIGHT
    move.w  d0, d4              ; r = row
    move.w  d0, d5
    add.w   d2, d5              ; d5 = row + length

cr_rlp:
    cmp.w   d5, d4              ; r < row+length?
    bge     .cr_done
    cmp.w   #SCREEN_HEIGHT, d4 ; r < SCREEN_HEIGHT?
    bge     .cr_done

    ; row_ptr = byte_base + r * BYTES_PER_ROW
    move.w  d4, d6
    mulu    #BYTES_PER_ROW, d6  ; d6 = r * 80
    lea     (a0, d6.l), a1      ; a1 = row_ptr

    ; c loop: c = col; c < col+width && c < SCREEN_WIDTH
    move.w  d1, d6              ; c = col
    move.w  d1, d7
    add.w   d3, d7              ; d7 = col + width

cr_clp:
    cmp.w   d7, d6              ; c < col+width?
    bge     .cr_nxt
    cmp.w   #SCREEN_WIDTH, d6  ; c < SCREEN_WIDTH?
    bge     .cr_nxt

    ; byte_addr = row_ptr + (c >> 3)
    move.w  d6, d5
    lsr.w   #3, d5              ; d5 = c >> 3
    ext.l   d5
    ; *byte_addr &= ~(0x80 >> (c & 7))
    move.w  d6, d0
    and.w   #7, d0              ; d0 = c & 7
    move.b  #$80, d1
    lsr.b   d0, d1              ; d1 = 0x80 >> (c&7)
    not.b   d1                  ; d1 = ~(0x80 >> (c&7))
    and.b   d1, (a1, d5.l)     ; clear the bit

    addq.w  #1, d6              ; c++
    bra     .cr_clp

cr_nxt:
    addq.w  #1, d4              ; r++
    bra     .cr_rlp

cr_done:
    movem.l (sp)+, d0-d7/a0-a1

    rts

;=============================================================================
; plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch)
; stack: base(4), row(2), col(2), ch(4)
;=============================================================================
plt_str:
    movem.l d0-d2/a0-a1, -(sp) ; 5 regs = 20 bytes

    ; Args at offset 20 + 4 = 24
    move.l  24(sp), a0          ; a0 = base
    move.w  28(sp), d0          ; d0 = row
    move.w  30(sp), d1          ; d1 = col
    move.l  32(sp), a1          ; a1 = ch (char pointer)

ps_loop:
    move.b  (a1), d2            ; d2 = *ch
    beq     .ps_done            ; if '\0', stop

    ; plot_character(base, row, col, *ch)
    clr.l   d2
    move.b  (a1), d2
    move.w  d2, -(sp)           ; push char
    move.w  d1, -(sp)           ; push col
    move.w  d0, -(sp)           ; push row
    move.l  a0, -(sp)           ; push base
    jsr     plt_chr
    ; addq.l  #8, sp              ; pop 4+2+2+2 = 10... use proper adjustment
    ; Note: args = 4+2+2+2 = 10 bytes, but word-aligned so addq.l is fine
    ; Actually adjust by 10:
    add.l   #10, sp

    addq.w  #8, d1              ; col += 8
    addq.l  #1, a1              ; ch++
    bra     .ps_loop

ps_done:
    movem.l (sp)+, d0-d2/a0-a1

    rts
    
;=============================================================================
; plot_character - draws an 8x16 character from font table
; C : void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch)
;=============================================================================
	xdef	plt_chr
    xdef    fnt_tbl          ; imported from raster.c
	
plt_chr:
    movem.l D3-D6/A2-A3,-(SP)   ; save registers
    
    move.l  28(sp),a2           ; a2 = base
    move.w  32(sp),d3           ; d3 = row
    move.w  34(sp),d4           ; d4 = col
    move.w  36(sp),d5           ; d5 = ch (character)
    
    ;condition check 
    cmp.w   #32, d5
    blt     pc_ussp
    cmp.w   #127, d5
    bgt     pc_ussp
    bra     pc_calc

pc_ussp:
    move.w  #32,d5

pc_calc:
    ; index = ch - 32
    ; offset into fnt_tbl = index * 8  (8 bytes per glyph)

    sub.w   #32, d5             ; d5 = ch - 32
    lsl.w   #3, d5              ; d5 = index * 8 (shift left 3 = multiply by 8)
    ext.l   d5

    lea     fnt_tbl, a3      ; a3 = base of fnt_tbl
    add.l   d5, a3              ; a3 = &fnt_tbl[index][0]

    clr.w   d6                  ; d6 = i = 0

    
ch_loop:
    cmp.w   #16,d6
    bge     ch_done
    
    ; Calculate screen_pos = base + (row + i) * 80 + (col >> 3)
    move.w  d3,d0               ; d0 = row
    add.w   d6,d0               ; d0 = row + i
    mulu    #BYTES_PER_ROW,d0              ; d0 = (row + i) * 80
    move.w  d4,d1               ; d1 = col
    lsr.w   #3,d1               ; d1 = col >> 3
    add.w   d1,d0               ; d0 = offset
    move.l  a2,a0               ; a0 = base
    add.l   d0,a0               ; a0 = screen_pos
    
    ; *screen_pos |= font[ch][i]
    move.b  (a3)+,d1            ; d1 = font[ch][i
    or.b    d1,(a0)             ; OR into screen memory
    
    addq.w  #1,d6               ; i++
    bra     ch_loop
    
ch_done:
    movem.l (sp)+,d3-d6/a2-a3
    
    rts

;=============================================================================
; clear_screen(UINT32 *base)
;
; Stack layout after movem.l d0/a0 -(sp)  [2 regs = 8 bytes saved]:
;   0(sp)  = saved d0
;   4(sp)  = saved a0
;   8(sp)  = return address
;  12(sp)  = base (4 bytes)
;
; Calls clr_rgn(base, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH)
;=============================================================================
    xdef    clr_rgn

clear_screen:
    movem.l d0/a0, -(sp)        ; save registers (8 bytes)

    move.l  12(sp), a0          ; a0 = base

    ; push args for clr_rgn    (base, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH)
    move.w  #SCREEN_WIDTH,  -(sp)   ; width  = 640
    move.w  #SCREEN_HEIGHT, -(sp)   ; length = 400
    move.w  #0, -(sp)               ; col    = 0
    move.w  #0, -(sp)               ; row    = 0
    move.l  a0, -(sp)               ; base

    jsr     clr_rgn
    add.l   #12, sp             ; pop args (4+2+2+2+2 = 12 bytes)

    movem.l (sp)+, d0/a0        ; restore registers
    rts

;=============================================================================
; plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length)
;
; Stack layout after movem.l d3-d6/a2 -(sp)  [5 regs = 20 bytes saved]:
;   0(sp)  = saved d3
;   4(sp)  = saved d4
;   8(sp)  = saved d5
;  12(sp)  = saved d6
;  16(sp)  = saved a2
;  20(sp)  = return address
;  24(sp)  = base   (4 bytes)
;  28(sp)  = row    (2 bytes)
;  30(sp)  = col    (2 bytes)
;  32(sp)  = length (2 bytes)
;=============================================================================
    xdef    plot_horizontal_line

plot_horizontal_line:
    movem.l d3-d6/a2, -(sp)     ; save registers (20 bytes)

    move.l  24(sp), a2          ; a2 = base
    move.w  28(sp), d3          ; d3 = row
    move.w  30(sp), d4          ; d4 = col
    move.w  32(sp), d5          ; d5 = length

    clr.w   d6                  ; d6: i = 0

henry_hline_loop:
    cmp.w   d5, d6              ; i < length?
    bge     henry_hline_done

    ; boundary check: row < SCREEN_HEIGHT
    cmp.w   #SCREEN_HEIGHT, d3
    bge     henry_hline_done

    ; boundary check: (col + i) < SCREEN_WIDTH
    move.w  d4, d0
    add.w   d6, d0              ; d0 = col + i
    cmp.w   #SCREEN_WIDTH, d0
    bge     henry_hline_done

    ; plt_pxl(base, row, col + i)
    move.w  d0, -(sp)           ; push col + i
    move.w  d3, -(sp)           ; push row
    move.l  a2, -(sp)           ; push base
    jsr     plt_pxl
    add.l   #8, sp              ; pop args (4+2+2 = 8 bytes)

    addq.w  #1, d6              ; i++
    bra     henry_hline_loop

henry_hline_done:
    movem.l (sp)+, d3-d6/a2     ; restore registers
    rts

;=============================================================================
; plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height)
;
; Stack layout after movem.l d3-d7/a2-a3 -(sp)  [7 regs = 28 bytes saved]:
;   0(sp)  = saved d3
;   ...
;  24(sp)  = saved a2
;  28(sp)  = saved a3 (wait: 7 regs = d3,d4,d5,d6,d7,a2,a3)
;  32(sp)  = return address
;  36(sp)  = base   (4 bytes)
;  40(sp)  = row    (2 bytes)
;  42(sp)  = col    (2 bytes)
;  44(sp)  = height (2 bytes)
;
; Hardcoded bitmap: {0xFF, 0x81, 0x81, 0xFF}
;=============================================================================
    xdef    bitm_8

bitm_8:
    movem.l d3-d7/a2-a3, -(sp)  ; save registers (28 bytes)

    move.l  32(sp), a2          ; a2 = base
    move.w  36(sp), d3          ; d3 = row
    move.w  38(sp), d4          ; d4 = col
    move.w  40(sp), d5          ; d5 = height

    lea     henry_bm8_data, a3  ; a3 = &bitmap[0]

    clr.w   d6                  ; d6: r = 0

henry_bm8_row_loop:
    cmp.w   d5, d6              ; r < height?
    bge     henry_bm8_done

    move.b  (a3, d6.w), d7      ; d7 = bitmap[r]
    move.b  #$80, d0            ; d0: mask = 0x80
    clr.w   d1                  ; d1: b = 0

henry_bm8_bit_loop:
    cmp.w   #8, d1              ; b < 8?
    bge     henry_bm8_next_row

    ; if (byte & mask) == 0, skip
    move.b  d7, d2
    and.b   d0, d2
    beq     henry_bm8_skip

    ; boundary check: (row + r) < SCREEN_HEIGHT
    move.w  d3, d2
    add.w   d6, d2              ; d2 = row + r
    cmp.w   #SCREEN_HEIGHT, d2
    bge     henry_bm8_skip

    ; boundary check: (col + b) < SCREEN_WIDTH
    move.w  d4, d2
    add.w   d1, d2              ; d2 = col + b
    cmp.w   #SCREEN_WIDTH, d2
    bge     henry_bm8_skip

    ; plt_pxl(base, row + r, col + b)
    move.w  d4, d2
    add.w   d1, d2              ; col + b
    move.w  d2, -(sp)           ; push col + b
    move.w  d3, d2
    add.w   d6, d2              ; row + r
    move.w  d2, -(sp)           ; push row + r
    move.l  a2, -(sp)           ; push base
    jsr     plt_pxl
    add.l   #8, sp              ; pop args (4+2+2 = 8 bytes)

henry_bm8_skip:
    lsr.b   #1, d0              ; mask >>= 1
    addq.w  #1, d1              ; b++
    bra     henry_bm8_bit_loop

henry_bm8_next_row:
    addq.w  #1, d6              ; r++
    bra     henry_bm8_row_loop

henry_bm8_done:
    movem.l (sp)+, d3-d7/a2-a3  ; restore registers
    rts

henry_bm8_data:
    dc.b    $FF, $81, $81, $FF  ; hardcoded bitmap rows
    ds.b    4                   ; pad to 8 bytes total

;=============================================================================
; plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col,
;           UINT16 end_row, UINT16 end_col)
; Bresenham's line algorithm
;
; Register usage inside function:
;   a2 = base
;   d3 = start_row (updated each iteration)
;   d4 = start_col (updated each iteration)
;   d5 = end_row
;   d6 = end_col
;   d7 = err
;   Local vars sx, sy, dx, dy pushed to stack
;
; Stack layout after movem.l d3-d7/a2 -(sp)  [6 regs = 24 bytes saved]:
;  24(sp)  = return address
;  28(sp)  = base       (4 bytes)
;  32(sp)  = start_row  (2 bytes)
;  34(sp)  = start_col  (2 bytes)
;  36(sp)  = end_row    (2 bytes)
;  38(sp)  = end_col    (2 bytes)
;=============================================================================
    xdef    plot_line

plot_line:
    movem.l d3-d7/a2, -(sp)     ; save registers (24 bytes)

    move.l  28(sp), a2          ; a2 = base
    move.w  32(sp), d3          ; d3 = start_row (mutable)
    move.w  34(sp), d4          ; d4 = start_col (mutable)
    move.w  36(sp), d5          ; d5 = end_row
    move.w  38(sp), d6          ; d6 = end_col

    ; dx = end_col - start_col
    move.w  d6, d0
    sub.w   d4, d0              ; d0 = dx (signed)

    ; dy = end_row - start_row
    move.w  d5, d1
    sub.w   d3, d1              ; d1 = dy (signed)

    ; sx = (dx > 0) ? 1 : -1
    tst.w   d0
    bgt     henry_line_sx_pos
    move.w  #-1, d2             ; sx = -1
    bra     henry_line_sx_done

henry_line_sx_pos:
    move.w  #1, d2              ; sx = 1

henry_line_sx_done:

    ; sy = (dy > 0) ? 1 : -1
    tst.w   d1
    bgt     henry_line_sy_pos
    move.w  #-1, d0             ; reuse d0 temporarily for sy
    bra     henry_line_sy_done

henry_line_sy_pos:
    move.w  #1, d0

henry_line_sy_done:
    ; push sx and sy onto stack for safe keeping
    ; d2 = sx, d0 = sy
    move.w  d0, -(sp)           ; push sy
    move.w  d2, -(sp)           ; push sx
    ; sp+0 = sx, sp+2 = sy

    ; reload dx and dy, take abs
    move.w  38(sp), d0          ; reload end_col (offset shifted +4)
    sub.w   36(sp), d0          ; dx = end_col - start_col
    ; wait -- offsets shifted by 4 due to sx/sy push
    ; recalculate cleanly:
    move.w  d6, d0              ; end_col (still in d6)
    sub.w   d4, d0              ; dx
    tst.w   d0
    bge     henry_line_dx_abs
    neg.w   d0                  ; dx = abs(dx)

henry_line_dx_abs:

    move.w  d5, d1              ; end_row (still in d5)
    sub.w   d3, d1              ; dy
    tst.w   d1
    bge     henry_line_dy_abs
    neg.w   d1                  ; dy = abs(dy)

henry_line_dy_abs:

    ; push dx and dy
    move.w  d1, -(sp)           ; push dy
    move.w  d0, -(sp)           ; push dx

    ; stack: sp+0=dx, sp+2=dy, sp+4=sx, sp+6=sy

    ; err = dx - dy
    sub.w   d1, d0              ; d0 = dx - dy
    move.w  d0, d7              ; d7 = err

henry_line_loop:
    ; boundary check: start_col < SCREEN_WIDTH && start_row < SCREEN_HEIGHT
    cmp.w   #SCREEN_WIDTH, d4
    bge     henry_line_no_plot
    cmp.w   #SCREEN_HEIGHT, d3
    bge     henry_line_no_plot

    ; plt_pxl(base, start_row, start_col)
    move.w  d4, -(sp)           ; push start_col
    move.w  d3, -(sp)           ; push start_row
    move.l  a2, -(sp)           ; push base
    jsr     plt_pxl
    add.l   #8, sp              ; pop args

henry_line_no_plot:
    ; if start_col == end_col && start_row == end_row: done
    cmp.w   d6, d4
    bne     henry_line_step
    cmp.w   d5, d3
    beq     henry_line_done

henry_line_step:
    ; e2 = 2 * err
    move.w  d7, d0
    add.w   d0, d0              ; d0 = e2 = 2 * err

    ; if e2 > -dy: err -= dy; start_col += sx
    ; sp+0=dx, sp+2=dy, sp+4=sx, sp+6=sy
    move.w  2(sp), d1           ; d1 = dy
    neg.w   d1                  ; d1 = -dy
    cmp.w   d1, d0              ; e2 > -dy?
    ble     henry_line_skip_x

    move.w  2(sp), d1           ; dy
    sub.w   d1, d7              ; err -= dy
    move.w  4(sp), d1           ; sx
    add.w   d1, d4              ; start_col += sx

henry_line_skip_x:
    ; if e2 < dx: err += dx; start_row += sy
    move.w  0(sp), d1           ; dx
    cmp.w   d1, d0              ; e2 < dx?
    bge     henry_line_skip_y

    move.w  0(sp), d1           ; dx
    add.w   d1, d7              ; err += dx
    move.w  6(sp), d1           ; sy
    add.w   d1, d3              ; start_row += sy

henry_line_skip_y:
    bra     henry_line_loop

henry_line_done:
    add.l   #8, sp              ; pop dx/dy/sx/sy (4 words = 8 bytes)
    movem.l (sp)+, d3-d7/a2     ; restore registers
    rts

;=============================================================================
; plot_triangle(UINT32 *base, UINT16 row, UINT16 col,
;               UINT16 tri_base, UINT16 height, UINT8 direction)
;
; Stack layout after movem.l d3-d7/a2 -(sp)  [6 regs = 24 bytes saved]:
;  24(sp)  = return address
;  28(sp)  = base      (4 bytes)
;  32(sp)  = row       (2 bytes)
;  34(sp)  = col       (2 bytes)
;  36(sp)  = tri_base  (2 bytes) -- not used in loop logic
;  38(sp)  = height    (2 bytes)
;  40(sp)  = direction (2 bytes, UINT8 promoted)
;=============================================================================
    xdef    plot_triangle

plot_triangle:
    movem.l d3-d7/a2, -(sp)     ; save registers (24 bytes)

    move.l  28(sp), a2          ; a2 = base
    move.w  32(sp), d3          ; d3 = row
    move.w  34(sp), d4          ; d4 = col
    move.w  38(sp), d5          ; d5 = height
    move.w  40(sp), d6          ; d6 = direction
    and.w   #$FF, d6            ; mask to UINT8

    clr.w   d7                  ; d7: i = 0

    cmp.w   #0, d6
    beq     henry_tri_dir0
    cmp.w   #1, d6
    beq     henry_tri_dir1
    cmp.w   #2, d6
    beq     henry_tri_dir2
    cmp.w   #3, d6
    beq     henry_tri_dir3
    bra     henry_tri_done

; --- Direction 0: top-left, length = i+1, col fixed ---
henry_tri_dir0:
    cmp.w   d5, d7
    bge     henry_tri_done

    move.w  d7, d0
    addq.w  #1, d0              ; length = i + 1
    move.w  d0, -(sp)           ; push length
    move.w  d4, -(sp)           ; push col
    move.w  d3, d0
    add.w   d7, d0
    move.w  d0, -(sp)           ; push row + i
    move.l  a2, -(sp)           ; push base
    jsr     plot_horizontal_line
    add.l   #10, sp             ; pop 4+2+2+2 = 10 bytes

    addq.w  #1, d7
    bra     henry_tri_dir0

; --- Direction 1: top-right, length = i+1, col = col - i ---
henry_tri_dir1:
    cmp.w   d5, d7
    bge     henry_tri_done

    move.w  d7, d0
    addq.w  #1, d0              ; length = i + 1
    move.w  d0, -(sp)           ; push length
    move.w  d4, d0
    sub.w   d7, d0              ; col - i
    move.w  d0, -(sp)           ; push col - i
    move.w  d3, d0
    add.w   d7, d0
    move.w  d0, -(sp)           ; push row + i
    move.l  a2, -(sp)           ; push base
    jsr     plot_horizontal_line
    add.l   #10, sp

    addq.w  #1, d7
    bra     henry_tri_dir1

; --- Direction 2: bottom-left, length = height-i, col fixed ---
henry_tri_dir2:
    cmp.w   d5, d7
    bge     henry_tri_done

    move.w  d5, d0
    sub.w   d7, d0              ; length = height - i
    move.w  d0, -(sp)           ; push length
    move.w  d4, -(sp)           ; push col
    move.w  d3, d0
    add.w   d7, d0
    move.w  d0, -(sp)           ; push row + i
    move.l  a2, -(sp)           ; push base
    jsr     plot_horizontal_line
    add.l   #10, sp

    addq.w  #1, d7
    bra     henry_tri_dir2

; --- Direction 3: bottom-right, length = height-i, col = col + i ---
henry_tri_dir3:
    cmp.w   d5, d7
    bge     henry_tri_done

    move.w  d5, d0
    sub.w   d7, d0              ; length = height - i
    move.w  d0, -(sp)           ; push length
    move.w  d4, d0
    add.w   d7, d0              ; col + i
    move.w  d0, -(sp)           ; push col + i
    move.w  d3, d0
    add.w   d7, d0
    move.w  d0, -(sp)           ; push row + i
    move.l  a2, -(sp)           ; push base
    jsr     plot_horizontal_line
    add.l   #10, sp

    addq.w  #1, d7
    bra     henry_tri_dir3

henry_tri_done:
    movem.l (sp)+, d3-d7/a2     ; restore registers
    rts
