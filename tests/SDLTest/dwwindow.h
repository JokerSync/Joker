#ifndef DWWINDOW_H
#define DWWINDOW_H
#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"


#include <QApplication>
#include <QWidget>
#include <QShowEvent>
#include <QTimer>
#include <QMainWindow>
#include <QDebug>

namespace Ui {
class DWwindow;
}

class DWwindow : public QMainWindow
{
    Q_OBJECT
    
public:
    //~DWwindow();

    DWwindow();
    virtual ~DWwindow();
    void setScreen(SDL_Surface *surface);
    SDL_Surface* getScreen();
    bool UpdateSurface();
    void keyPressEvent(QKeyEvent *keyEvent);
    void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination);
    SDL_Surface *screen;
    void scroll();

private:
    bool windowInitialized;


};
#endif // DWWINDOW_H
