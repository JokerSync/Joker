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

    int method = 2;

    glClearColor(.5,.5,.5,0); 	//Change la couleur du fond
    glEnable(GL_DEPTH_TEST); 	//Active le depth test
    glEnable(GL_TEXTURE_2D); 	//Active le texturing

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

        glGenTextures(1,&Nom); 	//Génère un n° de texture
        glBindTexture(GL_TEXTURE_2D,Nom); 	//Sélectionne ce n°
        glTexImage2D (
                    GL_TEXTURE_2D, 	//Type : texture 2D
                    0, 	//Mipmap : aucun
                    4, 	//Couleurs : 4
                    2, 	//Largeur : 2
                    2, 	//Hauteur : 2
                    0, 	//Largeur du bord : 0
                    GL_RGBA, 	//Format : RGBA
                    GL_UNSIGNED_BYTE, 	//Type des couleurs
                    Texture 	//Addresse de l'image
                    );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        break;
    }
    case 2:
    case 3:
    {

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_Surface *surface;	// This surface will tell us the details of the image


        switch (method) {
            case 2: // load a texture with SDL_Image:
            {
                surface = IMG_Load("../../../../../data/box.png");

            }
                break;
            case 3: // load a texture with SDL_TTF:
            {
                SDL_Color textColor={ 255, 255, 255 };
                TTF_Init();
                TTF_Font *font;

                font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 30);
                if (font == NULL){
                    qDebug() << TTF_GetError();
                    exit(1);
                }

                surface = TTF_RenderUTF8_Blended( font, "Les chaussettes de l'archi duchesse sont-elles sèches?", textColor );

                break;
            }
        }

        GLint  nOfColors;
        GLenum texture_format;


        if (surface != NULL){
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

        }else{
            qDebug() << SDL_GetError();

        }

        break;
    }
    }
}

void TestSDLOpenGLWidget::paintGL()
{

    // 1st displaying method with texture
#if 0
    GLuint texture_name;
    GLuint objet;


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


    if (x > 360){
        x = 0;
    }
    x += 1.6;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	//Efface le framebuffer et le depthbuffer
    glMatrixMode(GL_MODELVIEW); 	//Un petit gluLookAt()...
    glLoadIdentity();

    glTranslatef(0.0f, 3.0f, -10);

    glRotatef(x, 0, 1, 1);
    gluLookAt(3,2,3,0,0,0,0,1,0);
    glBegin(GL_QUADS); 	//Et c'est parti pour le cube !

    glTexCoord2i(0,0);glVertex3i(-1,-1,-1);
    glTexCoord2i(1,0);glVertex3i(+1,-1,-1);
    glTexCoord2i(1,1);glVertex3i(+1,+1,-1);
    glTexCoord2i(0,1);glVertex3i(-1,+1,-1);

    //1 face

    glTexCoord2i(0,0);glVertex3i(-1,-1,+1);
    glTexCoord2i(1,0);glVertex3i(+1,-1,+1);
    glTexCoord2i(1,1);glVertex3i(+1,+1,+1);
    glTexCoord2i(0,1);glVertex3i(-1,+1,+1);

    //2 faces

    glTexCoord2i(0,0);glVertex3i(+1,-1,-1);
    glTexCoord2i(1,0);glVertex3i(+1,-1,+1);
    glTexCoord2i(1,1);glVertex3i(+1,+1,+1);
    glTexCoord2i(0,1);glVertex3i(+1,+1,-1);

    //3 faces

    glTexCoord2i(0,0);glVertex3i(-1,-1,-1);
    glTexCoord2i(1,0);glVertex3i(-1,-1,+1);
    glTexCoord2i(1,1);glVertex3i(-1,+1,+1);
    glTexCoord2i(0,1);glVertex3i(-1,+1,-1);

    //4 faces

    glTexCoord2i(1,0);glVertex3i(-1,+1,-1);
    glTexCoord2i(1,1);glVertex3i(+1,+1,-1);
    glTexCoord2i(0,1);glVertex3i(+1,+1,+1);
    glTexCoord2i(0,0);glVertex3i(-1,+1,+1);

    //5 faces

    glTexCoord2i(1,0);glVertex3i(-1,-1,+1);
    glTexCoord2i(1,1);glVertex3i(+1,-1,+1);
    glTexCoord2i(0,1);glVertex3i(+1,-1,-1);
    glTexCoord2i(0,0);glVertex3i(-1,-1,-1);

    //6 faces
    glEnd();

}
