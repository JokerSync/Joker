#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QTextEdit>
#include <QCloseEvent>

#include <PhTools/PhDebug.h>

#include "TextEditTestWindow.h"
#include "ui_TextEditTestWindow.h"

TextEditTestWindow::TextEditTestWindow(TextEditTestSettings *settings) :
	PhEditableDocumentWindow(settings),
	ui(new Ui::TextEditTestWindow),
	_settings(settings),
	_isModified(false)
{
	ui->setupUi(this);

	connect(ui->textEdit, &QTextEdit::textChanged, this, &TextEditTestWindow::onTextChanged);
}

TextEditTestWindow::~TextEditTestWindow()
{
	delete ui;
}

bool TextEditTestWindow::openDocument(const QString &fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly)) {
		PHDEBUG << "Error opening" << fileName;
		return false;
	}

	QTextStream ts(&file);
	ui->textEdit->setText(ts.readAll());
	QTextCursor cursor = ui->textEdit->textCursor();
	cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	ui->textEdit->setTextCursor(cursor);

	file.close();

	_isModified = false;

	return PhEditableDocumentWindow::openDocument(fileName);
}

void TextEditTestWindow::saveDocument(const QString &fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, "Error", "Unable to save " + fileName);
		return;
	}

	QTextStream ts(&file);
	PHDEBUG << ui->textEdit->toPlainText();
	ts << ui->textEdit->toPlainText();

	file.close();

	_isModified = false;

	PhEditableDocumentWindow::saveDocument(fileName);
}

void TextEditTestWindow::closeEvent(QCloseEvent *event)
{
	// the user will be asked if the document has to be saved
	PhEditableDocumentWindow::closeEvent(event);

	// if the close operation is not cancelled by the user,
	if (event->isAccepted()) {
		// Force doc to unmodified to avoid double confirmation
		// since closeEvent is called twice
		// https://bugreports.qt.io/browse/QTBUG-43344
		_isModified = false;
	}

}

QMenu *TextEditTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QAction *TextEditTestWindow::fullScreenAction()
{
	return NULL;
}

bool TextEditTestWindow::isDocumentModified()
{
	return _isModified;
}

void TextEditTestWindow::on_actionNew_triggered()
{
	PHDEBUG;
	if(checkDocumentModification()) {
		ui->textEdit->clear();
		_isModified = false;

		resetDocument();
	}
}

void TextEditTestWindow::on_actionOpen_triggered()
{
	PHDEBUG << _settings->lastDocumentFolder();
	if(checkDocumentModification()) {
		QString fileName = QFileDialog::getOpenFileName(this, "Open a text file...", _settings->lastDocumentFolder(), "Text file (*.txt)");

		if(QFile::exists(fileName)) {
			if(!openDocument(fileName))
				QMessageBox::critical(this, "", tr("Unable to open ") + fileName);
		}
	}
}

void TextEditTestWindow::on_actionSave_triggered()
{
	PHDEBUG << _settings->currentDocument();
	if(QFile(_settings->currentDocument()).exists())
		saveDocument(_settings->currentDocument());
	else
		on_actionSave_as_triggered();
}

void TextEditTestWindow::on_actionSave_as_triggered()
{
	QString fileName = _settings->currentDocument();
	if(fileName.isEmpty())
		fileName = _settings->lastDocumentFolder();

	fileName = QFileDialog::getSaveFileName(this, "Save a text file...", fileName, "Text file (*.txt)");
	PHDEBUG << fileName;
	if(!fileName.isEmpty()) {
		saveDocument(fileName);
	}
}

void TextEditTestWindow::onTextChanged()
{
	_isModified = true;
}
