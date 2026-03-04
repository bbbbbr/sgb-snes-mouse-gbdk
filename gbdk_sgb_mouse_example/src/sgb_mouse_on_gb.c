#include <gbdk/platform.h>
#include <gb/sgb.h>


#define ARRAY_LEN(A)  (sizeof(A) / sizeof(A[0]))

#define SGB_PACKET_SIZE 16u
#define SGB_PAYLOAD_LEN (SGB_PACKET_SIZE - 1u)

// MouseHook:
// https://github.com/bbbbbr/sgb-testbed/blob/a10011c71f6ed66e3f16a7c097611a9e899b3eb5/snes_src/hacks/mouseHook.s#L3
//
// .org $808
// 
// PreGBMainLoopHook:
//     jmp $902
// 
//                                  DATA_SND,Addr-Low/Hi, Bank, Len,  Payload Data.....................................................
const uint8_t sgb_mouse_hook[]      = { 0x79, 0x08, 0x08, 0x00, 0x03, 0x4c, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


// SGB Command $0F — DATA_SND
// Byte  Content
// 0     Command*8+Length    (fixed length=1)
// 1     SNES Destination Address, low
// 2     SNES Destination Address, high
// 3     SNES Destination Address, bank number
// 4     Number of bytes to write ($01-$0B)
// 5     Data Byte #1
// 6     Data Byte #2 (if any)
// 7     Data Byte #3 (if any)
// etc.

// Modified from original version which had SNES cursor overlays and sent absolute screen positions to GB
//
// MouseInner:
// https://github.com/bbbbbr/sgb-testbed/blob/a10011c71f6ed66e3f16a7c097611a9e899b3eb5/snes_src/hacks/mouseInner.s#L16
//
//                                  DATA_SND,Addr-Low/Hi, Bank, Len,  Payload Data...................................................
const uint8_t sgb_mouse_handler_0[] = { 0x79, 0x02, 0x09, 0x00, 0x0b, 0xad, 0x39, 0x0f, 0x49, 0xff, 0x8f, 0x05, 0x60, 0x00, 0xad, 0x37, };
const uint8_t sgb_mouse_handler_1[] = { 0x79, 0x0d, 0x09, 0x00, 0x0b, 0x0f, 0x49, 0xff, 0x8f, 0x06, 0x60, 0x00, 0xad, 0x31, 0x0f, 0x29, };
const uint8_t sgb_mouse_handler_2[] = { 0x79, 0x18, 0x09, 0x00, 0x0b, 0x01, 0xd0, 0x05, 0xa9, 0x00, 0x4c, 0x36, 0x09, 0xa9, 0xff, 0xed, };
const uint8_t sgb_mouse_handler_3[] = { 0x79, 0x23, 0x09, 0x00, 0x0b, 0x4f, 0x0c, 0xf0, 0x02, 0xa9, 0x04, 0x8d, 0x00, 0x09, 0xad, 0x3b, };
const uint8_t sgb_mouse_handler_4[] = { 0x79, 0x2e, 0x09, 0x00, 0x0b, 0x0f, 0x29, 0x03, 0x09, 0xa0, 0x0d, 0x00, 0x09, 0x49, 0xff, 0x8f, };
const uint8_t sgb_mouse_handler_5[] = { 0x79, 0x39, 0x09, 0x00, 0x0b, 0x07, 0x60, 0x00, 0xa2, 0x00, 0xaf, 0xdb, 0xff, 0x00, 0xf0, 0x08, };
const uint8_t sgb_mouse_handler_6[] = { 0x79, 0x44, 0x09, 0x00, 0x0b, 0x20, 0xa0, 0xbc, 0x68, 0x68, 0x4c, 0xaa, 0xba, 0x20, 0xa3, 0xbc, };
const uint8_t sgb_mouse_handler_7[] = { 0x79, 0x4f, 0x09, 0x00, 0x05, 0x68, 0x68, 0x4c, 0xad, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


// After installation the mouse data will be copied
// into P2-4 (X -> 2, Y -> 3, Buttons -> 4)
void sgb_mouse_install(void) {

    // Install the mouse handler first
    sgb_transfer(sgb_mouse_handler_0);
    sgb_transfer(sgb_mouse_handler_1);
    sgb_transfer(sgb_mouse_handler_2);
    sgb_transfer(sgb_mouse_handler_3);
    sgb_transfer(sgb_mouse_handler_4);
    sgb_transfer(sgb_mouse_handler_5);
    sgb_transfer(sgb_mouse_handler_6);
    sgb_transfer(sgb_mouse_handler_7);

    // Then install the hook
    sgb_transfer(sgb_mouse_hook);
}
