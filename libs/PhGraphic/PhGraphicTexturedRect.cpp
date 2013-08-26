/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicTexturedRect.h"

PhGraphicTexturedRect::PhGraphicTexturedRect(int x, int y, int w, int h, int z, float tu, float tv, QColor *color)
	: PhGraphicRect(x, y, w, h , z, color), _tu(tu), _tv(tv)
{
}


void PhGraphicTexturedRect::createTextureFromColor(QColor color){

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

bool PhGraphicTexturedRect::createTextureFromSurface(SDL_Surface *surface)
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
		return false;

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

	return true;
}

void PhGraphicTexturedRect::draw()
{
//	qDebug() << "PhGraphicTexturedRect::draw()";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, _texture);

	glEnable(GL_TEXTURE_2D);

//        (0,0) ------ (1,0)
//          |            |
//          |            |
//        (0,1) ------ (1,1)

//		qDebug() << " x:" << x << " y:" << y << " z:" << z << " w:" << w << " h:" << h << " tu:" << _tu << " tv:" << _tv;

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glTexCoord3f(0, 0, 1);		glVertex3f(_x,		_y,	_z);
		glTexCoord3f(_tu, 0, 1);	glVertex3f(_x + _w,	_y,	_z);
		glTexCoord3f(_tu, _tv, 1);	glVertex3f(_x + _w,	_y + _h,  _z);
		glTexCoord3f(0, _tv, 1);	glVertex3f(_x,		_y + _h,  _z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void PhGraphicTexturedRect::setTextureCoordinate(float tu, float tv)
{
	_tu = tu;
	_tv = tv;
}

GLuint PhGraphicTexturedRect::getTexture()
{
    return _texture;
}
