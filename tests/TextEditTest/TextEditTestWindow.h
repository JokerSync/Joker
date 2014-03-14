#ifndef TEXTEDITTESTWINDOW_H
#define TEXTEDITTESTWINDOW_H

#include <QFileSystemWatcher>
#include <QTime>

#include "PhCommonUI/PhDocumentWindow.h"

#include "TextEditTestSettings.h"

namespace Ui {
class TextEditTestWindow;
}

class TextEditTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit TextEditTestWindow(TextEditTestSettings *settings);
	~TextEditTestWindow();

protected:
	bool openDocument(QString fileName);
	QMenu *recentDocumentMenu();

private slots:
	void on_actionOpen_triggered();

	void on_actionSave_triggered();

	void on_actionSave_as_triggered();
	void onExternalChange(QString path);

private:
	Ui::TextEditTestWindow *ui;
	TextEditTestSettings *_settings;
	QFileSystemWatcher _watcher;
	QTime _restrain;
};

#endif // TEXTEDITTESTWINDOW_H
