#include "TestSDLOpenGLWidget.h"
#include "glu.h"
//#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"
#include "SDL/SDL.h"


TestSDLOpenGLWidget::TestSDLOpenGLWidget(QWidget *parent)
    : PhGLWidget( parent, "Premier affichage de dessin avec OpenGL et Qt")
{
    x = 0.0;
}


void TestSDLOpenGLWidget::initializeGL()
{
    paintGL();
}

void TestSDLOpenGLWidget::resizeGL(int width, int height)
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

void TestSDLOpenGLWidget::paintGL()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Color textColor={ 0, 0, 255 };
    TTF_Init();
    TTF_Font *font;

    font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 30);
    if (font == NULL){
        qDebug() << TTF_GetError();
        exit(1);
    }

// 1st displaying method with texture
#if 0
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
#endif

    // 2nd displaying method with texture
#if 1
    GLuint texture;			// This is a handle to our texture object
    SDL_Surface *surface;	// This surface will tell us the details of the image
    GLenum texture_format;
    GLint  nOfColors;
    surface = IMG_Load("../../../../../data/box.png");

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    // 3rd displaying method with texture
#if 0
    GLuint texture_name;
    GLuint objet;
    SDL_Surface * texture;
    // ---- Création d'un objet de texture. ------------------------------------------------
    glGenTextures (1, & texture_name);
    glBindTexture (GL_TEXTURE_2D, texture_name);
    // Paramétrage de la texture.
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Chargement du fichier.
    texture = IMG_Load ("/brique.png" );
    // Jonction entre OpenGL et SDL.
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture-> pixels);
    // ---- Fin de création de l'objet de texture. -----------------------------------------
    // ...
    // Création d'un objet par l'utilisation d'une liste d'affichage.
    objet = glGenLists (1);
    glNewList (objet, GL_COMPILE);
    // Avant d'appeler les fonction glVertex..., on indique quelle texture va être associée à l'objet.
    // On active l'application de texture...
    glEnable (GL_TEXTURE_2D);
    // ... et on indique quelle texture utiliser (celle qui a été créée précédemment).
    glBindTexture (GL_TEXTURE_2D, texture_name);
    // On peut enfin créer un objet (ici un simple carré).
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0); glNormal3f (1.0, 1.0, 0.0); glVertex3f (1.0, 1.0, -2.0);
    glTexCoord2f (0.0, 5.0); glNormal3f (1.0, 1.0, 0.0); glVertex3f (-1.0, 1.0, -2.0);
    glTexCoord2f (5.0, 5.0); glNormal3f (1.0, 1.0, 0.0); glVertex3f (-1.0, -1.0, -2.0);
    glTexCoord2f (5.0, 0.0); glNormal3f (1.0, 1.0, 0.0); glVertex3f (1.0, -1.0, -2.0);
    glEnd ();
    // Fin de l'application de la texture.
    glDisable (GL_TEXTURE_2D);
    // Fin de la liste d'affichage.
    glEndList ();
#endif

    // displaying method with shape
#if 0

    x += 0.03;
    if (x > 3.8){
        x=-4;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(x, 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();



#endif



}
