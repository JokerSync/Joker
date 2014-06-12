/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef MEDIAPANELTEST_H
#define MEDIAPANELTEST_H

#include <QObject>

class MediaPanelTest : public QObject
{
	Q_OBJECT
public:
	explicit MediaPanelTest(QObject *parent = 0);

signals:

public slots:

private slots:
	void setFirstFrameTest();
	void setMediaLengthTest();
};

#endif // MEDIAPANELTEST_H
