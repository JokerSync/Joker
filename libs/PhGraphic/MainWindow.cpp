/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>

#include <QInputDialog>



MainWindow::MainWindow(PhString file)
{
    resize(640,360);
    createMenus();
    setWindowTitle(tr("Phonation"));
    _strip = new StripWindow(this, file);
    _strip->show();

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    
    openAct = new QAction(tr("&Open"), this);
    fileMenu->addAction(openAct);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    fileMenu->addSeparator();


    toolMenu = menuBar()->addMenu(tr("Tools"));

    exitAct = new QAction(tr("&Change font..."), this) ;
    toolMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(changeFont()));
    toolMenu->addSeparator();

}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");
    if (!fileName.isEmpty())
        _strip->openFile(fileName);
}

void MainWindow::changeFont()
{
    QStringList fonts;
    int i = 0;
    for (auto it : _strip->getFonts() ){
        fonts << (QString::number(i) + " : " + it->getFontName());
        i++;
    }
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Font Selection"),tr("fonts"), fonts, 0, false, &ok);

}

void MainWindow::keyPressEvent( QKeyEvent *keyEvent )
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Q:
        close();
        break;
    case Qt::Key_Space:
        toggleFullWindow();
        break;
    default:
        _strip->keyPressEvent(keyEvent);
    }
}

void MainWindow::toggleFullWindow()
{
    if(this->isFullScreen())
    {
        showNormal();
        _strip->toggleFS(false);
    }
    else
    {
        showFullScreen();
        _strip->toggleFS(true);
    }
    _strip->resizeGL(this->width(), this->height());
}

