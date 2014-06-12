/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOTEST_H
#define VIDEOTEST_H

#include <QObject>

#include "PhGraphic/PhGraphicView.h"

#include "PhVideo/PhVideoEngine.h"
#include "PhGraphic/PhGraphicView.h"

#include "VideoTestSettings.h"


class VideoTest : public QObject
{
	Q_OBJECT
public:
	VideoTest();

private slots:

	void openMovieTest();
	void goToTest01();
	void goToTest02();
	void playTest();
	void deinterlaceTest();
	void goToTest03();
	void testBuffer();
	void testFirstFrame();
	void testFPS();
private:
	VideoTestSettings _settings;
	PhVideoEngine _videoEngine;
	PhGraphicView _view;

#warning /// @todo move to a generic graphicTest implementation
	void saveBuffer(PhGraphicView *view);
#warning /// @todo move to a generic graphicTest implementation
	void findMatch(QImage source);
};

#endif // VIDEOTEST_H
