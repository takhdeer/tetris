    xdef    _vbl_isr

_vbl_isr:
    movem.l d0-d7/a0-a6,-(sp)
    movem.l (sp)+,d0-d7/a0-a6
    rte