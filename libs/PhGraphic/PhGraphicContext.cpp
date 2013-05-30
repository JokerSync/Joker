/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicContext.h"

PhGraphicContext::PhGraphicContext()
{
    init();
}


void PhGraphicContext::init(){
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
        qDebug() << "SDL Ok.";
    else
        qDebug() << "SDL error:" << SDL_GetError();
    if (TTF_Init() == 0)
        qDebug() << "TTF Ok.";
    else
        qDebug() << "TTF error:" << TTF_GetError();


    glClearColor(.90,.90,.90,1.0); 	//Background color RGBA
    glEnable(GL_DEPTH_TEST); 	//Activate the depth test
    glEnable(GL_TEXTURE_2D); 	//Activate the texturing

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


}
