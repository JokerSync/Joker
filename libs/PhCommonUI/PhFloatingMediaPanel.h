/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHFLOATINGMEDIAPANEL_H
#define PHFLOATINGMEDIAPANEL_H

#include <QDialog>

#include "PhMediaPanel.h"

class PhFloatingMediaPanel : public PhMediaPanel
{
	Q_OBJECT

public:
	explicit PhFloatingMediaPanel(QWidget *parent = 0);

	bool isMousePressed();

private:
	// For moving with mouse
	bool _mousePressed;
	QPoint _mousePressedLocation;

	void mousePressEvent( QMouseEvent * event);
	void mouseReleaseEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
};

#endif // PHFLOATINGMEDIAPANEL_H
