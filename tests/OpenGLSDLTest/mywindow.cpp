#include "myWindow.h"
#include "glu.h"
#include "SDL_ttf/SDL_ttf.h"


myWindow::myWindow(QWidget *parent)
    : myGLWidget( parent, "Premier Polygone avec OpenGL et Qt")
{
}


void myWindow::initializeGL()
{
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
    GLuint texture_name;
    GLuint objet;
    SDL_Color textColor={ 0, 0, 255 };
    TTF_Init();
    TTF_Font *font;

    font = TTF_OpenFont("../../data/zoinks.ttf", 30);
    if (font == NULL){
        exit(1);
    }
    SDL_Surface *texture = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );
    if (texture == NULL){
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
    //texture = IMG_Load("../../data/look.png");
    qDebug() << texture ;

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
   SDL_GL_SwapBuffers( );


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
