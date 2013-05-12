#include <QDebug>

#include "sdlwidget.h"

SDLWidget::SDLWidget(int width, int height, QWidget *parent) :
    QWidget(parent)
{
    this->initOk = false;

    resize(width, height);
}

SDLWidget::~SDLWidget()
{
    this->dispose();
}

void SDLWidget::showEvent(QShowEvent *)
{
    qDebug() << "showEvent()";
    init();
}

bool SDLWidget::init()
{
    if(!initOk)
    {
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_NoSystemBackground);
        // Here's the part where we put SDL in a Qt Widget
        char windowid[64];

#ifdef Q_WS_WIN
        sprintf(windowid, "SDL_WINDOWID=0x%lx", reinterpret_cast<qlonglong>(winId()));
#else
        sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
#endif

        SDL_putenv(windowid);
        qDebug() << "SDLWiget id :" << windowid;

        // Initialisation du système vidéo de SDL
        SDL_Init(SDL_INIT_VIDEO);
        int flags = SDL_SWSURFACE;// | SDL_FULLSCREEN;
        this->screen = SDL_SetVideoMode(this->width(), this->height(), 32, flags);

        /*      TTF_Init();
        font = TTF_OpenFont( "../../data/zoinks.ttf", 30 );

        image = IMG_Load( "../../data/look.png" );

        SDL_Color textColor={ 0, 0, 255 };
        txt = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );
*/
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
        t_Timer->start( 0);

        initOk = true;
    }

    return initOk;
}

void SDLWidget::dispose()
{
    if(initOk)
    {
        //Free the surface
        //      SDL_FreeSurface( image );
        //    SDL_FreeSurface(txt);
        //  SDL_FreeSurface(screen);

        //Quit SDL
        //TTF_CloseFont( font );
       // TTF_Quit();

        SDL_Quit();

        initOk = false;
    }
}

void SDLWidget::onRefresh()
{
//    qDebug() << "onRefresh()";
}


