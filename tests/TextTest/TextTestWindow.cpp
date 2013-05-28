#include "glu.h"

#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"
#include "SDL/SDL.h"

#include "PhTools/memorytool.h"


#include "TextTestWindow.h"

TextTestWindow::TextTestWindow(QWidget *parent)
    : PhGraphicView( parent, "Premier affichage de dessin avec OpenGL et Qt")
{
    _stripWidth = 0;
    _stripHeight = 0;
    _doc = NULL;
    xmove = this->width() * 1.5;
    resize(1000,600);
}

void TextTestWindow::openFile(PhString filename){
    _doc = new PhStripDoc(filename);
}

int TextTestWindow::getStripWidth()
{
    return _stripWidth;
}

void TextTestWindow::setStripWidth(int w)
{
    _stripWidth = w;
}

void TextTestWindow::initializeGL()
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
        SDL_Surface *surface = IMG_Load("../../../../../../texture_temp.jpg");
        this->setStripWidth(surface->w);
        this->setStripHeight(surface->h);
        qDebug() << this->_doc->getTexts().last()->getTrack();
        // Create a texture from this surface
        if(surface != NULL)
            textures[1] = createTextureFromSurface(surface);

        textures[2] = createSurfaceFromText(_doc->getTexts().last()->getPeople().getName() + " : " + _doc->getTexts().last()->getContent());
    }
}

void TextTestWindow::paintGL()
{
    // in order to start from the window's right border
    // could have been 0 to start from left border
    int x = this->width();
    // refers to window size
    int w = this->width();
    int h = this->height();
    if (xmove > x + w)
        xmove = 0;

    if (move){
        xmove += xdelta;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // rythmo strip
    float tu = w / this->getStripWidth();
    float tv = h / this->getStripHeight();
            //qDebug() << n << " " << w;

    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS); 	//Begining the cube's drawing

    glTexCoord3i(0, 0, 1);glVertex3i(0, 0, -2);
    glTexCoord3i(tu, 0, 1);glVertex3i(w, 0, -2);
    glTexCoord3i(tu, tv, 1);glVertex3i(w, h, -2);
    glTexCoord3i(0, tv, 1);glVertex3i(0, h, -2);

    glEnd();
    glDisable(GL_TEXTURE_2D);


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

}


void TextTestWindow::setStripHeight(int h)
{
    _stripHeight = h;
}

int TextTestWindow::getStripHeight()
{
    return _stripHeight;
}
