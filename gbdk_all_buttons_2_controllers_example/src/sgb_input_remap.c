#include <gbdk/platform.h>
#include <gb/sgb.h>


#define ARRAY_LEN(A)  (sizeof(A) / sizeof(A[0]))

#define SGB_PACKET_SIZE 16u
#define SGB_PAYLOAD_LEN (SGB_PACKET_SIZE - 1u)

// Hook:
//                                   RTS
//                                   ||
// Power-up: 60 A8 74 5A 2D FA BC FC 60 5A AF 08 E7 C3 F7 1B
//
// Modified: 60 A8 74 5A 2D FA BC FC 4C 02 09 08 E7 C3 F7 1B
//                                   |------|
//                                    JMP ....
//
// https://github.com/bbbbbr/sgb-testbed/blob/a10011c71f6ed66e3f16a7c097611a9e899b3eb5/snes_src/hacks/mouseHook.s#L3
//
// .org $808
// 
// PreGBMainLoopHook:
//     jmp $902
// 
//                                   DATA_SND,Addr-Low/Hi, Bank, Len,  Payload Data.....................................................
const uint8_t sgb_input_hook[]       = { 0x79, 0x08, 0x08, 0x00, 0x03, 0x4c, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
const uint8_t sgb_input_hook_reset[] = { 0x79, 0x08, 0x08, 0x00, 0x03, 0x60, 0x5A, 0xAF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


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
// HookInner:
// https://github.com/bbbbbr/sgb-testbed/blob/a10011c71f6ed66e3f16a7c097611a9e899b3eb5/snes_src/hacks/mouseInner.s#L16
//
//                                  DATA_SND,Addr-Low/Hi, Bank, Len,  Payload Data...................................................

const uint8_t sgb_input_handler_0[] = { 0x79, 0x02, 0x09, 0x00, 0x0b, 0xa9, 0x28, 0x8d, 0x43, 0x0c, 0x8d, 0x44, 0x0c, 0xad, 0x12, 0x0f, };
const uint8_t sgb_input_handler_1[] = { 0x79, 0x0d, 0x09, 0x00, 0x0b, 0x8f, 0x04, 0x60, 0x00, 0xad, 0x11, 0x0f, 0x8f, 0x05, 0x60, 0x00, };
const uint8_t sgb_input_handler_2[] = { 0x79, 0x18, 0x09, 0x00, 0x0b, 0xad, 0x14, 0x0f, 0x8f, 0x06, 0x60, 0x00, 0xad, 0x13, 0x0f, 0x8f, };
const uint8_t sgb_input_handler_3[] = { 0x79, 0x23, 0x09, 0x00, 0x0b, 0x07, 0x60, 0x00, 0xa2, 0x00, 0xaf, 0xdb, 0xff, 0x00, 0xf0, 0x05, };
const uint8_t sgb_input_handler_4[] = { 0x79, 0x2e, 0x09, 0x00, 0x0a, 0x68, 0x68, 0x4c, 0xaa, 0xba, 0x68, 0x68, 0x4c, 0xad, 0xba, 0x00, };


// After installation:
// Map the extra SNES controller/gamepad buttons from Players 1 and 2
// into button data for SGB Players 3 and 4
void sgb_input_remap_install(void) {

    // First uninstall the hook in case it was previously running and this is
    // a soft-reset, such as pushing menu button on an EverDrive and loading new ROM
    sgb_transfer(sgb_input_hook_reset);

    // Then install the button remap handler before enabling the hook
    sgb_transfer(sgb_input_handler_0);
    sgb_transfer(sgb_input_handler_1);
    sgb_transfer(sgb_input_handler_2);
    sgb_transfer(sgb_input_handler_3);
    sgb_transfer(sgb_input_handler_4);

    // Then install the hook
    sgb_transfer(sgb_input_hook);
}
