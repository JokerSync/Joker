#ifndef VIDEOSTRIPVIEW_H
#define VIDEOSTRIPVIEW_H

#include <QSettings>

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhSonyController.h"

class VideoStripView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit VideoStripView(QWidget *parent = 0);

	void setSettings(QSettings * settings);
	PhVideoEngine * videoEngine() { return &_videoEngine;}
	PhGraphicStrip * strip() { return &_strip;}
	void setSony(PhSonyController * sony);
protected:
	bool init();
	void paint();

private:
	PhVideoEngine _videoEngine;
	PhGraphicStrip _strip;
	QSettings *_settings;
	PhSonyController *_sony;
};

#endif // VIDEOSTRIPVIEW_H
