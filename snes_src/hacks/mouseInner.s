
; wMenuStatusBitTemp: is at $900
.org $902

; https://www.repairfaq.org/REPAIR/F_SNES.html
; Clock Cycle     Button Reported
; ===========     ===============
; 1               B
; 2               Y
; 3               Select
; 4               Start
; 5               Up on joypad
; 6               Down on joypad
; 7               Left on joypad
; 8               Right on joypad
;
; 9               A
; 10              X
; 11              L
; 12              R
; 13              none (always high)
; 14              none (always high)
; 15              none (always high)
; 16              none (always high)

RemapExtraButtonsToPlayers3and4:
    .DEFINE MENU_HELD_BUTTON_TIMEOUT_MAX  $28
    ; Force the L/R held button menu counter for both gamepads to max to prevent
    ; L + R button combo from opening the menu.
    lda  #MENU_HELD_BUTTON_TIMEOUT_MAX
    sta  wMenuHeldTimoutGamepad      // Gamepad 1
    sta  wMenuHeldTimoutGamepad + 1  // Gamepad 2

    ; Note, the controller order here is going to match the SNES format instead
    ; of the usual SGB format, so on the GB side it will be parsed a little different.
    ; It could be remapped here with more work to what is normal for the GB.

    ; Map the extra SNES controller/gamepad buttons from Players 1 and 2 into button data for SGB Players 3 and 4 
    ; Controller 1
    lda  wJoyPad1Hi
    sta  ICD2P1.l
    lda  wJoyPad1Lo
    sta  ICD2P2.l

    ; Controller 2
    lda  wJoyPad2Hi
    sta  ICD2P3.l
    lda  wJoyPad2Lo
    sta  ICD2P4.l


; P1 controls and skipping normal input send routine is revision-specific
    ldx #$00  ; Select Joypad 0 for jsr subroutine call below to Send1JoypadsInputsToGB
    lda CART_VERSION.l
    beq @ver0

    ; jsr $bca0 ; Send1JoypadsInputsToGB  ; Don't call the update since we're overwriting controller 1 as well
    pla
    pla
    jmp $baaa ; Skip SendInputsToGB

@ver0:
    ; jsr $bca3 ; Send1JoypadsInputsToGB  ; Don't call the update since we're overwriting controller 1 as well
    pla
    pla
    jmp $baad ; Skip SendInputsToGB


