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
	int _iXdeffarance;
	int _iYdeffarance;
	bool _mousePressed;

	void mousePressEvent( QMouseEvent * event);
	void mouseReleaseEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
};

#endif // PHFLOATINGMEDIAPANEL_H
