#include "TestSDLOpenGLWidget.h"

#include "glu.h"

#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"
#include "SDL/SDL.h"


TestSDLOpenGLWidget::TestSDLOpenGLWidget(QWidget *parent)
    : PhGLWidget( parent, "Premier affichage de dessin avec OpenGL et Qt")
{
    x = 5;
    y = 0;
}


void TestSDLOpenGLWidget::initializeGL()
{
    int method = 3;

    glClearColor(.5,.5,.5,0); 	//Background color RGB
    glEnable(GL_DEPTH_TEST); 	//Activate the depth test
    glEnable(GL_TEXTURE_2D); 	//Activate the texturing

    switch(method)
    {
    case 0: // no texture is displayed
        break;

    case 1: // initialize with a simple buffer
    {
        GLubyte Texture[16] =
        {
            0,0,0,0, 0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF, 0,0,0,0
        };
        //Image (2x2)
        GLuint Nom;

        glGenTextures(1,&Nom); 	//Generate a texture @ 1
        glBindTexture(GL_TEXTURE_2D,Nom); 	//Select this texture
        glTexImage2D (
                    GL_TEXTURE_2D,      //Type : texture 2D
                    0,                  //Mipmap : none
                    4,                  //Couleurs : 4
                    2,                  //height : 2
                    2,                  //width : 2
                    0,                  //border width : 0
                    GL_RGBA,            //Format : RGBA
                    GL_UNSIGNED_BYTE, 	//Colors type
                    Texture             //Picture's address
                    );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        break;
    }
    case 2:
    case 3:
        // Using SDL library

        if (SDL_Init(SDL_INIT_VIDEO) == 0) {
            SDL_Surface *surface = NULL;	// This surface will tell us the details of the image

            //Create a surface
            switch (method) {
            case 2: // load a texture with SDL_Image:
            {
                surface = IMG_Load("../../../../../data/box.png");
            }
                break;
            case 3: // load a texture with SDL_TTF:

                SDL_Color textColor={ 255, 255, 0, 1 };
                if (TTF_Init() == 0){;
                    TTF_Font *font;
                    font = TTF_OpenFont("../../../../../data/Arial.ttf", 1);
                    if (font != NULL){
                        surface = TTF_RenderText_Blended(font, "L", textColor );
                    }
                    else
                        qDebug() << "Error (Font) : " << TTF_GetError();
                }
                else
                    qDebug() << "Error (Font) : " << TTF_GetError();

                break;
            }
            GLint  nbOfColors;
            GLenum texture_format = 0;

            if (surface != NULL){
                qDebug() << "info sur la surface";
                qDebug() << surface->w;
                qDebug() << surface->h;
                qDebug() << surface->format->Rmask;

                // get the number of channels in the SDL surface
                nbOfColors = surface->format->BytesPerPixel;

                switch (nbOfColors) {
                case 3:     // no alpha channel
                    if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                    else
                        texture_format = GL_BGR;
                    break;
                case 4:     // contains an alpha channel
                    if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                    else
                        texture_format = GL_BGRA;
                    break;
                default:
                    qDebug() << "Warning: the image is not truecolor...";
                    break;
                }

                glEnable( GL_TEXTURE_2D );
                // Have OpenGL generate a texture object handle for us
                glGenTextures( 1, &texture );

                // Bind the texture object
                glBindTexture( GL_TEXTURE_2D, texture );

                // Set the texture's stretching properties
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

                // Edit the texture object's image data using the information SDL_Surface gives us
                glTexImage2D( GL_TEXTURE_2D, 0, nbOfColors, surface->w, surface->h, 0,
                              texture_format, GL_UNSIGNED_BYTE, surface->pixels );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            }
            else{
                qDebug() << "Error (SDL) : " << SDL_GetError();
            }
        }
        else
            qDebug() << "Error (SDL) : " << SDL_GetError();

        break;

    }
}

void TestSDLOpenGLWidget::paintGL()
{
    if (x < -5)
        x = 5;
    if (x > 5)
        x = -5;
    if (y < -5)
        y = 5;
    if (y > 5)
        y = -5;

    if (move){
        x += xdelta;
        y += ydelta;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(x,y,-5);

    //  glRotatef(x, 0, 1, 1);
    //  gluLookAt(10,0,0,   //eyeXYZ
    //            0,0,0,    //centerXYZ
    //            0,1,0);   //upXYZ

    glBegin(GL_QUADS); 	//Begining the cube's drawing

    //int k = 50;
    int i = 0;
    //for (i = -k; i < k; i++)
//    {
//        glTexCoord2i(0,0);glVertex3i(-1 +i,-1,-1);
//        glTexCoord2i(1,0);glVertex3i(+1 +i,-1,-1);
//        glTexCoord2i(1,1);glVertex3i(+1 +i,+1,-1);
//        glTexCoord2i(0,1);glVertex3i(-1 +i,+1,-1);
//    }

    glTexCoord2i(0,0);glVertex3i(-1 +i,-1,-1);
    glTexCoord2i(1,0);glVertex3i(+1 +i + surface->w,-1,-1);
    glTexCoord2i(1,1);glVertex3i(+1 +i + surface->w,+1 + surface->h,-1);
    glTexCoord2i(0,1);glVertex3i(-1 +i,+1 + surface->h ,-1);


    //    //1st face done

    //    glTexCoord2i(0,0);glVertex3i(-1,-1,+1);
    //    glTexCoord2i(1,0);glVertex3i(+1,-1,+1);
    //    glTexCoord2i(1,1);glVertex3i(+1,+1,+1);
    //    glTexCoord2i(0,1);glVertex3i(-1,+1,+1);

    //    //2nd face done

    //    glTexCoord2i(0,0);glVertex3i(+1,-1,-1);
    //    glTexCoord2i(1,0);glVertex3i(+1,-1,+1);
    //    glTexCoord2i(1,1);glVertex3i(+1,+1,+1);
    //    glTexCoord2i(0,1);glVertex3i(+1,+1,-1);

    //    //3rd face done

    //    glTexCoord2i(0,0);glVertex3i(-1,-1,-1);
    //    glTexCoord2i(1,0);glVertex3i(-1,-1,+1);
    //    glTexCoord2i(1,1);glVertex3i(-1,+1,+1);
    //    glTexCoord2i(0,1);glVertex3i(-1,+1,-1);

    //    //4th face done

    //    glTexCoord2i(1,0);glVertex3i(-1,+1,-1);
    //    glTexCoord2i(1,1);glVertex3i(+1,+1,-1);
    //    glTexCoord2i(0,1);glVertex3i(+1,+1,+1);
    //    glTexCoord2i(0,0);glVertex3i(-1,+1,+1);

    //    //5th face done

    //    glTexCoord2i(1,0);glVertex3i(-1,-1,+1);
    //    glTexCoord2i(1,1);glVertex3i(+1,-1,+1);
    //    glTexCoord2i(0,1);glVertex3i(+1,-1,-1);
    //    glTexCoord2i(0,0);glVertex3i(-1,-1,-1);

    //6th face done
    glEnd();

}
