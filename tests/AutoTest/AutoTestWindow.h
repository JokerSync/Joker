#ifndef AUTOTESTWINDOW_H
#define AUTOTESTWINDOW_H

#include "PhCommonUI/PhDocumentWindow.h"

namespace Ui {
class AutoTestWindow;
}

class AutoTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit AutoTestWindow(PhDocumentWindowSettings *settings);
	~AutoTestWindow();

	QAction *fullScreenAction();

	bool openDocument(QString fileName);
	QMenu* recentDocumentMenu();

	QString text();
private:
	Ui::AutoTestWindow *ui;
};

#endif // AUTOTESTWINDOW_H
