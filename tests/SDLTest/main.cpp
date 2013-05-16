//
//  main.cpp
//  Test SDL
//
//  Created by Thomas on 15/04/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"
#include <string>
#include <cstring>
#include "string.h"
#include "iostream"
//#include "SDLMain.h"

#include <QString>
#include <QDebug>

#include "PhTools/memorytool.h"

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

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

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
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
        return false;

    //Set the title
    SDL_WM_SetCaption( "TTF's just the best", NULL );

    // Create a surface from picture:
    const char * imagePath = "../../../../../data/look.png";
    image = load_image( imagePath );

    if( image == NULL )
    {
        qDebug("Error loading %s", imagePath);
        return false;
    }

    // Display the picture:
    apply_surface( 0, 0, image, screen );


    // Initialize TTF :
    if( TTF_Init() == -1 ) {
        qDebug() << "TTF error.";
        return false;
    }

    //Create a font
    TTF_Font *font = TTF_OpenFont( "../../../../../data/Bedizen.ttf", 100 );
    if (font == NULL)
        return false;

    //Font's color
    SDL_Color textColor = { 255, 255, 0, 1 };

    // Create a text surface:
    SDL_Surface *surface = TTF_RenderUTF8_Blended( font, ".....", textColor );

    qDebug("surface : %dx%d / %dbpp / %d", surface->w, surface->h,
           surface->format->BytesPerPixel, surface->pitch);

    // Dump the text surface:
    //MemoryDump(surface->pixels, surface->pitch, surface->h, surface->format->BytesPerPixel);

    // Display the text surface:
    apply_surface( ( SCREEN_WIDTH - surface->w ) / 2, ( SCREEN_HEIGHT / 2 - surface->h ) / 2, surface, screen );


    //Update the screen
    if( SDL_Flip(screen) == -1 )
        return 1;

    bool quit = true;

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


