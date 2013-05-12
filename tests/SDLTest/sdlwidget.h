#ifndef SDLWIDGET_H
#define SDLWIDGET_H

#include <QWidget>
#include <QShowEvent>
#include <QTimer>

#include "SDL/SDL.h"
//#include "SDL_ttf/SDL_ttf.h"
//#include "SDL_image/SDL_image.h"


class SDLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SDLWidget(int width, int height, QWidget *parent = 0);
    virtual void showEvent(QShowEvent *);
    virtual ~SDLWidget();

signals:

public slots:
    virtual void onRefresh();

protected:
    bool init();
    void dispose();

private:
    bool initOk;
    SDL_Surface *screen;
    QTimer *t_Timer;

};

#endif // SDLWIDGET_H
