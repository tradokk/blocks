#!/usr/bin/env python
#(C) David Leiter 2017

import sys
import math
from PIL import Image

font_width=8

if len(sys.argv) < 3:
    sys.stderr.write("error: not enough arguments\n")
    sys.stderr.write("usage: createfont name input output\n")
    exit(1)
name=sys.argv[1]
try:
    with Image.open(sys.argv[2]) as img, open(sys.argv[3],mode="w") as output_file:
        if img.mode!='L' and img.mode!='1':
            sys.stderr.write("error: unsupported image\n")
            exit(1)
        image_width=img.size[0]
        image_height=img.size[1]
        font_height=image_height
        glyph_count=image_width//font_width
        buffer_size=font_height*glyph_count
        
        output_file.write("/*auto generated file*/\n")
        output_file.write("\n")
        output_file.write("#include<resource.h>\n")
        output_file.write("\n")
        output_file.write("static uint8_t data["+str(buffer_size)+"]=\n{\n")
        
        for i in range(glyph_count):
            xpos=i*font_width
            for y in range(font_height):
                byte=0
                for x in range(font_width):
                    byte<<=1
                    if img.getpixel((x+xpos,y)) > 0:
                        byte|=1
                output_file.write(hex(byte)+",")
            output_file.write("  //"+str(i)+"\n")
        output_file.write("};\n");
        output_file.write("struct GlyphMap "+name.lower()+"_glyphs = {")
        output_file.write(str(glyph_count)+","+str(buffer_size)+",data};\n")
except IOError:
    sys.stderr.write("error: could not open file\n")
