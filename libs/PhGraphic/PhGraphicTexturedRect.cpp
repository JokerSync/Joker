/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicTexturedRect.h"

#include <stdio.h>
#include <stdlib.h>

PhGraphicTexturedRect::PhGraphicTexturedRect(int x, int y, int z, int w, int h, PhColor color, int tv, int tu) :
    PhGraphicRect(x, y, z, w, h, color)
{
    _tu = tu;
    _tv = tv;

    //Witch will be overwrited by createTextureFromSurface if is an PhGraphicImage
    createTextureFromColor(color);

}


void PhGraphicTexturedRect::createTextureFromColor(PhColor color){

    GLubyte Texture[4] =
    {
        color.red(),
        color.green(),
        color.blue(),
        255,
    };

    GLenum textureFormat = 0;

    textureFormat = GL_RGBA;

    glEnable( GL_TEXTURE_2D );
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &_texture );

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, _texture );


    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D (
                GL_TEXTURE_2D, 	//Type : texture 2D
                0, 	//Mipmap : aucun
                4, 	//Couleurs : 4
                1, 	//Largeur : 2
                1, 	//Hauteur : 2
                0, 	//Largeur du bord : 0
                GL_RGBA, 	//Format : RGBA
                GL_UNSIGNED_BYTE, 	//Type des couleurs
                Texture 	//Addresse de l'image
                );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

void PhGraphicTexturedRect::draw(int scroll){


    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int x = this->getX();
    int w = this->getWidth();



    // only draw object from 3000px before screen and 3000px after and rythmo
    if(-scroll + 3000 > x && -scroll - 3000 - w < x){
        glBindTexture(GL_TEXTURE_2D, _texture);
        if (w == 240)
            glDisable(GL_BLEND);

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS); 	//Begining the cube's drawing



        int y = this->getY();
        int h = this->getHeight();
        int z = this->getZ();

        /*
        (0,0) ------ (1,0)
          |            |
          |            |
        (0,1) ------ (1,1)
        */
        glTexCoord3i(0, 0, 1);glVertex3i(x + scroll,         y,      z);
        glTexCoord3i(_tv, 0, 1);glVertex3i(x + w * _tv + scroll,     y,      z);
        glTexCoord3i(_tv, _tu, 1);glVertex3i(x + w * _tv + scroll,     y + h * _tu,  z);
        glTexCoord3i(0, _tu, 1);glVertex3i(x + scroll,         y + h * _tu,  z);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}
GLuint PhGraphicTexturedRect::getTexture(){
    return _texture;
}
