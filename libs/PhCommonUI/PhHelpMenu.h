#ifndef PHHELPMENU_H
#define PHHELPMENU_H

#include <QMenu>

/**
 * @brief Helper class for OSX spotlight feature
 *
 * This class allow to fix this bug: https://bugreports.qt.io/browse/QTBUG-45602
 * that make spotlight unavailable from the help menu if translated
 */
class PhHelpMenu : public QMenu
{
public:
	/**
	 * @brief The PhHelpMenu constructor
	 * @param parent The parent widget
	 */
	PhHelpMenu(QWidget *parent = 0);
};

#endif // PHHELPMENU_H
