#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include <PhTools/PhDebug.h>

#include "TextEditTestWindow.h"
#include "ui_TextEditTestWindow.h"

TextEditTestWindow::TextEditTestWindow(TextEditTestSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::TextEditTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	connect(&_watcher, SIGNAL(fileChanged(QString)), this, SLOT(onExternalChange(QString)));
	_restrain.start();
}

TextEditTestWindow::~TextEditTestWindow()
{
	delete ui;
}

bool TextEditTestWindow::openDocument(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return false;

	QTextStream ts(&file);
	ui->textEdit->setText(ts.readAll());
	_watcher.addPath(fileName);

	setCurrentDocument(fileName);
	return true;
}

QMenu *TextEditTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void TextEditTestWindow::on_actionOpen_triggered()
{
	PHDEBUG << _settings->lastDocumentFolder();
	QString fileName = QFileDialog::getOpenFileName(this, "Open a text file...", _settings->lastDocumentFolder(), "Text file (*.txt)");

	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "", tr("Unable to open ") + fileName);
	}
}

void TextEditTestWindow::on_actionSave_triggered()
{

}

void TextEditTestWindow::on_actionSave_as_triggered()
{

}

void TextEditTestWindow::onExternalChange(QString path)
{
	if(_restrain.restart() > 500)
	{
		PHDEBUG << "File changed :" << path;
		int ret = QMessageBox::warning(this,
									   "Warning",
									   "The document has been modified outside of the app.\n"
									   "Do you want to load the changes?",
									   QMessageBox::Yes | QMessageBox::No,
									   QMessageBox::Yes);
		if (ret == QMessageBox::Yes)
			openDocument(path);
		_restrain.restart();
	}
}
