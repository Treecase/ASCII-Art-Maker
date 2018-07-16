

ASCII Art Maker
===============


A program that converts images to ASCII art. The conversion is darkness based.


To convert some image (img.png);

    `./ASCII-art img.png`


The size (in characters) of the output can be changed, too:

    `./ASCII-art -s [SIZE] img.png`
    `./ASCII-art --size [SIZE] img.png`


The colours can be inverted (eg for use in a terminal):

    `./ASCII-art -i img.png`
    `./ASCII-art --invert img.png`

Additionally, for images with transparency, the minimum alpha for a pixel to be ignored can be set:

    `./ASCII-art -a [ALPHA] img.png`
    `./ASCII-art --alpha [ALPHA] img.png`

