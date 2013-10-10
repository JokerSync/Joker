#ifndef PHFFMPEGWIDGETVIDEOVIEW_H
#define PHFFMPEGWIDGETVIDEOVIEW_H

#include <QWidget>

#include "PhFFMpegVideoView.h"

class PhFFMpegWidgetVideoView : public QWidget, public PhFFMpegVideoView
{
	Q_OBJECT
public:
	explicit PhFFMpegWidgetVideoView(QWidget *parent = 0);

protected slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);
	void checkVideoPosition();

protected:
	bool drawFrame(AVFrame *avFrame);
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *event);
private:
	QImage * _image;
	uint8_t *_rgb;
};

#endif // PHFFMPEGWIDGETVIDEOVIEW_H
