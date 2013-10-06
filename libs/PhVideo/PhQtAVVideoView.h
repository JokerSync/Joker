#ifndef PHFFMPEGVIDEOVIEW_H
#define PHFFMPEGVIDEOVIEW_H

#include <QWidget>
#include "PhVideoObject.h"

#include <QtAV.h>

class PhQtAVVideoView : public QWidget, public PhVideoObject
{
	Q_OBJECT
public:
	explicit PhQtAVVideoView(QWidget *parent = 0);

	bool open(QString fileName);
signals:

protected slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);
	void checkVideoPosition();

private:
	QtAV::AVPlayer _player;
    QtAV::VideoRenderer *mpRenderer;
};

#endif // PHFFMPEGVIDEOVIEW_H
