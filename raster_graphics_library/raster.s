; ======= RASTER C LIBRARY TRANSLATED TO ASSEMBLY ======

;CONSTANTS
BYTES_PER_ROW   EQU 80
SCREEN_HEIGHT   EQU 400
SCREEN_WIDTH    EQU 640

;=============================================================================
; plot_pixel(UINT8 *base, UINT16 row, UINT16 col)
; stack on entry: base(4 bytes), row(2 bytes), col(2 bytes)
    xdef plot_pixel             ; exporting the symbol

plot_pixel:
    movem.l d0-d2/a0, -(sp)     ;save registers

    ;Arguments
    move.l 20(sp), a0           ;a0 = base
    move.l 20(sp), a1           ;a1 = row
    move.l 20(sp), a2           ;a2 = col                          