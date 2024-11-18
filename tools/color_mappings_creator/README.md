# CLCM - Command Line Color Mappings

The color mapping file is split into 2 parts:

- The first section consists of **384 bits** representing the 16 color console palette associated with the color mappings
  First 24 bits represent the big endian RGB values of the first color in the palette, the next 24 bits represent the second color and so on, until the 16th color

- The second part consists of **8,388,608 (256^3 / 2) 24-bit** sections, that store the required data about two of the 24-bit colors. For example the first section holds the data for color #000000 and #000001, second for #000002 and #000003, and so on, until #FFFFFE and #FFFFFF.

  Sections are split into **4 subsections** described below: <br/>

        (bits counted from right to to left)

        0 0 0 0 0 0 0 0    0 0 0 0    0 0 0 0    0 0 0 0 0 0 0 0
        \_____________/    \_____/    \_____/    \_____________/
              \ /            \ /        \ /            \ /
               A              B          C              D

        A) [bits 0 - 7]   - big endian unsigned integer representing the brightness of the first color

        B) [bits 8 - 11]  - big endian unsigned integer representing the console color code of the first color

        C) [bits 12 - 15] - big endian unsigned integer representing the console color code of the second color

        D) [bits 16 - 23] - big endian unsigned integer representing the brightness of the second color

# Generating CLCMs

To generate a CLCM:

- Go to tools/color_mappings_creator/color_mappings_config.hpp and modify it to your liking

- Compile the color_mappings_creator target

- Run the resulting executable

A file with the name color_mappings.clcm should now be created in the same directory as the executable