#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QGraphicsScene>

#include "AboutDialog.h"

#include "PhCommonUI/PhDocumentWindow.h"

#include "FormTestSettings.h"

namespace Ui {
class FormTestWindow;
}

class GoTo;
class AboutDialog;

class FormTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit FormTestWindow(FormTestSettings *settings);
	~FormTestWindow();

	void generateRGB();

	void generateYUV();

protected:
	bool openDocument(QString fileName);

	void paintEvent(QPaintEvent *);

	QMenu *recentDocumentMenu();
private slots:
	void on_actionAbout_triggered();

	void on_actionDocumentation_triggered();

	void on_actionOpen_triggered();

	void on_actionGenerate_YUV_pattern_triggered();

	void on_actionGenerate_RGB_pattern_triggered();

private:
	Ui::FormTestWindow *ui;
	AboutDialog _about;
	FormTestSettings *_settings;
	QImage *_image;
	unsigned char * _rgb;
};

#endif // MAINWINDOW_H
