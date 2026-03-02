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
;=============================================================================
; plot_character - draws an 8x16 character from font table
; C : void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch)
;=============================================================================
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