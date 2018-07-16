/*
 * image to ASCII art converter
 *
 */

#include "util.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <SDL2/SDL_image.h> /* TODO: switch to a proper image loading lib instead of SDL2 */

#include <getopt.h>



static const char *OPTSTRING = "is:a:";
static const struct option LONGOPTS[] =
    {   { "invert",       no_argument, NULL, 'i' },
        { "size",   required_argument, NULL, 's' },
        { "alpha",  required_argument, NULL, 'a' },
        { "help",         no_argument, NULL, 'h' },
        { NULL,           no_argument, NULL,  0  }
    };


const char *pname;

enum usagemode {
    USAGE_HELP,
    USAGE_ERROR
};
void print_usage (enum usagemode mode);



/* ascii: creates ASCII art images from bitmaps */
int main (int argc, char *argv[]) {

    pname = argv[0];


    char FLAG_invert = 0;

    char *filename = NULL;

    double x_step,
           y_step;
    int out_width = 0;

    SDL_Surface *img;


    /* command-line options */
    int opt;
    while ((opt = getopt_long (argc, argv, OPTSTRING, LONGOPTS, NULL)) != -1) {
        switch (opt) {
        /* -i : invert darkness values */
        case 'i':
            FLAG_invert = 1;
            break;

        /* -s : lock to a specific size */
        case 's':
          { char *end;
            errno = 0;
            out_width = strtol (optarg, &end, 0);
            if (errno != 0)                     fatal ("%s - %s", optarg, strerror (errno));
            if (!(*end == 0 && *optarg != 0))   fatal ("'%s' is not a number", optarg);
          } break;

        /* -a : set the minimum alpha before the pixel is ignored */
        case 'a':
          { char *end;
            errno = 0;
            min_alpha = strtol (optarg, &end, 0);
            if (errno != 0)                     fatal ("%s - %s", optarg, strerror (errno));
            if (!(*end == 0 && *optarg != 0))   fatal ("'%s' is not a number", optarg);
          } break;

        /* -h : print help */
        case 'h':
            print_usage (USAGE_HELP);
            break;

        /* unknown arg */
        case '?':
            print_usage (USAGE_ERROR);
            break;

        default:
            print_usage (USAGE_ERROR);
            break;
        }
    }

    /* get filename */
    if (optind < argc)
        filename = argv[optind];
    else
        fatal ("no filename supplied!");


    /* verify file exists/was supplied */
    FILE *t = fopen (filename, "r");
    if (!t)
        fatal ("Failed to open '%s' (%s)", filename, strerror (errno));
    fclose (t);


    /* load image */
    IMG_Init (IMG_INIT_PNG);
    atexit (IMG_Quit);

    img = IMG_Load (filename);
    if (!img)
        fatal ("failed to load image '%s'! (%s)", filename, IMG_GetError());


    /* set the output width */
    if (out_width <= 0)
        out_width = (img->w > 80)? 80 : img->w;


    /* set x/y step */
    double temp = (double)img->w / (double)out_width;
    x_step = temp? temp : 1;
    y_step = 2 * x_step;


    /* create ASCII image */
    for (double y = 0; y < img->h; y += y_step) {
        for (double x = 0; x < img->w; x += x_step)
            putchar (get_char (area_avgcolour (img,
                                          x,y,
                                          x_step,x_step),
                               img->format,
                               FLAG_invert));
        putchar ('\n');
    }
    return EXIT_SUCCESS;
}



/* print_usage: print usage information */
void print_usage (enum usagemode mode) {

    if (mode == USAGE_HELP) {

        printf ("Usage: %s [OPTION]... IMAGE\n", pname);
        printf ("Convert IMAGE to ASCII art\n\n"      );

        puts ("  -i, --invert              invert the colours, eg for white-on-black display");
        puts ("  -s, --size SIZE           set the size of the output in characters"         );
        puts ("  -a, --alpha ALPHA         set the minimum alpha for a pixel to be ignored"  );
        puts ("      --help                display this help and exit\n"                     );
    }
    else if (mode == USAGE_ERROR) {
        fprintf (stderr, "Usage: %s [OPTION]... IMAGE\n"          , pname);
        fprintf (stderr, "Try '%s --help' for more information.\n", pname);
        exit (EXIT_FAILURE);
    }
    exit (EXIT_SUCCESS);
}

