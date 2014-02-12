/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef MainView_H
#define MainView_H


#include <QMainWindow>

#include "PhGraphicStrip/PhGraphicStripView.h"
//#include "MainController.h"
#include "PhGraphicStrip/PhGraphicStripController.h"

//#if LEAP
//#include "PhCommonUI/PhLeap.h"
//#endif

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    MainView();
   // virtual void keyPressEvent( QKeyEvent *keyEvent );
	//virtual void wheelEvent(QWheelEvent *wheel);
    /**
     * @brief openFile
     * call StripWindow::openFile() to open a PhStripDoc with the
     * user selected file
     */
	void openFile(QString fileName);

//	PhGraphicStripController* getController(){return _MController;};

//private slots:


	//void switchScrolling();
//    /**
//     * @brief changeFont
//     * List all loaded PhFont and allow the user to set the PhFont
//     * currently used
//     */
//    void changeFont();
//    /**
//     * @brief exportRythomAsPNG
//     * export the next minute of the rythmo strip to a bundle of PNG
//     * files
//     */
	//void exportRythomAsPNG();
//    /**
//     * @brief resizeEvent
//     * handle the window resize
//     */
//    void resizeEvent(QResizeEvent *);


private:

	//MainController * _MController;
	//PhGraphicStripView* _MController;

//    /**
//     * @brief toggleFullWindow
//     * toogle from fullScreen to normal and vice-versa
//     */
//    void toggleFullWindow();
//    /**
//     * @brief createMenus
//     * create the menu bar of the program
//     */
//    void createMenus();

    /**
     * @brief _strip
     * link to the StripWindow
     */
    PhGraphicStripView *_stripView;


//    /**
//     * @brief fileMenu
//     * QMenu for files related actions
//     */
//    QMenu *fileMenu;
//    /**
//     * @brief toolMenu
//     * QMenu for tools related actions
//     */
//    QMenu *toolMenu;

//    /**
//     * @brief openAct
//     */
//    QAction *openAct;
//    /**
//     * @brief changeFontAct
//     */
//    QAction *changeFontAct;
//    /**
//     * @brief exportRythmoAct
//     */
//    QAction *exportRythmoAct;

//    QAction *switchScroll;

//#if LEAP
//    PhLeap leapListener;
//    Controller _leapController;
//#endif
};

#endif // MainView_H
