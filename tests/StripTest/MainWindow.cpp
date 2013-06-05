/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "MainWindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>

#include <QInputDialog>



MainWindow::MainWindow(PhString file)
{
    resize(1280,360);
    createMenus();
    setWindowTitle(tr("Striptest"));
    _strip = new StripWindow(this, file);
    _strip->show();

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    fileMenu->addSeparator();


    toolMenu = menuBar()->addMenu(tr("Tools"));

    changeFontAct = new QAction(tr("&Change font..."), this) ;
    toolMenu->addAction(changeFontAct);
    connect(changeFontAct, SIGNAL(triggered()), this, SLOT(changeFont()));
    toolMenu->addSeparator();

    exportRythmoAct = new QAction(tr("&Export next minute"), this) ;
    toolMenu->addAction(exportRythmoAct);
    exportRythmoAct->setShortcut(QKeySequence::Print);
    connect(exportRythmoAct, SIGNAL(triggered()), this, SLOT(exportRythomAsPNG()));
    toolMenu->addSeparator();


}

void MainWindow::openFile()
{
    _strip->setScroll(false);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");
    if (!fileName.isEmpty())
        _strip->openFile(fileName);
}

void MainWindow::changeFont()
{
    QStringList fonts;
    int i = 0;
    int j = 0;
    for (auto it : _strip->getFonts() ){
        fonts << it->getFontName();
        if (it->getFontName() == _strip->getCurrentFont()->getFontName())
            j = i;
        i++;
    }
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Font Selection"),tr("fonts loaded"), fonts, j, false, &ok);
    for (auto it : _strip->getFonts() ){
        if (it->getFontName() == item)
            _strip->setCurrentFont(it);
    }
    _strip->initializeGL();

}

void MainWindow::exportRythomAsPNG()
{
    //_strip->setXmove(0);
    // As 1920px is 4 sec, 1 min is 28800 px
    int nbFrames = 28800 / this->width() + 1;
    for(int i = 0; i < nbFrames ; i++){
        _strip->getContext()->saveToPNG(QString::number(i));
        _strip->setXmove(this->width());
        _strip->paintGL();
    }
    QMessageBox::about(this, "Info",
                       tr("Export ended well! \n") + tr("file(s) saved here : ") + QDir::homePath() + "/") ;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    _strip->resizeGL(this->width(), this->height());
}

void MainWindow::keyPressEvent( QKeyEvent *keyEvent )
{

    switch(keyEvent->key())
    {
    case Qt::Key_Space:
        _strip->changeScroll();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Q:
        close();
        break;
    case Qt::Key_F10:
        toggleFullWindow();
        break;
    case Qt::Key_Left:
        _strip->setScroll(false);
        _strip->setXmove(this->width());
        break;
    case Qt::Key_Right:
        _strip->setScroll(false);
        _strip->setXmove(-this->width());
        break;
    default:
        _strip->keyPressEvent(keyEvent);
    }
}

void MainWindow::toggleFullWindow()
{
    if(this->isFullScreen())
        showNormal();
    else
        showFullScreen();
}

