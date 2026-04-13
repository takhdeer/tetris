    xdef    _vbl_isr
    xref    _render_request

_vbl_isr:
    movem.l     d0-d7/a0-a6,-(sp)
    move.w      #1,_render_request
    movem.l     (sp)+,d0-d7/a0-a6
    rte