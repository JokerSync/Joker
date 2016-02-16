#ifndef WINDOWSPECWINDOW_H
#define WINDOWSPECWINDOW_H

#include "PhCommonUI/PhWindow.h"

namespace Ui {
class WindowSpecWindow;
}

class WindowSpecWindow : public PhWindow
{
	Q_OBJECT

public:
	explicit WindowSpecWindow(PhWindowSettings *settings);
	~WindowSpecWindow();

	QAction *fullScreenAction();
private:
	Ui::WindowSpecWindow *ui;
};

#endif // WINDOWSPECWINDOW_H
