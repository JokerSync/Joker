#include "SDL/SDL.h"
#include "SDLMain.h"

static int    gArgc;
static char  **gArgv;

/* The main class of the application, the application's delegate */
@implementation SDLMain

/* Called when the internal event loop has just started running */
- (void) applicationDidFinishLaunching: (NSNotification *) note
{
    int status;

    /* Hand off to main application code */
    status = SDL_main (gArgc, gArgv);

    /* We're done, thank you for playing */
    exit(status);
}
@end

#ifdef main
#  undef main
#endif


/* Main entry point to executable - should *not* be SDL_main! */
int main (int argc, char **argv)
{
	int i;
	gArgc = argc;
	gArgv = (char **) SDL_malloc(sizeof (char *) * (argc+1));
	for (i = 0; i <= argc; i++)
		gArgv[i] = argv[i];

	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];
    SDLMain				*sdlMain;

    /* Ensure the application object is initialised */
    [NSApplication sharedApplication];

    /* Create SDLMain and make it the app delegate */
    sdlMain = [[SDLMain alloc] init];
    [NSApp setDelegate:sdlMain];

    /* Start the main event loop */
    [NSApp run];

    [sdlMain release];
    [pool release];

	return 0;

	//return SDL_main(argc, argv);
}

