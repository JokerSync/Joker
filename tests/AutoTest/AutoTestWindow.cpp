#include <QPlainTextEdit>

#include "PhTools/PhDebug.h"

#include "AutoTestWindow.h"
#include "ui_AutoTestWindow.h"

AutoTestWindow::AutoTestWindow(PhDocumentWindowSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::AutoTestWindow)
{
	ui->setupUi(this);

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
}

AutoTestWindow::~AutoTestWindow()
{
	delete ui;
}

QAction *AutoTestWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}

bool AutoTestWindow::openDocument(QString fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return false;

	QTextStream ts(&file);
	ui->plainTextEdit->setPlainText(ts.readAll());

	setCurrentDocument(fileName);
	return true;
}

QMenu *AutoTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QString AutoTestWindow::text()
{
	return ui->plainTextEdit->toPlainText();
}
