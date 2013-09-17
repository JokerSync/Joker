#ifndef PHQTVIDEOVIEW_H
#define PHQTVIDEOVIEW_H

#include <QVideoWidget>
#include <QMediaPlayer>

#include "PhVideoObject.h"

/**
 * The PhQTVideoView class display video content.
 */
class PhQTVideoView : public QVideoWidget, public PhVideoObject
{
    Q_OBJECT
public:
	/**
	 * @brief PhQTVideoView constructor
	 * @param parent widget for the view
	 */
	explicit PhQTVideoView(QObject *parent = 0);
	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);

	void setClock(PhClock *clock);
signals:

public slots:
	void onRateChanged(PhRate rate);
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onTCTypeChanged(PhTimeCodeType tcType);
	void checkVideoPosition();
private:
	QMediaPlayer _player;
};

#endif // PHQTVIDEOVIEW_H
