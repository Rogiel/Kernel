#!/usr/bin/python

import sys, os

header = [0x02, 0xB0, 0xAD, 0x1B, 0x03, 0x00, 0x00, 0x00, 0xFB, 0x4F, 0x52, 0xE4]
kernel_file = sys.argv[1]

with open(kernel_file, 'rb') as file:
    data = bytearray(file.read())

if data[0:4] != bytearray([0x7F, 0x45, 0x4C, 0x46]):
    print("Not a ELF kernel file!")
    exit()

data[512:512 + len(header)] = bytearray(header)

with open(kernel_file, 'wb') as file:
    file.write(data)

