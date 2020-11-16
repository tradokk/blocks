#!/usr/bin/python
#(C) David Leiter 2017

import sys
import math
from PIL import Image

tile_size = 8

if len(sys.argv) < 4:
    sys.stderr.write("error: not enough arguments\n")
    sys.stderr.write("usage: createfont name input output\n")
    exit(1)

name = sys.argv[1]

try:
    with Image.open(sys.argv[2]) as img, open(sys.argv[3],
                                              mode="w") as output_file:
        if img.mode == '1':
            bpp = 1
        elif img.mode == 'L':
            bpp = 1
        elif img.mode == 'P':
            bpp = 2
        else:
            sys.stderr.write("error: unsupported image\n")
            exit(1)

        image_width = img.size[0]
        image_height = img.size[1]
        ppb = 8 // bpp
        horizontal_count = image_width // tile_size
        vertical_count = image_height // tile_size
        tile_count = horizontal_count * vertical_count
        buffer_size = image_width * image_height // ppb

        output_file.write("/*auto generated file*/\n")
        output_file.write("\n")
        output_file.write('#include "resource.h"\n')
        output_file.write("\n")
        output_file.write("static const uint8_t data[" + str(buffer_size) +
                          "]=\n{\n")

        for vcount in range(vertical_count):
            yoffset = vcount * tile_size
            for hcount in range(horizontal_count):
                xoffset = hcount * tile_size
                for y in range(tile_size):
                    for i in range(tile_size // ppb):
                        byte = 0
                        for j in range(ppb):
                            val = img.getpixel(
                                (xoffset + i * ppb + j, yoffset + y))
                            byte = byte << bpp
                            if bpp == 1:
                                if val > 0:
                                    byte |= 1
                            else:
                                byte |= (val & 0x3)
                        output_file.write(hex(byte) + ",\n")
        output_file.write("};\n")
        output_file.write("const struct TileSet " + name.lower() + "_tiles = {")
        output_file.write(
            str(tile_count) + "," + str(bpp) + "," + str(buffer_size) +
            ",data};\n")
except IOError:
    sys.stderr.write("error: could not open file\n")
