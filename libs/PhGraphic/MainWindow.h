/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

#include "StripWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PhString file);
    virtual void keyPressEvent( QKeyEvent *keyEvent );


private slots:
    void openFile();
    void changeFont();

private:
    void toggleFullWindow();
    void createMenus();

    StripWindow *_strip;


    QMenu *fileMenu;
    QMenu *toolMenu;
    QAction *openAct;
    QAction *exitAct;
    //QAction *changeFont;
   };

#endif // MAINWINDOW_H
