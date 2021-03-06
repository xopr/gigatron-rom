.segment "CHRGET"
RAMSTART1:
GENERIC_CHRGET:
        inc     TXTPTR
        bne     GENERIC_CHRGOT
        inc     TXTPTR+1
GENERIC_CHRGOT:
.ifndef CONFIG_CHRGET_NOT_IN_ZP
GENERIC_TXTPTR = GENERIC_CHRGOT + 1
        lda     $EA60
.else
        sty     SCRATCH
        ldy     #0
        lda     (TXTPTR),y
        ldy     SCRATCH
.endif
.ifdef KBD
        jsr     LF430
.endif
        cmp     #$3A
        bcs     L4058
GENERIC_CHRGOT2:
        cmp     #$20
        beq     GENERIC_CHRGET
        sec
        sbc     #$30
        sec
        sbc     #$D0
L4058:
        rts
