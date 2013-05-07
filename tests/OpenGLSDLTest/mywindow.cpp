#include "myWindow.h"
#include "glu.h"
#include "SDL_ttf/SDL_ttf.h"


myWindow::myWindow(QWidget *parent)
    : myGLWidget( parent, "Premier affichage de texte avec OpenGL et Qt")
{
}


void myWindow::initializeGL()
{
    float ratio = this->width() / this->height();

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, this->width(), this->height() );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 100.0, ratio, 1.0, 5.0 );

}

void myWindow::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myWindow::paintGL()
{


    SDL_Color textColor={ 0, 0, 255 };
    TTF_Init();
    TTF_Font *font;

    font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 30);
    if (font == NULL){
        qDebug() << TTF_GetError();
        exit(1);
    }

#define TUTO (1)

#if TUTO
    GLuint texture_name;
    GLuint objet;

    SDL_Surface *texture = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );
    if (texture == NULL){
        qDebug() << SDL_GetError();
        exit(2);
    }

    // ---- Création d'un objet de texture. ------------------------------------------------
    glGenTextures(1, & texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    // Paramétrage de la texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Chargement du fichier.

    // Jonction entre OpenGL et SDL.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
    // ---- Fin de création de l'objet de texture. -----------------------------------------
    // ...
    // Création d'un objet par l'utilisation d'une liste d'affichage.
    objet = glGenLists(1);
    glNewList(objet, GL_COMPILE);
    // Avant d'appeler les fonction glVertex..., on indique quelle texture va être associée à l'objet.
    // On active l'application de texture...
    glEnable(GL_TEXTURE_2D);
    // ... et on indique quelle texture utiliser(celle qui a été créée précédemment).
    glBindTexture(GL_TEXTURE_2D, texture_name);
    // On peut enfin créer un objet(ici un simple carré).
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glNormal3f(1.0, 1.0, 0.0); glVertex3f(1.0, 1.0, -2.0);
    glTexCoord2f(0.0, 5.0); glNormal3f(1.0, 1.0, 0.0); glVertex3f(-1.0, 1.0, -2.0);
    glTexCoord2f(5.0, 5.0); glNormal3f(1.0, 1.0, 0.0); glVertex3f(-1.0, -1.0, -2.0);
    glTexCoord2f(5.0, 0.0); glNormal3f(1.0, 1.0, 0.0); glVertex3f(1.0, -1.0, -2.0);
    glEnd();
    // Fin de l'application de la texture.
    glDisable(GL_TEXTURE_2D);
    // Fin de la liste d'affichage.
    glEndList();

#else

    GLuint texture;			// This is a handle to our texture object
    SDL_Surface *surface;	// This surface will tell us the details of the image
    GLenum texture_format;
    GLint  nOfColors;
    surface = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );

    if ( surface ) {

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
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
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        exit (1);
    }

    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }


#endif

}

void myWindow::loadTexture(QString textureName)
{
    QImage qim_Texture;
    QImage qim_TempTexture;
    qim_TempTexture.load(textureName);
    qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}
