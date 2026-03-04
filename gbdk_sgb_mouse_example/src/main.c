#include <gbdk/platform.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <gbdk/console.h>
#include "sgb_mouse_on_gb.h"


const uint8_t sprite_data[] = {
    0x3C,0x3C,0x42,0x7E,0x99,0xFF,0xA9,0xFF,0x89,0xFF,0x89,0xFF,0x42,0x7E,0x3C,0x3C,
    0x3C,0x3C,0x42,0x7E,0xB9,0xFF,0x89,0xFF,0x91,0xFF,0xB9,0xFF,0x42,0x7E,0x3C,0x3C,
    0x3C,0x3C,0x42,0x7E,0x99,0xFF,0x89,0xFF,0x99,0xFF,0x89,0xFF,0x5A,0x7E,0x3C,0x3C,
    0x3C,0x3C,0x42,0x7E,0xA9,0xFF,0xA9,0xFF,0xB9,0xFF,0x89,0xFF,0x42,0x7E,0x3C,0x3C 
};

joypads_t joypads;


void input_init(void) {
    // init joypads
    joypad_init(4, &joypads);

    // Init mouse hook and handler
    sgb_mouse_install();    
}


void input_update(void) {

    // poll joypads
    joypad_ex(&joypads);

    if (joypads.npads == 4) {

        // Main controller
        uint8_t joy = joypads.joypads[0];
        if (joy & J_LEFT)  scroll_sprite(0, -1,  0);
        if (joy & J_RIGHT) scroll_sprite(0,  1,  0);
        if (joy & J_UP)    scroll_sprite(0,  0, -1);
        if (joy & J_DOWN)  scroll_sprite(0,  0,  1);

        // Mouse
        if ((joypads.joy3 & SNES_MOUSE_IS_CONNECTED_MASK) == SNES_MOUSE_IS_CONNECTED) {

            // Don't use mouse data while SGB menu is open/visible
            // since it may result in unintended mouse movement and clicking
            if (joypads.joy3 & SNES_MOUSE_SGB_MENU_OPEN) {
                gotoxy(2,2);
                printf("SGB Menu Open");
            }
            else  {
                gotoxy(2,2);
                printf("%hx,%hx,%hx     \n", (uint8_t)joypads.joy1, (uint8_t)joypads.joy2, (uint8_t)joypads.joy3);

                // // ===== RELATIVE MODE USING MOUSE IN SGB MOUSE HARDWARE FORMAT =====
                //
                int8_t mouse_x_move = (joypads.joy1 & SNES_MOUSE_X_MASK);
                if (joypads.joy1 & SNES_MOUSE_X_DIR) mouse_x_move *= -1;

                int8_t mouse_y_move = (joypads.joy2 & SNES_MOUSE_Y_MASK);
                if (joypads.joy2 & SNES_MOUSE_Y_DIR) mouse_y_move *= -1;

                uint8_t mouse_buttons = joypads.joy3;

                // Visualize mouse data (move cursor 2 with x,y. move cursor 3 with buttons)
                scroll_sprite(1, mouse_x_move, mouse_y_move);
                if (mouse_buttons & SNES_MOUSE_BUTTON_LEFT)  scroll_sprite(2, -1, 0);
                if (mouse_buttons & SNES_MOUSE_BUTTON_RIGHT) scroll_sprite(2,  1, 0);
            }
        }

        // start on joypad 1 resets position
        if (joypads.joy0 & J_START) {
            for (uint8_t i = 0; i < 3; i++) move_sprite(i, (i << 3) + 64, 64);
        }
    } else {
        gotoxy(2,2);
        printf("Mouse Not Found");
    }

}


void main(void) {

    set_sprite_data(0, 3, sprite_data);
    for (uint8_t i = 0; i < 3; i++) {
        set_sprite_tile(i, i);
        move_sprite(i,
                    DEVICE_SPRITE_PX_OFFSET_X + (i << 3) + ((DEVICE_SCREEN_PX_WIDTH - (4 * 8)) / 2), 
                    DEVICE_SPRITE_PX_OFFSET_Y + ((DEVICE_SCREEN_PX_HEIGHT - 8) / 2));
    }
    SHOW_SPRITES;

    DISPLAY_ON;


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
