

#include "util.h"



/* darkness: get the "darkness" (ie greyscale value) of some pixel */
uint8_t darkness (uint32_t pixel, SDL_PixelFormat *f) {

    uint8_t r,g,b;
    SDL_GetRGB (pixel, f, &r,&g,&b);

    return (r + g + b) / 3;
}

/* get_char: get the ASCII char for some pixel */
char get_char (uint32_t pixel, SDL_PixelFormat *f, char invert) {

    const static int max = sizeof(ascii_ch);
    int transparency = f->Amask? (pixel & f->Amask) >> 24 : 255;

    int adj = transparency / max;
    if (!adj)
        return max-1;

    int r = darkness (pixel, f) / adj;

    if (r >= max)
        r = max-1;
    else if (r < 0)
        r = 0;

    return ascii_ch[invert? (max-1) - r : r];
}

/* get_block: read a block of pixels from the image,
    returns the average colour of the block */
uint32_t get_block (SDL_Surface *img, int x, int y, int w, int h) {

    uint8_t  r,g,b,a;
    unsigned long r_avg,g_avg,b_avg,a_avg;

    r_avg = g_avg = b_avg = 0;

    for (int i = 0; i < w; i += (w/5)? w/5 : 1)
        for (int j = 0; j < h; j += (h/5)? h/5 : 1) {
            SDL_GetRGBA (get_pixel (img, x+i, y+j),
                img->format, &r,&g,&b,&a);
            r_avg += r;
            g_avg += g;
            b_avg += b;
            a_avg += a;
        }
    r_avg /= w*h;
    g_avg /= w*h;
    b_avg /= w*h;
    a_avg /= w*h;

    return SDL_MapRGBA (img->format, r,g,b,a);
}

/* get_pixel: returns the pixel in surface at x,y */
uint32_t get_pixel (SDL_Surface *surface, int x, int y) {

    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return 0;

    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y *
                surface->pitch + x * bpp;

    switch(bpp) {

    /* 8 bit */
    case 1:
        return *p;
        break;

    /* 16 bit */
    case 2:
        return *(Uint16 *)p;
        break;

    /* 24 bit */
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    /* 32 bit */
    case 4:
        return *(Uint32 *)p;
        break;

    /* never happens */
    default:
        return 0;
        break;
    }
}

