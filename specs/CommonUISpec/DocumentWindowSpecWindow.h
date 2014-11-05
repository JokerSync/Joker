/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef DOCUMENTWINDOWSPECWINDOW_H
#define DOCUMENTWINDOWSPECWINDOW_H

#include "PhCommonUI/PhDocumentWindow.h"

#include "DocumentWindowSpecSettings.h"

namespace Ui {
class DocumentWindowSpecWindow;
}

class DocumentWindowSpecWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit DocumentWindowSpecWindow(DocumentWindowSpecSettings *settings = 0);
	~DocumentWindowSpecWindow();

	bool openDocument(const QString &fileName);
	QMenu* recentDocumentMenu();

	QString text();

private:
	Ui::DocumentWindowSpecWindow *ui;
};

#endif // DOCUMENTWINDOWSPECWINDOW_H
