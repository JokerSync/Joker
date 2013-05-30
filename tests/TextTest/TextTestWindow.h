#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"


#include "PhStrip/PhStripDoc.h"


class TextTestWindow : public PhGraphicView
{
    Q_OBJECT
public:
    explicit TextTestWindow(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void openFile(PhString filename);
    int getStripWidth();
    void setStripWidth(int w);

    int getStripHeight();
    void setStripHeight(int w);

private :
    GLuint textures[3];			// This is a handle to a tab of textures
    //GLuint texture;
    float xmove;
    float y;
    int _stripWidth;
    int _stripHeight;

    QList<PhFont *> _fonts;
    QList<PhGraphicText *> _texts;
    QList<PhGraphicImage *> _imgs;


};

#endif // MYWINDOW_H
