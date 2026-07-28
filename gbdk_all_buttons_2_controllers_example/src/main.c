#include <gbdk/platform.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <gbdk/console.h>
#include "sgb_input_remap.h"

#include "../res/snespad.h"

const uint8_t sprite_data[] = {
    0x3C,0x3C,0x7E,0x42,0xFF,0x99,0xFF,0xA9,0xFF,0x89,0xFF,0x89,0x7E,0x42,0x3C,0x3C,
    0x3C,0x3C,0x7E,0x42,0xFF,0xB9,0xFF,0x89,0xFF,0x91,0xFF,0xB9,0x7E,0x42,0x3C,0x3C,
};

const uint8_t  sprite_tile_highlight[] = {
    0x3C,0x3C,0x7E,0x7E,0xFF,0xE7,0xFF,0xC3,0xFF,0xC3,0xFF,0xE7,0x7E,0x7E,0x3C,0x3C,  // Outlined circle
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00   // Transparent
};

joypads_t joypads;


#define SPR_TILE_GAMEPAD_START   2u
#define SPR_TILE_GAMEPAD_COUNT   2u
#define SPR_TILE_GAMEPAD_ON      (SPR_TILE_GAMEPAD_START + 0)
#define SPR_TILE_GAMEPAD_OFF     (SPR_TILE_GAMEPAD_START + 1)

#define SPR_GAMEPAD_START        2u
#define GAMEPAD_COUNT            2u
#define BUTTONS_PER_GAMEPAD     12u
#define BUTTONS_TOTAL           (BUTTONS_PER_GAMEPAD * GAMEPAD_COUNT)

#define SPR_OFFSET_GAMEPAD_2_Y  72u

typedef struct spr_pos {
    uint8_t x;
    uint8_t y;
} spr_pos;

const spr_pos gamepad_sprite_pos[BUTTONS_PER_GAMEPAD] = {
    // Gamepad 1
    {.x=56u,  .y=32u},  // Right
    {.x=39u,  .y=32u},  // Left
    {.x=48u,  .y=41u},  // Down
    {.x=48u,  .y=24u},  // Up
    {.x=81u,  .y=36u},  // Start
    {.x=68u,  .y=36u},  // Select
    {.x=101u, .y=35u},  // Y
    {.x=112u, .y=42u},  // B

    {.x=111u, .y=8u},   // R
    {.x=48u,  .y=8u},   // L
    {.x=110u, .y=25u},  // X
    {.x=121u, .y=33u},  // A
};

bool snes_gamepad_button_on[BUTTONS_TOTAL];


void gamepad_highlights_zero(void) {
    for (uint8_t c = 0; c < (GAMEPAD_COUNT * BUTTONS_PER_GAMEPAD); c++) {
        snes_gamepad_button_on[c] = false;
    }
}


void gamepad_highlights_update(void) {

    for (uint8_t gp = 0; gp < GAMEPAD_COUNT; gp++) {
        uint8_t spr_base = SPR_GAMEPAD_START + (gp * BUTTONS_PER_GAMEPAD);
        uint8_t button_base = (gp * BUTTONS_PER_GAMEPAD);

        for (uint8_t btn = 0; btn < BUTTONS_PER_GAMEPAD; btn++) {
            uint8_t spr_num = spr_base + btn;
            if (snes_gamepad_button_on[button_base + btn])  // Sprites start at an offset, rewind array index by that much
                set_sprite_tile(spr_num, SPR_TILE_GAMEPAD_ON);
            else
                set_sprite_tile(spr_num, SPR_TILE_GAMEPAD_OFF);
        }
    }

}


void input_init(void) {
    // init joypads
    joypad_init(4, &joypads);

    // Init mouse hook and handler
    sgb_input_remap_install();    
}


void input_update(void) {

    // poll joypads
    joypad_ex(&joypads);

    if (joypads.npads == 4) {

        // Invert button bits data to be active high instead of active low
        for (uint8_t c = 0; c < 4; c++)
            joypads.joypads[c] = ~joypads.joypads[c];

        // Update the gamepad display bits used for showing/hiding sprites
        uint8_t button_data;
        uint8_t bit;
        uint8_t spr_num = 0;
        uint8_t gp_id   = 0;
        // Loop through gamepads
        for (uint8_t gp = 0; gp < GAMEPAD_COUNT; gp++) {
            // Lower Byte
            bit = 0x01;
            button_data = joypads.joypads[gp_id++];
            for (uint8_t c = 0; c < 8; c++) {
                snes_gamepad_button_on[spr_num++] = (button_data & bit);
                bit <<= 1;
            }
            // Upper Byte
            bit = 0x10; // Button data starts in upper nibble
            button_data = joypads.joypads[gp_id++];
            for (uint8_t c = 0; c < 4; c++) {
                snes_gamepad_button_on[spr_num++] = (button_data & bit);
                bit <<= 1;
            }
        }
        gamepad_highlights_update();

        // Print out the raw data
        gotoxy(0,0);
        printf("1:%hx\n", (uint8_t)joypads.joypads[0]);
        printf("2:%hx\n", (uint8_t)joypads.joypads[1]);
        printf("3:%hx\n", (uint8_t)joypads.joypads[2]);
        printf("4:%hx\n", (uint8_t)joypads.joypads[3]);

        // Check both players D-Pads for moving sprites around
        // (first and third bytes: [0] = Player 1, [3] = Player 2
        for (uint8_t i = 0; i < 2; i++) {
            uint8_t joy = joypads.joypads[i*2];
            if (joy & SNES_BTN_LEFT)  scroll_sprite(i, -1, 0);
            if (joy & SNES_BTN_RIGHT) scroll_sprite(i, 1, 0);
            if (joy & SNES_BTN_UP)    scroll_sprite(i, 0, -1);
            if (joy & SNES_BTN_DOWN)  scroll_sprite(i, 0, 1);
        }

        // Start on joypad 1 resets position
        if (joypads.joy0 & SNES_BTN_START) {
            for (uint8_t i = 0; i < 2; i++) move_sprite(i, (i << 3) + 64, 64);
        }        
    }

}


void init(void) {
    gamepad_highlights_zero();

    // A printf to let the printing system perform it's clear and load tiles
    // Do it now before loading the BG map so that it doesn't get overwritten
    printf(" ");


    // Sprites movable with D-Pad    
    set_sprite_data(0, 2, sprite_data);
    for (uint8_t i = 0; i < 2; i++) {
        set_sprite_tile(i, i);
        move_sprite(i,
                    DEVICE_SPRITE_PX_OFFSET_X + (i << 3) + ((DEVICE_SCREEN_PX_WIDTH - (4 * 8)) / 2), 
                    DEVICE_SPRITE_PX_OFFSET_Y + ((DEVICE_SCREEN_PX_HEIGHT - 8) / 2));
    }

    // Highlight sprites
    set_sprite_data(SPR_TILE_GAMEPAD_START, SPR_TILE_GAMEPAD_COUNT, sprite_tile_highlight);

    gamepad_highlights_update();
    for (uint8_t gp = 0; gp < GAMEPAD_COUNT; gp++) {
        uint8_t spr_base = SPR_GAMEPAD_START + (gp * BUTTONS_PER_GAMEPAD);

        for (uint8_t btn = 0; btn < BUTTONS_PER_GAMEPAD; btn++) {
            uint8_t spr_num = spr_base + btn;
            move_sprite(spr_num,
                        gamepad_sprite_pos[btn].x + DEVICE_SPRITE_PX_OFFSET_X - 3,
                        gamepad_sprite_pos[btn].y + (SPR_OFFSET_GAMEPAD_2_Y * gp) + DEVICE_SPRITE_PX_OFFSET_Y - 3);
        }
    }
    OBP0_REG = DMG_PALETTE(DMG_WHITE, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK); 

    SHOW_SPRITES;
    DISPLAY_ON;
    SHOW_BKG;

    // Align map bg tiles at end of tile data to make room for printf font at the start
    #define BK_TILE_ST (255u - (uint8_t)snespad_TILE_COUNT)
    set_bkg_data(BK_TILE_ST, snespad_TILE_COUNT, snespad_tiles);
    set_bkg_based_tiles(0,0, snespad_TILES_WIDTH, snespad_TILES_HEIGHT, snespad_map, BK_TILE_ST);
}


void main(void) {

    init();

    // Wait 4 frames
    // For SGB on PAL SNES this delay is required on startup, otherwise borders don't show up
    for (uint8_t i = 4; i != 0; i--) vsync();

    if (sgb_check()) {
    
        input_init();

        while(1) {
            input_update();
            vsync();
        }
    }
}
