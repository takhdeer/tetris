; File: ikbd_isr.s
; Author: Henry
; Purpose: IKBD ISR - handles keyboard make/break codes and mouse packets
;          Replaces TOS IKBD vector. Must be fast - minimum work only.

; ====== HARDWARE ADDRESSES ======
IKBD_ACIA_CTRL  equ $FFFFFC00   ; IKBD 6850 control/status register
IKBD_ACIA_DATA  equ $FFFFFC02   ; IKBD 6850 data register
MFP_ISRB        equ $FFFA11     ; MFP in-service register B (must clear bit 6)

; ====== SCREEN BOUNDS ======
SCREEN_MAX_X    equ 639
SCREEN_MAX_Y    equ 399

; ====== ScanQueue struct offsets ======
QUEUE_BUF       equ 0    ; buf[8]  - 8 bytes
QUEUE_HEAD      equ 8    ; head    - 1 byte
QUEUE_TAIL      equ 9    ; tail    - 1 byte
QUEUE_COUNT     equ 10   ; count   - 1 byte

; ====== MOUSE PACKET HEADER ======
MOUSE_HDR_MIN   equ $F8         ; mouse packet headers are $F8-$FB
MOUSE_HDR_MAX   equ $FB

; ====== ISR STATES ======
STATE_IDLE      equ 0           ; waiting for first byte
STATE_MOUSE_DX  equ 1           ; got header, waiting for dx
STATE_MOUSE_DY  equ 2           ; got dx, waiting for dy

	xdef        _ikbd_isr
	xref		_mouse_buttons
	xref		_key_state
	xref		_scan_queue
	xref		_mouse_x
	xref		_mouse_y

	bss

; ISR state machine variable
ikbd_state:     ds.b    1       ; current state (0/1/2)
mouse_dx_temp:  ds.b    1       ; temporarily store dx between state 1 and 2

	text

; ====================================================
; _ikbd_isr
; The actual ISR. Called once per byte from IKBD 6850.
; ====================================================
_ikbd_isr:
	movem.l     d0-d1/a0,-(sp)         ; save only what we use - keep it fast

	; --- STEP 1: check IKBD 6850 status - spurious interrupt guard ---
	move.b      IKBD_ACIA_CTRL,d0      ; read status register
	btst        #0,d0                  ; bit 0 = RDRF (receive data register full)
	beq         done                   ; nothing to read, exit

	; --- STEP 2: read the byte (clears the interrupt) ---
	move.b      IKBD_ACIA_DATA,d0      ; d0.b = incoming byte

	; --- STEP 3: state machine ---
	move.b      ikbd_state,d1
	cmpi.b      #STATE_MOUSE_DX,d1
	beq         got_dx
	cmpi.b      #STATE_MOUSE_DY,d1
	beq         got_dy

	; ---- STATE 0: IDLE - first byte of a new packet ----

	; is it a mouse header? ($F8-$FB)
	cmpi.b      #MOUSE_HDR_MIN,d0
	blo         check_key              ; below $F8, must be a key code
	cmpi.b      #MOUSE_HDR_MAX,d0
	bhi         done                   ; above $FB, ignore (other IKBD status)

	; it's a mouse header - save buttons, advance state
	move.b      d0,_mouse_buttons       ; $F8=none, $F9=right, $FA=left, $FB=both
	move.b      #STATE_MOUSE_DX,ikbd_state
	bra         done

check_key:
	; is it a break code? (bit 7 set, $80-$F7)
	btst        #7,d0
	bne         break_code

	; ---- MAKE CODE ($00-$7F) ----
	; 1. update key_state table
	andi.w      #$7F,d0                ; zero-extend, clamp to 0-127
	lea         _key_state,a0
	move.b      #1,(a0,d0.w)

	; 2. enqueue scan code into circular queue
	lea         _scan_queue,a0         ; a0 -> scan_queue struct
	; check if full (count == QUEUE_SIZE)
	move.b      QUEUE_COUNT(a0),d1               ; count is at offset 7 (buf[8] then head, tail, count)
	cmpi.b      #8,d1
	beq         done                   ; queue full, drop it

	; buf[tail] = d0
	move.b      QUEUE_TAIL(a0),d1
	andi.w      #$FF,d1                ; zero-extend tail to word cleanly
	move.b      d0,(a0,d1.w)
	; tail = (tail + 1) % QUEUE_SIZE
	addq.b      #1,d1
	andi.b      #7,d1                  ; mod 8 via bitmask (works because QUEUE_SIZE=8)
	move.b      d1,QUEUE_TAIL(a0)              ; store new tail
	; count++
	addq.b      #1,QUEUE_COUNT(a0)
	bra         done

break_code:
	; ---- BREAK CODE ($80-$F7) ----
	; before using d0 as index into key_state
	andi.w      #$7F,d0                ; zero-extend, also masks to 0-127 safely
	lea         _key_state,a0
	move.b      #0,(a0,d0.w)
	bra         done

	; ---- STATE 1: waiting for mouse DX ----
got_dx:
	move.b      d0,mouse_dx_temp       ; save signed dx for state 2
	move.b      #STATE_MOUSE_DY,ikbd_state
	bra         done

	; ---- STATE 2: waiting for mouse DY ----
got_dy:
	; d0 = dy (current byte, arrived from STEP 2)
	; handle dx first using mouse_dx_temp
	move.b      d0,d1                  ; save dy into d1 before we clobber d0

	move.b      mouse_dx_temp,d0       ; get saved dx
	ext.w       d0
	ext.l       d0
	add.l       d0,_mouse_x

	; clamp mouse_x to [0, 639]
	tst.l       _mouse_x
	bge         clamp_x_max
	clr.l       _mouse_x
	bra         add_dy
clamp_x_max:
	cmpi.l      #SCREEN_MAX_X,_mouse_x
	ble         add_dy
	move.l      #SCREEN_MAX_X,_mouse_x

add_dy:
	move.b      d1,d0                  ; restore dy from d1
	ext.w       d0
	ext.l       d0
	add.l       d0,_mouse_y

	; clamp mouse_y to [0, 399]
	tst.l       _mouse_y
	bge         clamp_y_max
	clr.l       _mouse_y
	bra         reset_state
clamp_y_max:
	cmpi.l      #SCREEN_MAX_Y,_mouse_y
	ble         reset_state
	move.l      #SCREEN_MAX_Y,_mouse_y

reset_state:
	clr.b       ikbd_state

done:
	; --- STEP 4: clear MFP in-service bit 6 (MANDATORY or ISR fires once only) ---
	bclr        #6,MFP_ISRB

	movem.l     (sp)+,d0-d1/a0
	rte
