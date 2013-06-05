/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QDateTime> // For screenshot name

#include "PhGraphicContext.h"
#include "PhGraphicView.h"

PhGraphicContext::PhGraphicContext(PhGraphicView *view)
{
    _graphVeiw = view;
    init();
}

void PhGraphicContext::saveToPNG()
{
    QImage impr(_graphVeiw->grabFrameBuffer());
    QDateTime now(QDateTime::currentDateTime());
    QString s = QDir::homePath()+"/Phonations-" + now.toString("dd-MM-yy-h-m-s")  + ".png";
    impr.save(s);
}

void PhGraphicContext::saveToPNG(PhString number){
    QImage impr(_graphVeiw->grabFrameBuffer());
    QDateTime now(QDateTime::currentDateTime());
    QString s = QDir::homePath()+"/Phonations-Serie-" + now.toString("h-m-s") + "-" + number  + ".png";
    impr.save(s);
}


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
