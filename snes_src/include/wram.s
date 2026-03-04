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
    ds $c4f - $c23

wSelectedMenuFeature:  ; $c4f  ; 0xFF = menu closed, other values indicate menu(0x00) or submenus active
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
