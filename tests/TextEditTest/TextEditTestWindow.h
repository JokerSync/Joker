#ifndef TEXTEDITTESTWINDOW_H
#define TEXTEDITTESTWINDOW_H

#include "PhCommonUI/PhEditableDocumentWindow.h"

#include "TextEditTestSettings.h"

namespace Ui {
class TextEditTestWindow;
}

class TextEditTestWindow : public PhEditableDocumentWindow
{
	Q_OBJECT

public:
	explicit TextEditTestWindow(TextEditTestSettings *settings);
	~TextEditTestWindow();

protected:
	bool openDocument(const QString &fileName) override;
	void saveDocument(const QString &fileName) override;
	void closeEvent(QCloseEvent *event);

	QMenu *recentDocumentMenu() override;
	QAction *fullScreenAction() override;

	bool isDocumentModified() override;

private slots:
	void on_actionNew_triggered();

	void on_actionOpen_triggered();

	void on_actionSave_triggered() override;

	void on_actionSave_as_triggered();

	void onTextChanged();

private:
	Ui::TextEditTestWindow *ui;
	TextEditTestSettings *_settings;
	bool _isModified;
};

#endif // TEXTEDITTESTWINDOW_H
