#ifndef WINDOWSPECWINDOW_H
#define WINDOWSPECWINDOW_H

#include "PhCommonUI/PhDocumentWindow.h"

namespace Ui {
class WindowSpecWindow;
}

class WindowSpecWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit WindowSpecWindow(PhDocumentWindowSettings *settings);
	~WindowSpecWindow();

	QAction *fullScreenAction();

	bool openDocument(const QString &fileName);
	QMenu* recentDocumentMenu();

	QString text();
private:
	Ui::WindowSpecWindow *ui;
};

#endif // WINDOWSPECWINDOW_H
