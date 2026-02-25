.org $900

SendGamepadAndMouseToGB:
;     phk
;     pea @jslrtsreturn-1
;     pea $d7f4-1 ; Known bank 1 rtl
; ; Call 3 mouse-related routines
;     jml $01d7fb ; UpdateMenuCursorPosWithMouse
; @jslrtsreturn:
;     jsr $d1b0 ; AnimateMainMenuCursor
;     jsr $cfd5 ; UpdateMenuCursorShadowOam

; For relative addresses see: https://codeberg.org/ISSOtm/sgb-bios/src/commit/dcf599c259b9875eba3d21659c76602bf9d67acb/src/wram.asm#L253
;
; Send over mouse details
    lda wHorizontalMouseMovement+1 ; Mouse X relative deltas (SNES mouse format, bits inverted) to P2
    sta ICD2P2.l
    lda wVerticalMouseMovement+1   ; Mouse Y relative deltas (SNES mouse format, bits inverted) to P3
    sta ICD2P3.l
    lda wCurrMouseRLbits+1         ; Mouse button RL bits (0x02u for Left, 0x01 for Right) to P4
    sta ICD2P4.l

; P1 controls and skipping normal input send routine is revision-specific
    ldx #$00
    lda CART_VERSION.l
    beq @ver0

    jsr $bca0 ; Send1JoypadsInputsToGB
    pla
    pla
    jmp $baaa ; Skip SendInputsToGB

@ver0:
    jsr $bca3 ; Send1JoypadsInputsToGB
    pla
    pla
    jmp $baad ; Skip SendInputsToGB
