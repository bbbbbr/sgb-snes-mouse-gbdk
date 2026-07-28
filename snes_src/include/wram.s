.ramsection "LowRam" bank 0 slot 0

; 2f2: Maybe number of players?

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
    ds $f11 - $c50

wJoyPad1Lo:                ; $f11
    ds 1
wJoyPad1Hi:                ; $f12
    ds 1
wJoyPad2Lo:                ; $f13
    ds 1
wJoyPad2Hi:                ; $f14
    ds 1
wJoyPad3Lo:                ; $f15
    ds 1
wJoyPad3Hi:                ; $f16
    ds 1
wJoyPad4Lo:                ; $f17
    ds 1
wJoyPad4Hi:                ; $f18
    ds 1


wf19:
    ds $f30 - $f19

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
