#ifndef AUTOTESTWINDOW_H
#define AUTOTESTWINDOW_H

#include "PhCommonUI/PhWindow.h"

namespace Ui {
class AutoTestWindow;
}

class AutoTestWindow : public PhWindow
{
	Q_OBJECT

public:
	explicit AutoTestWindow(PhWindowSettings *settings);
	~AutoTestWindow();

	QAction *fullScreenAction();

private:
	Ui::AutoTestWindow *ui;
};

#endif // AUTOTESTWINDOW_H
