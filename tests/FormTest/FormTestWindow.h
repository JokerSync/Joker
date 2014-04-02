#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QGraphicsScene>

#include "AboutDialog.h"

#include "PhCommonUI/PhWindow.h"

#include "FormTestSettings.h"

namespace Ui {
class FormTestWindow;
}

class GoTo;
class AboutDialog;

class FormTestWindow : public PhWindow
{
	Q_OBJECT

public:
	explicit FormTestWindow(FormTestSettings *settings);
	~FormTestWindow();

protected:
	QAction *fullScreenAction();

private slots:
	void on_actionAbout_triggered();

	void on_actionDocumentation_triggered();

	void on_pushButton_clicked();

private:
	Ui::FormTestWindow *ui;
	AboutDialog _about;
	FormTestSettings *_settings;
	QImage *_image;
	unsigned char * _rgb;
};

#endif // MAINWINDOW_H
