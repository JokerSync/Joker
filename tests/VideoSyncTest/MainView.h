#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

#include "PhTools/PhClock.h"
#include "PhVideo/PhVideoView.h"
#include "PhCommonUI/PhMediaPanel.h"

using namespace std;

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
QT_END_NAMESPACE

namespace Ui {
	class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief MainView constructor
     */
    MainView();
	~MainView();
    /**
     * Open a video file.
     * @param fileName Path of the video file to open.
     * @return True if succeeds, false otherwise.
     */
    bool openFile(QString fileName);

private slots:

	void onOpenFile();
private:
	Ui::MainView *ui;
	PhVideoEngine _videoEngine;
};

#endif
