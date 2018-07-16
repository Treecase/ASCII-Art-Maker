


#ifndef _UTIL_H
#define _UTIL_H


#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


#define error(format, ...)  ({  fprintf (stderr, "Error: " format "\n", ##__VA_ARGS__);   })
#define fatal(format, ...)  ({  error(format, ##__VA_ARGS__); exit (EXIT_FAILURE);  })

/* defined in util.c */
extern unsigned min_alpha;

/* characters to use, in order lightest to darkest */
static const char ascii_ch[] = {    'M','B','R','X','Y',
                                    'V','I','t','i','+',
                                    ';',':',',','.',' '
                               };


Uint8  darkness (Uint32 pix, SDL_PixelFormat *f);
char     get_char (Uint32 pix, SDL_PixelFormat *f, bool invert);

Uint32 area_avgcolour (SDL_Surface *img, int x, int y, int w, int h);
Uint32 get_pixel (SDL_Surface *img, int x, int y);


#endif

