#ifndef VIDEOSYNCTESTWINDOW_H
#define VIDEOSYNCTESTWINDOW_H


#include "VideoSyncTestSettings.h"

#include "../VideoTest/VideoTestWindow.h"

class VideoSyncTestWindow : public VideoTestWindow
{
	Q_OBJECT
public:
	/**
	 * @brief VideoSyncTestWindow constructor
	 */
	VideoSyncTestWindow(VideoSyncTestSettings *settings);
private:
	VideoSyncTestSettings *_settings;
};

#endif
