/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicTexturedRect.h"

PhGraphicTexturedRect::PhGraphicTexturedRect(int x, int y, int z, int w, int h, PhColor color) :
    PhGraphicRect(x, y, z, w, h, color)
{

}

void PhGraphicTexturedRect::createTextureFromSurface(SDL_Surface *surface)
{
    // get the number of channels in the SDL surface
    GLint  nbOfColors = surface->format->BytesPerPixel;
    GLenum textureFormat = 0;

    switch (nbOfColors) {
    case 1:
        textureFormat = GL_ALPHA;
        break;
    case 3:     // no alpha channel
        if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGB;
        else
            textureFormat = GL_BGR;
        break;
    case 4:     // contains an alpha channel
        if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
        else
            textureFormat = GL_BGRA;
        break;
    default:
        qDebug() << "Warning: the image is not truecolor...";
        break;
    }

    glEnable( GL_TEXTURE_2D );
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &_texture );

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, _texture );


    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nbOfColors, surface->w, surface->h, 0,
                  textureFormat, GL_UNSIGNED_BYTE, surface->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

//void PhGraphicTexturedRect::setTexture(GLuint texture){
//    _texture = texture;
//}

//SDL_Surface * PhGraphicTexturedRect::getSurface(){
//    return *_surface;
//}

void PhGraphicTexturedRect::draw(){

    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, _texture);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS); 	//Begining the cube's drawing

    int x = this->getX();
    int y = this->getY();
    int w = this->getWidth();
    int h = this->getHeight();
    int z = this->getZ();
    //qDebug() << "Hi! Here's my size : " << h << w;
    //qDebug() << "And my position : " << x << y << z;

    /*
    (0,0) ------ (1,0)
      |            |
      |            |
    (0,1) ------ (1,1)
    */
    glTexCoord3i(0, 0, 1);glVertex3i(x,         y,      z);
    glTexCoord3i(1, 0, 1);glVertex3i(x + w,     y,      z);
    glTexCoord3i(1, 1, 1);glVertex3i(x + w,     y + h,  z);
    glTexCoord3i(0, 1, 1);glVertex3i(x,         y + h,  z);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}
GLuint PhGraphicTexturedRect::getTexture(){
    return _texture;
}
