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

    //The font
TTF_Font *font;

    //Font's color
SDL_Color textColor = { 0, 0, 255, 0 };

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

bool init()
{
        //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

        //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

        //If there was an error in setting up the screen
    if( screen == NULL )
        return false;
        //If there was an error with font init
    if( TTF_Init() == -1 ) {
        return false;
    }

    SDL_EnableKeyRepeat(300, SDL_DEFAULT_REPEAT_INTERVAL);

        //Set the title
    SDL_WM_SetCaption( "TTF's just the best", NULL );


        //If everything initialized fine
    return true;
}

void clean_up()
{
        //Free the surface
    SDL_FreeSurface( image );
    SDL_FreeSurface(message);
    SDL_FreeSurface(screen);

        //Quit SDL
    TTF_CloseFont( font );
    TTF_Quit();
    SDL_Quit();
}

    // Don't work with specials char
bool saisieTexte(bool fini){

    int a, b;
    SDL_GetMouseState(&a, &b);
    char msg[100] = {0};
    int positionCurseur = 0;
    SDL_EnableUNICODE(1);

    while(!fini)
    {
        SDL_WaitEvent(&event);
        SDL_Surface *msgtest;
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_BACKSPACE)
            {
                    // If it's not at the first row
                if (positionCurseur > 0) {
                    positionCurseur--; // going 1 row back
                    msg[positionCurseur] = '\0'; // erase old value
                }
            }
            else if(event.key.keysym.sym == SDLK_RETURN)
            {
                SDL_EnableUNICODE(0); // Disable UNICODE
                fini = true; // end of typing
            }
            else if (event.key.keysym.sym == SDLK_LSHIFT or event.key.keysym.sym == SDLK_RSHIFT or event.key.keysym.sym == SDLK_CAPSLOCK ){}
            else
            {
                msg[positionCurseur] = event.key.keysym.unicode;
                positionCurseur++;
                msg[positionCurseur] = '\0'; // Set this row free for next char
            }
                //cout << msg << endl;

            msgtest = TTF_RenderUTF8_Blended( font, msg, textColor );
            apply_surface( 0, 0, image, screen );
            apply_surface(a, b, msgtest, screen );
            if( SDL_Flip( screen ) == -1 ) {
                exit(1);
            }
        }
    }
    return true;
}

bool load_files(){
        //Load the image
    image = load_image( "/Users/thomas/Stage/Joker/data/look.png" );
    foo = load_image("/Users/thomas/Stage/Joker/data/foo.png");
        //If there was an error in loading the image
    if( image == NULL )
        return false;
        //Load the font
    font = TTF_OpenFont( "/Users/thomas/Stage/Joker/data/Bedizen.ttf", 10 );
    if (font == NULL)
        return false;


        //If everything loaded fine
    return true;
}

int main(int argc, char **argv)
{
    //Make sure the program waits for a quit
    bool quit = false;

        //Initialize
    if( init() == false )
        return 1;

        //Load the files
    if( load_files() == false )
        return 1;

        //If there was a problem in loading the image
    if( image == NULL )
        return 1;

        //image = load_image( "/Users/thomas/Stage/Joker/data/look.png" );
        //SDL_FillRect(image, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    apply_surface( 0, 0, image, screen );

        //Update the screen
    if( SDL_Flip(screen) == -1 )
        return 1;



        //While the user hasn't quit
    while(quit == false) {
            //While there's an event to handle
        while( SDL_PollEvent(&event) ) {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT ) { //Quit the program
                quit = true; }
            if( event.type == SDL_KEYDOWN ) {
                    // Creation of message's surfaces
                SDL_Surface *up = TTF_RenderUTF8_Blended( font, "Les", textColor );
                SDL_Surface *down = TTF_RenderUTF8_Blended( font, "Bas a été pressé.", textColor );
                SDL_Surface *left = TTF_RenderUTF8_Blended( font, "Gauche a été pressé", textColor );
                SDL_Surface *right = TTF_RenderUTF8_Blended( font, "Droite a été pressé", textColor );
                Uint8 *keystates = SDL_GetKeyState( NULL );



                if( keystates[ SDLK_UP ] ) {
                    cout << up->h << "x" << up->w << endl;
                    apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen );
                }
                if( keystates[ SDLK_DOWN ] ) {
                    cout << down->h << "x" << down->w << endl;
                    apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen );
                }
                if( keystates[ SDLK_LEFT ] ) {
                    apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen );
                }
                if( keystates[ SDLK_RIGHT ] ) {
                    apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2 + SCREEN_WIDTH / 2, ( SCREEN_HEIGHT - left->h ) / 2, right, screen );
                }
                if (keystates[ SDLK_r]) {
                        //apply_surface( 0, 0, image, screen );
                    apply_surface( 0, 0, image, screen );
                }
                if (keystates[SDLK_z]) {
                        //SDL_Rect sizeFoo = {0,0,160,120};
                    SDL_Rect sizeScreen = {0,0,SCREEN_WIDTH - 10,SCREEN_HEIGHT - 10 };

                        //SDL_SoftStretch(foo, NULL, screen, &sizeScreen);
                    SDL_SoftStretch(foo, NULL, screen, &sizeScreen);
                    cout << "Taille du rect de foo: " << sizeScreen.x << endl;
                }
                if (keystates[ SDLK_e]) {
                    bool fini = false;
                    saisieTexte(fini);
                    }

                if (keystates[ SDLK_d]) {
                        // Scrolling a surface
                    SDL_Surface *txt = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles seches?", textColor );
                    int i = 0;
                    int facteur = 20;
                    while (i <= SCREEN_WIDTH + txt->w) {
                            // Writing on the screen
                        apply_surface(SCREEN_WIDTH - i, SCREEN_HEIGHT - 100 , txt, screen );
                            // Updating the screen
                        SDL_Flip(screen);
                            // Cleanning the screen
                        apply_surface( 0, 0, image, screen );
                            // nb of pixels between each scroll
                        i+=facteur;
                        SDL_Delay(100);
                        if( keystates[ SDLK_RIGHT ] ) {
                            facteur += 1;
                        }
                        if( keystates[ SDLK_LEFT ] ) {
                            facteur -= 1;
                        }
                    }

                }



                if (keystates[ SDLK_q]) {
                    quit = true;
                }
                    //Update the screen
                if( SDL_Flip( screen ) == -1 ) {
                    return 1;
                }
            }
        }
    }

        //Free the surface and quit SDL
    clean_up();
    return 0;
}


