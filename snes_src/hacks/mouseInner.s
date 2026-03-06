
; wMenuStatusBitTemp: is at $900
.org $902

.DEFINE MOUSE_IS_CONNECTED_BIT   $01
.DEFINE MENU_IS_CLOSED_VAL       $FF

.DEFINE STAT_BYTE_LR_MASK        $03
.DEFINE STAT_BYTE_MENU_OPEN      $04
.DEFINE STAT_BYTE_CONNECTED      $A0
.DEFINE STAT_BYTE_NOT_CONNECTED  $00

.DEFINE MENU_HELD_BUTTON_TIMEOUT_MAX  $28

SendGamepadAndMouseToGB:

; Force mouse held button menu counter to max to prevent L + R mouse button combo from opening the menu.
; Menu open action still accessible using GamePad L + R
    lda  #MENU_HELD_BUTTON_TIMEOUT_MAX
    sta  wMenuHeldTimoutMouse

; For relative addresses see: https://codeberg.org/ISSOtm/sgb-bios/src/commit/dcf599c259b9875eba3d21659c76602bf9d67acb/src/wram.asm#L253
;
; Send over mouse details
    lda  wHorizontalMouseMovement+1      ; Mouse X relative deltas (SNES mouse format, bits inverted) to P2
    eor  #$ff                            ; Invert so data will be active high on the GB side
    sta  ICD2P2.l

    lda  wVerticalMouseMovement+1        ; Mouse Y relative deltas (SNES mouse format, bits inverted) to P3
    eor  #$ff                            ; Invert so data will be active high on the GB side
    sta  ICD2P3.l

    ; Prepare and save Mouse status byte (in Player 4)

    lda  wIsMouseConnected+1             ; Load mouse connected bit (located in bit .0)
    and  #MOUSE_IS_CONNECTED_BIT         ; Test if mouse connected bit is set (zero bit not set -> bne, then it's connected)
    bne  @mouse_is_connected
        ; Mouse connected bit not set
        lda  #STAT_BYTE_NOT_CONNECTED    ; All bits unset, not connected and no mouse button bits
        jmp  @save_mouse_status_and_buttons

    @mouse_is_connected:

        ; Check if menu is open, set indicator bit .3 if so
        lda  #MENU_IS_CLOSED_VAL
        sbc  wMenuActiveFeature          ; Check if menu is open. GB may choose to ignore mouse if so (0xFF = menu closed, 0x00 = menu open/visible)
        beq  @menu_check_done            ; If menu is closed (equal) then leave result as zero
            lda  #STAT_BYTE_MENU_OPEN    ; Menu is open, set indicator bit   
        @menu_check_done:
        sta  wMenuStatusBitTemp          ; Store result in output temporarily

        ; Mouse connected bit was set
        lda  wCurrMouseRLbits+1          ; Mouse button RL bits (0x02u for Left, 0x01 for Right) to P4
        and  #STAT_BYTE_LR_MASK          ; Mask to only button bits .1 and .0
        ora  #STAT_BYTE_CONNECTED        ; Set bits .7 and .5, So Mouse connected test can be ((N & 0xF0) == 0xA0)
        ora  wMenuStatusBitTemp          ; Or in the menu status bit

    @save_mouse_status_and_buttons:
    eor  #$ff                            ; Invert so data will be active high on the GB side
    sta  ICD2P4.l


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
