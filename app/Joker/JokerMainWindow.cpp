/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QAction>
#include <QFileDialog>
#include <QMenuBar>

#include <QInputDialog>

#include "JokerMainWindow.h"


#if LEAP
 using namespace Leap;
#endif    //LEAP


JokerMainWindow::JokerMainWindow(PhString file) : mediaPlayer(0, QMediaPlayer::VideoSurface)

{

    QVideoWidget *videoWidget = new QVideoWidget;


#if LEAP
    _leapController.addListener(leapListener);
#endif   //LEAP
    _MController = new MainController();
    if(_MController->openDoc(file))
        _MController->setLastFile(file);
    resize(1280,600);
    setWindowTitle(tr("Striptest"));
    _strip = new StripWindow(this);
    createMenus();
    _strip->setController(_MController);
    _strip->setNaturalScroll(_MController->getNaturalScrollPref());
    _strip->connectSlots();

    videoWidget->resize(600,400);
    videoWidget->move((this->width() - videoWidget->width())/2, 0);



    //QBoxLayout *layout = new QVBoxLayout;
    this->layout()->addWidget(_strip);
    this->layout()->addWidget(videoWidget);

    setLayout(this->layout());

    mediaPlayer.setVideoOutput(videoWidget);
    //connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    //connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    //connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    //connect(&mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));


#if LEAP
    qDebug() << leapListener.objectName();
    connect(&leapListener, SIGNAL(setPosition(int move)), _MController, SLOT(onPositionChanged2(int move)));
    connect(&leapListener, SIGNAL(setRate(float rate)), _MController, SLOT(onRateChanged2(float rate)));
#endif   //LEAP
}

void JokerMainWindow::createMenus()
{
    // The tr("&string") at the begining of a string is for translation tools

    // Add a menu
    fileMenu = menuBar()->addMenu(tr("&File"));

    // Create a new Open action
    openAct = new QAction(tr("&Open"), this);
    // Set a shortcut to execute this action
    openAct->setShortcut(QKeySequence::Open);
    // Add the action to the file menu
    fileMenu->addAction(openAct);
    // Connect the action to a function
    connect(openAct, SIGNAL(triggered()), _strip, SLOT(stopScroll()));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));


    openVideoAct = new QAction(tr("Openvideo"), this);
    connect(openVideoAct, SIGNAL(triggered()), this, SLOT(openVideoFile()));
    openVideoAct->setShortcut(QKeySequence::AddTab);
    fileMenu->addAction(openVideoAct);

    playButton = new QAction(tr("Play"), this);
    playButton->setShortcut(QKeySequence::Save);
    connect(playButton, SIGNAL(triggered()), this, SLOT(play()));
    fileMenu->addAction(playButton);


    // Add a menu
    toolMenu = menuBar()->addMenu(tr("&Tools"));

    changeFontAct = new QAction(tr("&Change font..."), this) ;
    toolMenu->addAction(changeFontAct);
    connect(changeFontAct, SIGNAL(triggered()), this, SLOT(changeFont()));

    //Separate 2 parts in the same menu list
    toolMenu->addSeparator();

    exportRythmoAct = new QAction(tr("&Export next minute"), this) ;
    toolMenu->addAction(exportRythmoAct);
    exportRythmoAct->setShortcut(QKeySequence::Print);
    connect(exportRythmoAct, SIGNAL(triggered()), this, SLOT(exportRythomAsPNG()));

    //Separate 2 parts in the same menu list
    toolMenu->addSeparator();

    switchScroll = new QAction(tr("&Toggle scroll"), this) ;
    toolMenu->addAction(switchScroll);
    connect(switchScroll, SIGNAL(triggered()), this, SLOT(switchScrolling()));

}

void JokerMainWindow::openFile()
{
    PhString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");
    if(!fileName.isNull())
        _MController->openDoc(fileName);
    //_strip->openFile("");
}

void JokerMainWindow::switchScrolling()
{
    _strip->toggleNaturalScrolling();
}

void JokerMainWindow::changeFont()
{

    // This is a routine witch load Apple system TTF fonts

    // Set the location
    PhString sourceFolder = "/Library/Fonts/";
    QDir sourceDir(sourceFolder);
    // List all files
    QStringList files = sourceDir.entryList(QDir::Files);
    QStringList fonts;
    // browse files and select only TTF ones
    for(int i = 0; i< files.count(); i++)
    {
        if(files[i].split(".").at(1) == "ttf")
        {
            fonts.push_back(files[i].split(".").at(0));
        }
    }

    // i & j are used to display the current font in the QInputDialog
    int i = 0;
    int j = 0;
    PhString oldFont = _strip->getCurrentFont()->getFontName();
    foreach (QString it, fonts ){
        if (it.contains(oldFont))
            j = i;
        i++;
    }
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Font Selection"),tr("fonts loaded"), fonts, j, false, &ok);
    if (true){
        PhString file = sourceFolder + item + ".ttf";
        _strip->setCurrentFont(file);
    }

}

void JokerMainWindow::exportRythomAsPNG()
{
    _strip->getContext()->exportToPng();
    QMessageBox::about(this, "Info",
                       tr("Export ended well! \n") + tr("file(s) saved here : ") + QDir::homePath() + "/Phonations/") ;
}

void JokerMainWindow::resizeEvent(QResizeEvent *)
{
    // Call the resize of the OpenGL context
    _strip->resizeGL(this->width(), this->height());
}

void JokerMainWindow::keyPressEvent( QKeyEvent *keyEvent )
{

    switch(keyEvent->key())
    {
    case Qt::Key_Space:
        play();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F10:
        toggleFullWindow();
        break;
    case Qt::Key_Right:
        _strip->setScroll(false);
        _strip->setXmove(this->width());
        break;
    case Qt::Key_Left:
        _strip->setScroll(false);
        _strip->setXmove(-this->width());
        break;
    default:
        _strip->keyPressEvent(keyEvent);
    }
}

void JokerMainWindow::wheelEvent(QWheelEvent *wheel)
{
    QPoint numPixels = wheel->pixelDelta();
    _strip->setXmove(numPixels.x());
    _strip->setXmove(-numPixels.y());
}



void JokerMainWindow::toggleFullWindow()
{
    if(this->isFullScreen())
        showNormal();
    else
        showFullScreen();
}


void JokerMainWindow::openVideoFile()
{
    qDebug() << "OpenVideoFile";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!fileName.isEmpty()) {
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
    }

    _strip->show();
}

void JokerMainWindow::play()
{
    switch(mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        _strip->setScroll(false);
        mediaPlayer.pause();
        break;
    default:
        _strip->setScroll(true);
        mediaPlayer.play();
        break;
    }
}
