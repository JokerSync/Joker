#ifndef VIDEOSTRIPVIEW_H
#define VIDEOSTRIPVIEW_H

#include <QSettings>
#include <QTime>

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
	void setPeople(PhPeople* people);

protected:
	bool init();
	void paint();

private slots:
	void onVideoSync();
private:
	PhVideoEngine _videoEngine;
	PhGraphicStrip _strip;
	QSettings *_settings;
	PhSonyController *_sony;
	QTime _lastVideoSyncElapsed;

	PhGraphicText _titleText;
	PhGraphicSolidRect _titleBackgroundRect;
	PhGraphicText _tcText;
	PhGraphicText _nextTCText;
	PhGraphicText _noVideoSyncError;
	PhGraphicText _currentPeopleName;

	PhPeople* _currentPeople;
};

#endif // VIDEOSTRIPVIEW_H
