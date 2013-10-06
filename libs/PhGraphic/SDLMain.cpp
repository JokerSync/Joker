#include "SDL/SDL.h"

#ifdef main
#  undef main
#endif

/* Main entry point to executable - should *not* be SDL_main! */
int main (int argc, char **argv)
{
	return SDL_main(argc, argv);
}

