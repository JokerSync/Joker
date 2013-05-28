/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"

GLuint createTextureFromSurface(SDL_Surface * surface)
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

    GLuint texture;

    glEnable( GL_TEXTURE_2D );
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &texture );

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, texture );


    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nbOfColors, surface->w, surface->h, 0,
                  textureFormat, GL_UNSIGNED_BYTE, surface->pixels );

    //    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}

GLuint createSurfaceFromText(PhString text){
    GLuint texture = 0;
    SDL_Surface *surface = NULL;
    // Initialize SDL_TTF :
    if (TTF_Init() == 0){;
        SDL_Color textColor={ 230, 0, 0, 0 };
        // Create a font:
        TTF_Font *font = TTF_OpenFont("../Resources/fonts/zoinks.ttf", 100);
        if (font != NULL)
        {
            // Create a surface from a string:
            surface = TTF_RenderText_Blended(font, text.toStdString().c_str(), textColor);


            // Create a texture from this surface
            if(surface != NULL)
                 texture = createTextureFromSurface(surface);
        }
        else
            qDebug() << "Error (Font) : " << TTF_GetError();
    }
    else
        qDebug() << "Error (Font) : " << TTF_GetError();

    return texture;
}
