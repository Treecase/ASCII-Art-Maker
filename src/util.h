


#ifndef _UTIL_H
#define _UTIL_H


#include <stdio.h>
#include <SDL2/SDL.h>


#define error(...)  {fprintf (stderr, "Error: %s - ", __func__);\
                     fprintf (stderr, __VA_ARGS__);}
#define fatal(...)  {error(__VA_ARGS__);\
                     exit (1);}


/* characters to use, in order lightest to darkest */
static const char ascii_ch[] = { 'M','B','R','X','Y','V','I','t','i','+',';',':',',','.',' ' };


uint8_t  darkness (uint32_t pix, SDL_PixelFormat *f);
char     get_char (uint32_t pix, SDL_PixelFormat *f, char invert);

uint32_t get_block (SDL_Surface *img, int x, int y, int w, int h);
uint32_t get_pixel (SDL_Surface *img, int x, int y);


#endif

