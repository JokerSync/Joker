#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QScreen>
#include <QTime>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhFeedbackDialog.h"

#include "FormTestWindow.h"
#include "ui_FormTestWindow.h"

FormTestWindow::FormTestWindow(FormTestSettings *settings) :
	PhWindow(settings),
	ui(new Ui::FormTestWindow),
	_settings(settings),
	_image(NULL),
	_rgb(NULL)
{
	ui->setupUi(this);

	this->restoreGeometry(_settings->windowGeometry());

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		ui->labelFrequency->setText(QString::number(screen->refreshRate()) + "hz");
}

FormTestWindow::~FormTestWindow()
{
	delete ui;
}

QAction *FormTestWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}

void FormTestWindow::on_actionAbout_triggered()
{
	_about.exec();
}

void FormTestWindow::on_actionDocumentation_triggered()
{
	if(QDesktopServices::openUrl(QUrl("http://www.doublage.org", QUrl::TolerantMode)))
		PHDEBUG <<"openned url correctly";
}

void FormTestWindow::on_pushButton_clicked()
{
	PhFeedbackDialog dlg(_settings);
	dlg.exec();
}
