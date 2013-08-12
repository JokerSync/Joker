/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QDateTime> // For screenshot name

#include "PhGraphicContext.h"
#include "PhGraphicView.h"

#include <SDL/SDL.h>

#if defined(Q_OS_MAC)
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif


PhGraphicContext::PhGraphicContext(PhGraphicView *view)
{
    _graphVeiw = view;
    init();
}

void PhGraphicContext::saveToPNG()
{
    QImage impr(_graphVeiw->grabFrameBuffer());
    QDateTime now(QDateTime::currentDateTime());
    if(!QDir(QDir::homePath()+"/Phonations/").exists())
        QDir().mkdir(QDir::homePath()+"/Phonations/");
    QString s = QDir::homePath()+"/Phonations/" +"Phonations-" + now.toString("dd-MM-yy-h-m-s")  + ".png";
    impr.save(s);
}

void PhGraphicContext::saveToPNG(QString number){
    QImage impr(_graphVeiw->grabFrameBuffer());
    QDateTime now(QDateTime::currentDateTime());
    if(!QDir(QDir::homePath()+"/Phonations/").exists())
        QDir().mkdir(QDir::homePath()+"/Phonations/");
    QString s = QDir::homePath()+"/Phonations/"+"Phonations-serie" + now.toString("h-m-s") + "-" + number  + ".png";
    impr.save(s);
}

//void PhGraphicContext::exportToPng()
//{
//    //_strip->setXmove(0);
//    // As 1920px is 4 sec, 1 min is 28800 px
//    int nbFrames = (28800 / _graphVeiw->width()) + 1;
//    for(int i = 0; i < nbFrames ; i++){
//        // Save the current frame buffer
//        saveToPNG(QString::number(i));
//        // Scroll the strip of the window width
//        _graphVeiw->setXmove(_graphVeiw->width());
//        // Re paint
//        _graphVeiw->paintGL();
//    }
//}


void PhGraphicContext::init(){
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
        qDebug() << "init SDL Ok.";
    else
        qDebug() << "SDL error:" << SDL_GetError();
    if (TTF_Init() == 0)
        qDebug() << "init TTF Ok.";
    else
        qDebug() << "TTF error:" << TTF_GetError();


    glClearColor(1,1,1,0); 	//Background color RGBA
    glEnable(GL_DEPTH_TEST); 	//Activate the depth test
    glEnable(GL_TEXTURE_2D); 	//Activate the texturing

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}
