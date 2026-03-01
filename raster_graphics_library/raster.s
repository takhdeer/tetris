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
