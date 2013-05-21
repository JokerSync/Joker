#include "TestSDLOpenGLWidget.h"

#include "glu.h"

#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"
#include "SDL/SDL.h"

#include "PhTools/memorytool.h"


TestSDLOpenGLWidget::TestSDLOpenGLWidget(QWidget *parent)
    : PhGLWidget( parent, "Premier affichage de dessin avec OpenGL et Qt")
{

    xmove = this->width() * 1.5;
}

GLuint createTextureFromSurface(SDL_Surface * surface)
{
/*    qDebug("surface : %dx%d / %dbpp / %x", surface->w, surface->h,
           surface->format->BytesPerPixel, surface->flags);

    MemoryDump(surface->pixels, surface->pitch/surface->format->BytesPerPixel,
               surface->h, surface->format->BytesPerPixel);
*/
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

void TestSDLOpenGLWidget::initializeGL()
{
    glClearColor(.90,.90,.90,1.0); 	//Background color RGBA
    glEnable(GL_DEPTH_TEST); 	//Activate the depth test
    glEnable(GL_TEXTURE_2D); 	//Activate the texturing

    // Initialize with a simple buffer
    GLubyte inlineBuffer[16] =
    {
        0,0,0,0, 0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF, 0,0,0,0
    };

    glGenTextures(1, &textures[0]); 	//Generate a texture at index 0
    glBindTexture(GL_TEXTURE_2D, textures[0]); 	//Select this texture
    glTexImage2D (
                GL_TEXTURE_2D,      //Type : texture 2D
                0,                  //Mipmap : none
                4,                  //Bpp : 4
                2,                  //height : 2
                2,                  //width : 2
                0,                  //border width : 0
                GL_RGBA,            //Format : RGBA
                GL_UNSIGNED_BYTE, 	//Colors type
                inlineBuffer             //Picture's address
                );

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Initializing SDL library
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        //Create a surface from a file:
        SDL_Surface *surface = IMG_Load("../../../../../../fond.jpg");

        // Create a texture from this surface
        if(surface != NULL)
            textures[1] = createTextureFromSurface(surface);

        // Initialize SDL_TTF :
        if (TTF_Init() == 0){;
            SDL_Color textColor={ 0, 0, 0, 0 };
            // Create a font:
            TTF_Font *font = TTF_OpenFont("../../../../../data/Bedizen.ttf", 100);
            if (font != NULL)
            {
                // Create a surface from a string:
                surface = TTF_RenderText_Blended(font, "Once upon a time...", textColor);

                // Create a texture from this surface
                if(surface != NULL)
                    textures[2] = createTextureFromSurface(surface);
            }
            else
                qDebug() << "Error (Font) : " << TTF_GetError();
        }
        else
            qDebug() << "Error (Font) : " << TTF_GetError();
    }
}

void TestSDLOpenGLWidget::paintGL()
{
    int x = this->width();
    int w = this->width() / 2;
    if (xmove > x + w)
        xmove = 0;

    if (move){
        xmove += xdelta;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // rythmo strip
    for (int i = 1; i<=6; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS); 	//Begining the cube's drawing

        glTexCoord3i(0, 0, 1);glVertex3i(0, this->height()-this->height()/4, -2);
        glTexCoord3i(1, 0, 1);glVertex3i(i * this->width()/6, this->height()-this->height()/4, -2);
        glTexCoord3i(1, 1, 1);glVertex3i(i * this->width()/6, this->height(), -2);
        glTexCoord3i(0, 1, 1);glVertex3i(0, this->height(), -2);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


    // Text texture
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS); 	//Begining the cube's drawing



    glTexCoord3i(0, 0, 1);glVertex3i(x - xmove, this->height()-this->height()/4, -1);
    glTexCoord3i(1, 0, 1);glVertex3i(x + w - xmove, this->height()-this->height()/4, -1);
    glTexCoord3i(1, 1, 1);glVertex3i(x + w - xmove, this->height(), -1);
    glTexCoord3i(0, 1, 1);glVertex3i(x - xmove, this->height(), -1);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    /*

    int w = 200;
    int h = 160;
    int space = 220; // space between quad

    for(int i=0;i<3;i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS); 	//Begining the cube's drawing

        glTexCoord2i(0, 0);glVertex2i(i * space, 0);
        glTexCoord2i(1, 0);glVertex2i(i * space + w, 0);
        glTexCoord2i(1, 1);glVertex2i(i * space + w, h);
        glTexCoord2i(0, 1);glVertex2i(i * space, h);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    */
}
