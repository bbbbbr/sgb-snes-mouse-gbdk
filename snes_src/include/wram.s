.ramsection "LowRam" bank 0 slot 0

w00:
    ds $c21

wCurrMenuCursorX: ; $c21
    db

wCurrMenuCursorY: ; $c22
    db

wc23:
    ds $f30-$c23

; currently polled
wIsMouseConnected:  ; $f30
    ds 2
wTargetMouseSensitivity:  ; $f32
    ds 2
wCurrentMouseSensitivity:  ; $f34
    ds 2
wVerticalMouseMovement:  ; $f36
    ds 2
wHorizontalMouseMovement: ; $f38
    ds 2
wCurrMouseRLbits: ; $f3a
    ds 2


.ends
