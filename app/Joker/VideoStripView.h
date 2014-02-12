/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

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
	QList<PhPeople*>* getSelectedPeoples();

protected:
	bool init();
	void paint();

private slots:
	void onVideoSync();
	void onDocChanged();

private:
	PhVideoEngine _videoEngine;
	PhGraphicStrip _strip;
	PhSonyController *_sony;
	QTime _lastVideoSyncElapsed;

	PhGraphicText _titleText;
	PhGraphicSolidRect _titleBackgroundRect;
	PhGraphicText _tcText;
	PhGraphicText _nextTCText;
	PhGraphicText _noVideoSyncError;
	PhGraphicText _currentPeopleName;

	QList<PhPeople*> _selectedPeoples;
};

#endif // VIDEOSTRIPVIEW_H
