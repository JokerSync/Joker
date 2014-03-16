#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QString>
#include <QTimer>

#include "PhTools/PhClock.h"
#include "PhVideo/PhVideoView.h"
#include "PhCommonUI/PhMediaPanel.h"
#include "PhCommonUI/PhDocumentWindow.h"

#include "VideoSyncTestSettings.h"

namespace Ui {
class VideoSyncTestWindow;
}

class VideoSyncTestWindow : public PhDocumentWindow
{
	Q_OBJECT
public:
	/**
	 * @brief MainView constructor
	 */
	VideoSyncTestWindow(VideoSyncTestSettings *settings);
	~VideoSyncTestWindow();
	/**
	 * Open a video file.
	 * @param fileName Path of the video file to open.
	 * @return True if succeeds, false otherwise.
	 */
	bool openDocument(QString fileName);

protected:
	QMenu *recentDocumentMenu();
	QAction *fullScreenAction() {
		return NULL;
	}

private slots:

	void onOpenFile();
private:
	Ui::VideoSyncTestWindow *ui;
	PhVideoEngine _videoEngine;
	VideoSyncTestSettings *_settings;
};

#endif
