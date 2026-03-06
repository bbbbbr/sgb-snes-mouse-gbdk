.ramsection "LowRam" bank 0 slot 0

w00:
    ds $900

wMenuStatusBitTemp:
    db
wUnused:
    db

w902:    
    ds $c21 - $902

wCurrMenuCursorX: ; $c21
    db

wCurrMenuCursorY: ; $c22
    db


wc23:
    ds $c43 - $c23

wMenuHeldTimoutGamepad: ; $c43  ; 0x00 - 0x27 = Menu action not timed out, pressing paired button can open, 0x28 = Menu won't open when other button pressed
    db
wc44:                           ; Unknown      
    db
wMenuHeldTimoutMouse:   ; $c45  ; 0x00 - 0x27 = Menu action not timed out, pressing paired button can open, 0x28 = Menu won't open when other button pressed
    db
wc46:                           ; Unknown
    ds $c4f - $c46
wMenuActiveFeature:     ; $c4f  ; 0xFF = menu closed, other values indicate menu(0x00) or submenus active
    db


wc50:
    ds $f30 - $c50

; currently polled
wIsMouseConnected:         ; $f30
    ds 2
wTargetMouseSensitivity:   ; $f32
    ds 2
wCurrentMouseSensitivity:  ; $f34
    ds 2
wVerticalMouseMovement:    ; $f36
    ds 2
wHorizontalMouseMovement:  ; $f38
    ds 2
wCurrMouseRLbits:          ; $f3a
    ds 2


.ends
