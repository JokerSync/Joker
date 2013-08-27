/*   SDLMain.m - main entry point for our Cocoa-ized SDL app
       Initial Version: Darrell Walisser <dwaliss1@purdue.edu>
       Non-NIB-Code & other changes: Max Horn <max@quendi.de>

    Feel free to customize this file to suit your needs
*/

#include "SDL/SDL.h"

#ifdef main
#  undef main
#endif

/* Main entry point to executable - should *not* be SDL_main! */
int main (int argc, char **argv)
{
//    int    gArgc;
//    char  **gArgv;

//    /* This is passed if we are launched by double-clicking */
//    if ( argc >= 2 && strncmp (argv[1], "-psn", 4) == 0 ) {
//        gArgv = (char **) SDL_malloc(sizeof (char *) * 2);
//        gArgv[0] = argv[0];
//        gArgv[1] = NULL;
//        gArgc = 1;
//    } else {
//        int i;
//        gArgc = argc;
//        gArgv = (char **) SDL_malloc(sizeof (char *) * (argc+1));
//        for (i = 0; i <= argc; i++)
//            gArgv[i] = argv[i];
//    }

//    SDL_main (gArgc, gArgv);

	return SDL_main(argc, argv);
}

