#include "dwwindow.h"
#include "customwindow.h"
#include <QApplication>
#include <QPushButton>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"
#include <string>
#include <cstring>
#include "string.h"
#include "iostream"



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


using namespace std;

// Synchronize buffer swaps with vertical refresh rate

SDL_Surface *load_image( std::string filename )
{
    SDL_Init(SDL_INIT_VIDEO);
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );


    //Return the optimized image
    return loadedImage;
}
bool init()
{


    SDL_EnableKeyRepeat(300, SDL_DEFAULT_REPEAT_INTERVAL);

    if (TTF_Init())
        return false;
    return true;}

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

bool load_files()
{
    //Load the image
    image = load_image( "../../data/look.png" );
    //If there was an error in loading the image
    if( image == NULL )
        return false;
    //Load the font
    font = TTF_OpenFont( "../../data/zoinks.ttf", 30 );
    if (font == NULL)
        return false;
    //If everything loaded fine
    return true;
}

int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);
    DWwindow w;


    //w.show();

    if( init() == false )
        return 1;

    //Load the files
    if( load_files() == false )
        return 2;

    w.apply_surface((w.getScreen()->w - image->w )/2, (w.getScreen()->h - image->h )/2, image, w.getScreen() );
    w.UpdateSurface();

    //clean_up();
    return a.exec();
}
