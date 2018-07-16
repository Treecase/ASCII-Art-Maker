/*
 * ASCII art maker utility functions
 *
 */

#include "util.h"



#define LEN(arr)    (sizeof(arr)/sizeof(*arr))


unsigned min_alpha = 255;



/* darkness: get the `darkness' (ie greyscale value) of some pixel */
Uint8 darkness (Uint32 pixel, SDL_PixelFormat *f) {

    Uint8 r, g, b;
    SDL_GetRGB (pixel, f, &r, &g, &b);

    return (r + g + b) / 3;
}

/* get_char: get the ASCII char for some pixel */
char get_char (Uint32 pixel, SDL_PixelFormat *f, bool invert) {

    static const int max = LEN(ascii_ch) - 1;


    Uint8 alpha = 255;
    if (f->Amask != 0) {
        alpha = ((pixel & f->Amask) >> f->Ashift) << f->Aloss;
        if (alpha < min_alpha)
            return ' ';
    }

    int r;
    int dark = darkness (pixel, f);
    if (dark == 0)
        r = 0;
    else if (alpha >= dark)
        r = (double)max / ((double)alpha / (double)dark);
    else
        r = max;

    return ascii_ch[invert? max - r : r];
}

/* area_avgcolour: get the average colour of an area in the image */
Uint32 area_avgcolour (SDL_Surface *img, int x, int y, int w, int h) {

    Uint8         col[4] = { 0,0,0,0 };
    unsigned long avg[4] = { 0,0,0,0 };

    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j) {
            SDL_GetRGBA (get_pixel (img, x, y),
                         img->format,
                         &col[0], &col[1],
                         &col[2], &col[3]);
            for (int n = 0; n < 4; ++n)
                avg[n] += col[n];
        }
    for (int n = 0; n < 4; ++n)
        avg[n] /= w * h;

    /* map colour to 4-byte int */
    return SDL_MapRGBA (img->format, (Uint8)avg[0], (Uint8)avg[1], (Uint8)avg[2], (Uint8)avg[3]);
}

/* get_pixel: get pixel in surface at x,y */
Uint32 get_pixel (SDL_Surface *surface, int x, int y) {

    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return 0;

    int    bpp = surface->format->BytesPerPixel;

    switch(bpp) {
    /* 8 bit */
    case 1:
        return *(Uint8 *)(surface->pixels + y * surface->pitch  + x * bpp);
        break;

    /* 16 bit */
    case 2:
        return *(Uint16 *)(surface->pixels + y * surface->pitch  + x * bpp);
        break;

    /* 24 bit */
    case 3:
      { Uint8 *p   = (Uint8 *)surface->pixels + y *
                          surface->pitch  + x * bpp;
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
      } break;

    /* 32 bit */
    case 4:
        return *(Uint32 *)(surface->pixels + y * surface->pitch  + x * bpp);
        break;

    /* never happens */
    default:
        return 0;
        break;
    }
}

