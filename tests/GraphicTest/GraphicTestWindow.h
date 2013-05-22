#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QImage>
#include "PhGraphicView.h"
 #include <QFont>
#include <QLabel>
#include <QPainter>

class GraphicTestWindow : public PhGraphicView
{
    Q_OBJECT
public:
    explicit GraphicTestWindow(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void loadTexture(QString textureName);

private :
    QLabel test;
    GLuint texture[1];
    float f_x;
    bool shouldRotate;
};

#endif // MYWINDOW_H
