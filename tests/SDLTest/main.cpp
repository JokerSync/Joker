#include <QtGlobal>

#include <SDL/SDL.h>
#if defined(Q_OS_MAC)
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#endif


#include <string>
#include <cstring>
#include "string.h"
#include "iostream"
//#include "SDLMain.h"

#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

#include "PhTools/PhDebug.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *foo = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *message = NULL;

//The event
SDL_Event event;

using namespace std;

int main(int argc, char **argv)
{
	PHDEBUG << "SDL_Init()";
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

	PHDEBUG << "SDL_SetVideoMode";
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
        return false;

	PHDEBUG << "SDL_WM_SetCaption";
    //Set the title
    SDL_WM_SetCaption( "Graphic Test", NULL );

	PHDEBUG << "current path : " << QDir::currentPath();
	PHDEBUG << "load_image";
    // Create a surface from picture:
    image = IMG_Load( "SDLTest.app/Contents/Resources/look.png");

    if( image == NULL )
    {
        return 1;
    }

    // Display the picture:
	SDL_Rect imageRect = {0, 0, image->w, image->h};

	SDL_BlitSurface( image, NULL, screen, &imageRect );


    // Initialize TTF :
    if( TTF_Init() == -1 ) {
        PHDEBUG << "TTF error.";
        return 2;
    }

    //Create a font
    TTF_Font *font = TTF_OpenFont("SDLTest.app/Contents/Resources/Bedizen.ttf", 100 );
    if (font == NULL)
        return 3;

    //Font's color
    SDL_Color textColor = { 255, 255, 0, 1 };

    // Create a text surface:
    SDL_Surface *surface = TTF_RenderUTF8_Blended( font, "SDL test OK", textColor );

    PHDEBUG << "surface :" << surface->w << "x" << surface->h << "/" << surface->format->BytesPerPixel << "/" << surface->pitch;

    // Dump the text surface:
    //MemoryDump(surface->pixels, surface->pitch, surface->h, surface->format->BytesPerPixel);

    // Display the text surface:
	SDL_Rect textRect = {0, 0, surface->w, surface->h};
	SDL_BlitSurface( surface, NULL, screen, &textRect );

	//Update the screen
    if( SDL_Flip(screen) == -1 )
        return -1;

    bool quit = false;

    //While the user hasn't quit
    while(quit == false) {
        //While there's an event to handle
        while( SDL_PollEvent(&event) ) {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT ) { //Quit the program
                quit = true; }
        }
    }

    //Free the surface and quit SDL
    SDL_FreeSurface( image );
    SDL_FreeSurface(message);
    SDL_FreeSurface(screen);

    //Quit SDL
    TTF_CloseFont( font );
    TTF_Quit();
    SDL_Quit();

    return 0;
}


