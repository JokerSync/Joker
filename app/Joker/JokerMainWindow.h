/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef JokerMainWindow_H
#define JokerMainWindow_H



#include <QVideoWidget>
#include <QMediaPlayer>
#include <QtGui/QMovie>
#include <QtWidgets/QWidget>

#include <QMainWindow>

#include "StripWindow.h"
#include "MainController.h"

#include "PhCommonUI/PhLeap.h"


class JokerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    JokerMainWindow(PhString file);
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void wheelEvent(QWheelEvent *wheel);


private slots:

    void openVideoFile();
    void play();

    /**
     * @brief openFile
     * call StripWindow::openFile() to open a PhStripDoc with the
     * user selected file
     */
    void openFile();

    void switchScrolling();
    /**
     * @brief changeFont
     * List all loaded PhFont and allow the user to set the PhFont
     * currently used
     */
    void changeFont();
    /**
     * @brief exportRythomAsPNG
     * export the next minute of the rythmo strip to a bundle of PNG
     * files
     */
    void exportRythomAsPNG();
    /**
     * @brief resizeEvent
     * handle the window resize
     */
    void resizeEvent(QResizeEvent *);

private:

    QMediaPlayer mediaPlayer;
    QAction *playButton;
    QAction *openVideoAct;



    MainController * _MController;

    /**
     * @brief toggleFullWindow
     * toogle from fullScreen to normal and vice-versa
     */
    void toggleFullWindow();
    /**
     * @brief createMenus
     * create the menu bar of the program
     */
    void createMenus();

    /**
     * @brief _strip
     * link to the StripWindow
     */
    StripWindow *_strip;


    /**
     * @brief fileMenu
     * QMenu for files related actions
     */
    QMenu *fileMenu;
    /**
     * @brief toolMenu
     * QMenu for tools related actions
     */
    QMenu *toolMenu;

    /**
     * @brief openAct
     */
    QAction *openAct;
    /**
     * @brief changeFontAct
     */
    QAction *changeFontAct;
    /**
     * @brief exportRythmoAct
     */
    QAction *exportRythmoAct;

    QAction *switchScroll;

    PhLeap leapListener;
    Controller _leapController;


};

#endif // JokerMainWindow_H
