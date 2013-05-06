#include "dwwindow.h"
#include "ui_dwwindow.h"


DWwindow::DWwindow()
{
    this->windowInitialized = false;
    this->screen = new SDL_Surface();
    //setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    resize(640, 480);
    if(!windowInitialized)
    {
        // Here's the part where we put SDL in a Qt Widget
        char windowid[64];
        /*
#ifdef Q_WS_WIN
        sprintf(windowid, "SDL_WINDOWID=0x%lx", reinterpret_cast<qlonglong>(winId()));
#elif defined Q_WS_X11
        sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
#else
        printf("Fatal: cast du winId() inconnu pour votre plate-forme; toute information est la bienvenue!");
#endif
*/
        sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
        SDL_putenv(windowid);

        // Initialisation du système vidéo de SDL
        SDL_Init(SDL_INIT_VIDEO);
        this->setScreen(SDL_SetVideoMode(this->width(), this->height(), 32, SDL_SWSURFACE));
        windowInitialized = true;
    }
}

DWwindow::~DWwindow()
{
    SDL_Quit();
}

void DWwindow::setScreen(SDL_Surface *surface){
    this->screen = surface;
}

SDL_Surface* DWwindow::getScreen(){
    return this->screen;
}

bool DWwindow::UpdateSurface(){
    if(SDL_Flip(this->screen) == -1) return false;
    return true;

}

void DWwindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Space:
        close();
        break;
    case Qt::Key_Q:
        close();
        break;
    case Qt::Key_D:
        scroll();
        break;
    }
}
void DWwindow::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}
void DWwindow::scroll(){

    TTF_Font *font;
    TTF_Init();
    font = TTF_OpenFont( "/zoinks.ttf", 30 );
    SDL_Surface *image = NULL;

    image = IMG_Load( "/look.png" );


    // Scrolling a surface
    SDL_Color textColor={ 0, 0, 255 };
    SDL_Surface *txt = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );
    int i = 0;
    int facteur = 4;

    while (i <= this->width() + txt->w) {
        // Writing on the screen
        this->apply_surface(this->width() - i, 400, txt, this->getScreen());
        // Updating the screen
        this->UpdateSurface();
        // Cleanning the screen
        this->apply_surface( 0, 0, image,this->getScreen());
        // nb of pixels between each scroll
        i+=facteur;
    }
}
