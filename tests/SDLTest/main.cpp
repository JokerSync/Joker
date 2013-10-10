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

// Synchronize buffer swaps with vertical refresh rate

SDL_Surface *load_image( QString filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

	QFileInfo info(filename);
	if(!info.exists())
	{
		PHDEBUG << "file doesn't exists : " << filename;
		return NULL;
	}
    //Load the image
    loadedImage = IMG_Load( filename.toLocal8Bit());

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );

            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }
    else
    {
           PHDEBUG<<"IMG_Load: "<<IMG_GetError();
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

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
    image = load_image( "SDLTest.app/Contents/Resources/look.png");

    if( image == NULL )
    {
        return 1;
    }

    // Display the picture:
    apply_surface( 0, 0, image, screen );


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
    apply_surface( ( SCREEN_WIDTH - surface->w ) / 2, ( SCREEN_HEIGHT / 2 - surface->h ) / 2, surface, screen );


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


