/*   SDLMain.m - main entry point for our Cocoa-ized SDL app
       Initial Version: Darrell Walisser <dwaliss1@purdue.edu>
       Non-NIB-Code & other changes: Max Horn <max@quendi.de>

    Feel free to customize this file to suit your needs
*/

#include "SDL.h"
#include "SDLMain.h"
#include <sys/param.h> /* for MAXPATHLEN */
#include <unistd.h>


/* Use this flag to determine whether we use CPS (docking) or not */
#define		SDL_USE_CPS		1
#ifdef SDL_USE_CPS
/* Portions of CPS.h */
typedef struct CPSProcessSerNum
{
    UInt32		lo;
    UInt32		hi;
} CPSProcessSerNum;

extern OSErr	CPSGetCurrentProcess( CPSProcessSerNum *psn);
extern OSErr 	CPSEnableForegroundOperation( CPSProcessSerNum *psn, UInt32 _arg2, UInt32 _arg3, UInt32 _arg4, UInt32 _arg5);
extern OSErr	CPSSetFrontProcess( CPSProcessSerNum *psn);

#endif /* SDL_USE_CPS */

static int    gArgc;
static char  **gArgv;
static BOOL   gFinderLaunch;
static BOOL   gCalledAppMainline = FALSE;


@interface NSApplication (SDLApplication)
@end

@implementation NSApplication (SDLApplication)
/* Invoked from the Quit menu item */
- (void)terminate:(id)sender
{
    /* Post a SDL_QUIT event */
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}
@end

/* The main class of the application, the application's delegate */
@implementation SDLMain

/* Set the working directory to the .app's parent directory */
- (void) setupWorkingDirectory:(BOOL)shouldChdir
{
    if (shouldChdir)
    {
        char parentdir[MAXPATHLEN];
        CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
        CFURLRef url2 = CFURLCreateCopyDeletingLastPathComponent(0, url);
        if (CFURLGetFileSystemRepresentation(url2, 1, (UInt8 *)parentdir, MAXPATHLEN)) {
            chdir(parentdir);   /* chdir to the binary app's parent */
        }
        CFRelease(url);
        CFRelease(url2);
    }
}

/* Replacement for NSApplicationMain */
static void CustomApplicationMain (int argc, char **argv)
{
    NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];
    SDLMain				*sdlMain;

    /* Ensure the application object is initialised */
    [NSApplication sharedApplication];

#ifdef SDL_USE_CPS
    {
        CPSProcessSerNum PSN;
        /* Tell the dock about us */
        if (!CPSGetCurrentProcess(&PSN))
            if (!CPSEnableForegroundOperation(&PSN,0x03,0x3C,0x2C,0x1103))
                if (!CPSSetFrontProcess(&PSN))
                    [NSApplication sharedApplication];
    }
#endif /* SDL_USE_CPS */

    /* Create SDLMain and make it the app delegate */
    sdlMain = [[SDLMain alloc] init];
    [NSApp setDelegate:sdlMain];

    /* Start the main event loop */
    [NSApp run];

    [sdlMain release];
    [pool release];
}

/* Called when the internal event loop has just started running */
- (void) applicationDidFinishLaunching: (NSNotification *) note
{
    int status;

    /* Set the working directory to the .app's parent directory */
    [self setupWorkingDirectory:gFinderLaunch];

    /* Hand off to main application code */
    gCalledAppMainline = TRUE;
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
    /* Copy the arguments into a global variable */
    /* This is passed if we are launched by double-clicking */
    if ( argc >= 2 && strncmp (argv[1], "-psn", 4) == 0 ) {
        gArgv = (char **) SDL_malloc(sizeof (char *) * 2);
        gArgv[0] = argv[0];
        gArgv[1] = NULL;
        gArgc = 1;
        gFinderLaunch = YES;
    } else {
        int i;
        gArgc = argc;
        gArgv = (char **) SDL_malloc(sizeof (char *) * (argc+1));
        for (i = 0; i <= argc; i++)
            gArgv[i] = argv[i];
        gFinderLaunch = NO;
    }

    CustomApplicationMain (argc, argv);

    return 0;
}

