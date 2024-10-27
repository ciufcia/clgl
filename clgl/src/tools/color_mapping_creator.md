# CLCM - Command Line Color Mappings

The file format consists of **16,777,216 (256^3) 8-bit** sections, that store the required data about each one of 24-bit colors. For example the first section holds the data for color #000000, second for #000001, and so on, until #FFFFFF.

Sections are split into 2 subsections described below: <br/>

    (bits counted from right to left)

    [bits 0 - 4] - big endian unsigned integer describing the console color code associated with the color

    [bits 5 - 8] - big endian unsigned integer describing the brightness of the color (0 is black, 15 is white)