/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#ifndef PHFLOATINGMEDIAPANEL_H
#define PHFLOATINGMEDIAPANEL_H

#include "PhMediaPanel.h"

/**
 * @brief A floating version of a PhMediaPanel
 *
 * It provides a nice media panel which can control the desired slave (any player).
 *
 */
class PhFloatingMediaPanel : public PhMediaPanel
{
	Q_OBJECT

public:
	/**
	 * @brief PhFloatingMediaPanel constructor
	 * @param parent The parent object
	 */
	explicit PhFloatingMediaPanel(QWidget *parent = 0);
	/**
	 * @brief Check if mouse button is pressed on PhFloatingMediaPanel
	 *
	 * Allow any PhFloatingMediaPanel user to know if the mouse is currently pressed over it.
	 *
	 * @return True if the mouse button is pressed, else otherwise
	 */
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
