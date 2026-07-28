#!/usr/bin/env python3

import sys

args = len(sys.argv)
assert args > 1, "Pass in the starting address"
assert args == 2, "Too many arguments passed to this script"

start_addr = int(sys.argv[1], 16)
with open('main.bin', 'rb') as f:
    cmd_data = f.read()

cmd_len = len(cmd_data)
assert cmd_len > 0, "Input file is empty"


# Set up Command Type and Length (always 1)
SGB_CMD_DATA_SND = 0x0f
SGB_PACKET_HEADER = (SGB_CMD_DATA_SND << 3) | 1;

SGB_PACKET_SIZE = 16
SGB_PAYLOAD_LEN = (SGB_PACKET_SIZE - 1)

i = 0
packet_num = 0
max_param_len = 0xb
while i < cmd_len:
    packet_params = cmd_data[i:i+max_param_len]
    packet_bytes = [
        start_addr&0xff, (start_addr>>8)&0xff, start_addr>>16,
        len(packet_params),
        *packet_params, 
    ]
    data_bytes = ", ".join(f"0x{b:02x}" for b in packet_bytes)
    pad_length = SGB_PAYLOAD_LEN - len(packet_bytes)
    pad_bytes = "0x00, " * pad_length
    print(f'const uint8_t sgb_input_handler_{packet_num}[] = {{ 0x{SGB_PACKET_HEADER:02x}, {data_bytes}, {pad_bytes}}};')
    packet_num += 1
    i += max_param_len
    start_addr += max_param_len

