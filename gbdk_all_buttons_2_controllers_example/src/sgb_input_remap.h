#ifndef _SGB_INPUT_REMAP_H
#define _SGB_INPUT_REMAP_H

// First Byte
#define SNES_BTN_B       0x80
#define SNES_BTN_Y       0x40
#define SNES_BTN_SELECT  0x20
#define SNES_BTN_START   0x10
#define SNES_BTN_UP      0x08
#define SNES_BTN_DOWN    0x04
#define SNES_BTN_LEFT    0x02
#define SNES_BTN_RIGHT   0x01
// Second Byte
#define SNES_BTN_A       0x80
#define SNES_BTN_X       0x40
#define SNES_BTN_L       0x20
#define SNES_BTN_R       0x10

void sgb_input_remap_install(void);

#endif // _SGB_INPUT_REMAP_H
