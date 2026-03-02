; ======= RASTER C LIBRARY TRANSLATED TO ASSEMBLY ======

;CONSTANTS
BYTES_PER_ROW   EQU 80
SCREEN_HEIGHT   EQU 400
SCREEN_WIDTH    EQU 640

;=============================================================================
; plot_pixel(UINT8 *base, UINT16 row, UINT16 col)
; stack on entry: base(4 bytes), row(2 bytes), col(2 bytes)
;=============================================================================

    xdef plot_pixel             ; exporting the symbol

plot_pixel:
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
	xdef plot_square

plot_square:
	move.w	16(sp),-(sp)		;push width (side)
	move.w	18(sp),-(sp)			;push length (side)
	move.w	16(sp),-(sp)		;push col 
	move.w	16(sp),-(sp)		;push row 
	move.l	16(sp),-(sp)		;push base

	jsr		plot_rectangle
	
	lea		24(sp),sp			;clean up stack
	rts

;=============================================================================
;=============================================================================
; plot_vertical_line - draws a vertical line
; C: plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length)
;=============================================================================
;=============================================================================
	xdef	plot_vertical_line

plot_vertical_line:
	movem.l d3-d4,-(sp)
	
	move.w	20(sp),d3	; d3 = length
	beq		done		; if length == 0, exit
	
	clr.w	d4			; d4: i = 0

loop:
	cmp.w	d3,d4		; compare i with length
	bge		done		; if i >= length, exit
	
	; calling plot_pixel(base, row + i, col)
	move.w	18(sp),-(sp)	;push col
	move.w	16(sp),d0		;d0 = row
	add.w	d4,d0			;d0 = row + i
	move.w 	d0,-(sp)		;push row + i
	move.l 	16(sp),-(sp)	;push base
	
	jsr		_plot_pixel
	
	lea		8(sp),sp		
	
	addq.w 	#1,d4			;i++
	bra		loop
	
done:
	movem.l (sp)+,d3-d4		;restoring registers
	rts
	
;=============================================================================
;=============================================================================
; plot_bitmap_16 - draws a 16-pixel wide bitmap
; C: plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height, UINT16 *bitmap)
;=============================================================================
;=============================================================================

	xdef plot_bitmap_16

plot_bitmap_16:
	movem.l d3-d7/a2-a3,-(sp)	;save registers (32 bytes)
	
	move.l 	68(sp),a3			;a3 = bitmap pointer
	move.l 	52(sp),a2			;a2 = base
	move.w 	56(sp),d3			;d3 = row
	move.w 	60(sp),d4			;d4 = col
	move.w 	64(sp),d5			;d5 = height
	
	clr.w	d6					;d6: r = 0

height_loop:
	cmp.w	d5,d6				
	bge		done
	
	move.w	(a3)+,d7			;d7 = bitmap[r]
	move.w 	#$8000,d0			;mask
	clr.w	d1 					;b = 0
	
width_loop:
	cmp.w	#16,d1
	bge		next_row
	
	btst	d1,d7				;test bit
	beq		skip
	
	move.w	d4,-(sp)			;push col + bx
	move.w 	d1,(sp)				
	move.w	d3,-(sp)			;push row + r
	move.w 	d6,(sp)
	move.l 	a2,-(sp)			;push base
	
	jsr		plot_pixel
	lea		8(sp),sp
	
skip:
	addq.w	#1,d1
	bra		width_loop
	
next_row:
	addq.w	#1,d6
	bra		height_loop
	
done:
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

outer_loop:
    cmp.w   d2, d3              ; r < height?
    bge     .bitmap_done

    move.l  (a1, d3.w*4), d5   ; word = bitmap[r]  -- note: d3 is word index
    ; Actually bitmap[r] offset = r*4
    move.w  d3, d6
    lsl.w   #2, d6              ; d6 = r * 4 (byte offset)
    ext.l   d6
    move.l  (a1, d6.l), d5     ; d5 = bitmap[r]

    move.l  #$80000000, d4      ; mask = 0x80000000

    clr.w   d6                  ; b = 0

inner_loop:
    cmp.w   #32, d6             ; b < 32?
    bge     .next_row

    move.l  d5, d7
    and.l   d4, d7              ; word & mask
    beq     .skip_pixel

    ; call plot_pixel(base, row+r, col+b)
    move.w  d1, d7
    add.w   d6, d7              ; col + b
    move.w  d0, -(sp)
    add.w   d3, (sp)            ; row + r  (push row+r)
    move.w  d7, -(sp)           ; col + b
    subq.l  #2, sp              ; align / padding
    move.l  a0, -(sp)           ; base

    ; Rebuild push correctly:
    ; plot_pixel(base, row+r, col+b)
    addq.l  #8, sp              ; undo the messy above

    ; Clean push:
    move.w  d1, d7
    add.w   d6, d7              ; col + b
    move.w  d7, -(sp)           ; push col+b
    move.w  d0, d7
    add.w   d3, d7              ; row + r
    move.w  d7, -(sp)           ; push row+r
    move.l  a0, -(sp)           ; push base
    jsr     plot_pixel
    addq.l  #8, sp              ; pop args (4 + 2 + 2)

skip_pixel:
    lsr.l   #1, d4              ; mask >>= 1
    addq.w  #1, d6              ; b++
    bra     .inner_loop

next_row:
    addq.w  #1, d3              ; r++
    bra     .outer_loop

bitmap_done:
    movem.l (sp)+, d0-d7/a0-a2
    
    rts

;=============================================================================
; plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, 
; UINT16 length, UINT16 width)
; stack: base(4), row(2), col(2), length(2), width(2)
;=============================================================================
plot_rectangle:
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

top_bot_loop:
    cmp.w   d5, d4              ; c < col+width?
    bge     .top_bot_done

    ; plot_pixel(base, row, c)
    move.w  d4, -(sp)
    move.w  d0, -(sp)
    move.l  a0, -(sp)
    jsr     plot_pixel
    addq.l  #8, sp

    ; plot_pixel(base, row+length-1, c)
    move.w  d4, -(sp)
    move.w  d0, d6
    add.w   d2, d6
    subq.w  #1, d6              ; row + length - 1
    move.w  d6, -(sp)
    move.l  a0, -(sp)
    jsr     plot_pixel
    addq.l  #8, sp

    addq.w  #1, d4              ; c++
    bra     .top_bot_loop

top_bot_done:
    ; left/right edge: for r = row; r < row + length; r++
    move.w  d0, d4              ; r = row
    move.w  d0, d5
    add.w   d2, d5              ; d5 = row + length

left_right_loop:
    cmp.w   d5, d4              ; r < row+length?
    bge     .rect_done

    ; plot_pixel(base, r, col)
    move.w  d1, -(sp)
    move.w  d4, -(sp)
    move.l  a0, -(sp)
    jsr     plot_pixel
    addq.l  #8, sp

    ; plot_pixel(base, r, col+width-1)
    move.w  d1, d6
    add.w   d3, d6
    subq.w  #1, d6              ; col + width - 1
    move.w  d6, -(sp)
    move.w  d4, -(sp)
    move.l  a0, -(sp)
    jsr     plot_pixel
    addq.l  #8, sp

    addq.w  #1, d4              ; r++
    bra     .left_right_loop

rect_done:
    movem.l (sp)+, d0-d6/a0
    
    rts

;=============================================================================
; clear_region(UINT32 *base, UINT16 row, UINT16 col
; ,UINT16 length, UINT16 width)
; stack: base(4), row(2), col(2), length(2), width(2)
;=============================================================================
clear_region:
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

cr_row_loop:
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

cr_col_loop:
    cmp.w   d7, d6              ; c < col+width?
    bge     .cr_next_row
    cmp.w   #SCREEN_WIDTH, d6  ; c < SCREEN_WIDTH?
    bge     .cr_next_row

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
    bra     .cr_col_loop

cr_next_row:
    addq.w  #1, d4              ; r++
    bra     .cr_row_loop

cr_done:
    movem.l (sp)+, d0-d7/a0-a1

    rts

;=============================================================================
; plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch)
; stack: base(4), row(2), col(2), ch(4)
;=============================================================================
plot_string:
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
    jsr     plot_character
    addq.l  #8, sp              ; pop 4+2+2+2 = 10... use proper adjustment
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
	xdef	plot_character
	
plot_character:
    movem.l D3-D6/A2-A3,-(SP)   ; save registers
    
    move.l  44(sp),a2           ; a2 = base
    move.w  48(sp),d3           ; d3 = row
    move.w  52(sp),d4           ; d4 = col
    move.w  54(sp),d5           ; d5 = ch (character)
    
    ; Calculating font address: font + ch * 16
    lea     font,a3          	; a3 = font base address (relook)
    and.l   #0xFF,d5            ; ensure ch is unsigned (0-255)
    lsl.w   #4,d5               ; d5 = ch * 16
    add.w   d5,a3               ; a3 = &font[ch][0]
    
    clr.w   d6                  ; d6 = i = 0
    
loop:
    cmp.w   #16,d6
    bge     done
    
    ; Calculate screen_pos = base + (row + i) * 80 + (col >> 3)
    move.w  d3,d0               ; d0 = row
    add.w   d6,d0               ; d0 = row + i
    mulu    #80,d0              ; d0 = (row + i) * 80
    move.w  d4,d1               ; d1 = col
    lsr.w   #3,d1               ; d1 = col >> 3
    add.w   d1,d0               ; d0 = offset
    move.l  a2,a0               ; a0 = base
    add.l   d0,a0               ; a0 = screen_pos
    
    ; *screen_pos |= font[ch][i]
    move.b  (a3)+,d1            ; d1 = font[ch][i
    or.b    d1,(a0)             ; OR into screen memory
    
    addq.w  #1,d6               ; i++
    bra     loop
    
done:
    movem.l (sp)+,d3-d6/a2-a3
    rts