;   setvbase.s
;   void set_video_base(UINT16 *base)
;   (Replaces Setscreen() TOS system call)

        xdef    _set_video_base

_set_video_base:
        move.l  4(sp),d0    ; load new base address from stack
        lsr.l   #8,d0       ; shift right 8

        movea.l #$FF8201,a0
        movep.w  d0,0(a0)

        rts