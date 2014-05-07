/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOTEST_H
#define VIDEOTEST_H

#include <QObject>

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"

class VideoTest : public PhGraphicView
{
	Q_OBJECT
public:
	explicit VideoTest(QWidget *parent = 0);

private slots:

	void openMovieTest();
	void goToTest();
private:
	void paint();
	PhVideoEngine _videoEngine;
};

#endif // VIDEOTEST_H
