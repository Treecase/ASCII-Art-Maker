

#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL_image.h>

#include "util.h"



void printusage (char *name, char help);



/* ascii: creates ASCII art images from bitmaps */
int main (int argc, char *argv[]) {


    atexit (IMG_Quit);


    char FLAG_invert;

    char *filename;
    int x_step, y_step;
    int out_width;

    SDL_Surface *img;


    /* handle args */
    filename = NULL;
    FLAG_invert = out_width = 0;

    if (argc <= 1)
        printusage (argv[0], 0);

    for (int i = 1, skip = 0; i < argc; ++i, skip = 0) {
        if (argv[i][0] == '-') {
            for (int j = 1; j < strlen (argv[i]); ++j)
                switch (argv[i][j]) {

                /* long style options*/
                case '-':
                    if (!strcmp (argv[i], "--help"))
                        printusage (argv[0], 1);
                    else if (!strcmp (argv[i], "--invert"))
                        FLAG_invert = 1;
                    else if (!strcmp (argv[i], "--size")) {
                        if (i+1 >= argc)
                            fatal ("incorrect size option!\n");
                        out_width = atoi (argv[i+1]);
                        skip = 1;
                    }
                    j = strlen (argv[i]);
                    break;

                /* -i : invert darkness values */
                case 'i':
                    FLAG_invert = 1;
                    break;

                /* -s : lock to a specific size */
                case 's':
                    if (i+1 >= argc)
                        fatal ("incorrect size option!\n");
                    out_width = atoi (argv[i+1]);
                    skip = 1;
                    break;

                /* -h : print help */
                case 'h':
                    printusage (argv[0], 1);
                    break;

                /* unknown option */
                default:
                    error ("unknown option '-%c'\n", argv[i][j]);
                    break;
                }
        }
        else {
            if (!filename)
                filename = argv[i];
            else
                error ("unknown argument '%s'\n", argv[i]);
        }
        i += skip;
    }


    /* verify file exists/was supplied */
    if (filename) {
        FILE *t = fopen (filename, "r");
        if (!t) {
            fatal ("Failed to open '%s' (%s)\n", filename, strerror (errno));
            exit (1);
        }
        fclose (t);
    }
    else
        fatal ("no filename supplied!\n");


    /* load image */
    IMG_Init (IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);

    img = IMG_Load (filename);
    if (!img)
        fatal ("failed to load image '%s'! (%s)\n", filename, IMG_GetError());


    /* decide the output width (if not supplied) */
    if (!out_width || out_width < 0) {
        if (img->w > 80)
            out_width = 80;
        else
            out_width = img->w;
    }
    if (out_width > img->w)
        out_width = img->w;

    /* create image from ASCII */
    int temp = img->w / out_width;
    x_step = temp? temp : 1;
    y_step = 2 * x_step;

    for (int y = 0; y < img->h; y += y_step) {
        for (int x = 0; x < img->w; x += x_step)
            putchar (get_char (get_block (img, x,y, x_step,x_step),
                img->format, FLAG_invert));
        putchar ('\n');
    }


    /* IMG_Quit is called automatically by atexit */
    return 0;
}

/* printusage: print usage information */
void printusage (char *name, char help) {

    printf ("Usage: %s [OPTION]... IMAGE\n", name);
    puts   ("Try '%s --help' for more information.");

    if (help) {
        putchar ('\n');
        puts ("  -i, --invert                  invert the colours");
        puts ("  -s, --size                    set the size of the output in characters");
        puts ("  -h, --help                    print this help");
        putchar ('\n');
    }
    exit (0);
}

