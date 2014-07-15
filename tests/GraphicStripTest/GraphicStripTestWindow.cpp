#include "GraphicStripTestWindow.h"
#include "ui_GraphicStripTestWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QStandardPaths>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

GraphicStripTestWindow::GraphicStripTestWindow(GraphicStripTestSettings * settings) :
	PhDocumentWindow(settings),
	ui(new Ui::GraphicStripTestWindow),
	_settings(settings),
	_strip(settings)
{
	ui->setupUi(this);

	ui->stripView->setGraphicSettings(settings);

	_doc = _strip.doc();
	_clock = _strip.clock();
	ui->actionInvert_colors->setChecked(_settings->invertColor());
	ui->actionRuler->setChecked(_settings->displayRuler());

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(ui->actionGenerate, SIGNAL(triggered()), this, SLOT(onGenerate()));

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));

	if(_settings->generate())
		_doc->generate(_settings->textContent(),
		               _settings->loopNumber(),
		               _settings->peopleNumber(),
		               _settings->spaceBetweenText(),
		               _settings->textNumber(),
		               _settings->trackNumber(),
		               _settings->startTime());

	connect(ui->stripView, &PhGraphicView::beforePaint, _clock, &PhClock::tick);
	connect(ui->stripView, &PhGraphicView::paint, this, &GraphicStripTestWindow::onPaint);
}

GraphicStripTestWindow::~GraphicStripTestWindow()
{
	delete ui;
}

bool GraphicStripTestWindow::openDocument(QString fileName)
{
	PHDEBUG << fileName;
	if(!_doc->openStripFile(fileName))
		return false;

	_clock->setTimeCodeType(_doc->timeCodeType());
	_settings->setGenerate(false);
	setCurrentDocument(fileName);
	return true;
}

QMenu *GraphicStripTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QAction *GraphicStripTestWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}

void GraphicStripTestWindow::onOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...", _settings->lastDocumentFolder(), "Rythmo files (*.joker *.detx)");
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void GraphicStripTestWindow::onGenerate()
{
	GenerateDialog dlgGen(_settings, _doc);
	if (dlgGen.exec()) {

		if(dlgGen.getCheckBoxState()) {
			_clock->setTime(0);
			_settings->setGenerate(true);
			_doc->reset();
			_doc->addPeople(new PhPeople("A people"));
			_doc->addPeople(new PhPeople("A second people", "red"));

			_doc->addObject(new PhStripText(0, _doc->peoples().first(), 10000, 1, "Hello", 0.25f));
			_doc->addObject(new PhStripCut(PhStripCut::CrossFade, 5400));
			_doc->addObject(new PhStripDetect(PhStripDetect::Off, 0, _doc->peoples().first(), 10000, 1));
			_doc->addObject(new PhStripLoop(3, 22000));
			_doc->addObject(new PhStripText(10000, _doc->peoples().last(), 15000, 2, "Hi !", 0.25f));
			_doc->addObject(new PhStripDetect(PhStripDetect::SemiOff, 10000, _doc->peoples().last(), 15000, 2));
		}
		else {
			_clock->setTime(_doc->lastTime());
			_settings->setGenerate(true);
			setCurrentDocument("");
		}
	}

}

void GraphicStripTestWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void GraphicStripTestWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
}

void GraphicStripTestWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void GraphicStripTestWindow::on_actionPlay_backward_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripTestWindow::on_actionStep_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void GraphicStripTestWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}

void GraphicStripTestWindow::on_actionStep_time_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() + 1);
}

void GraphicStripTestWindow::on_actionStep_time_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - 1);
}

void GraphicStripTestWindow::on_action_3_triggered()
{
	_clock->setRate(-3.0);
}

void GraphicStripTestWindow::on_action_1_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripTestWindow::on_action_0_5_triggered()
{
	_clock->setRate(-0.5);
}

void GraphicStripTestWindow::on_action0_triggered()
{
	_clock->setRate(0.0);
}

void GraphicStripTestWindow::on_action0_5_triggered()
{
	_clock->setRate(0.5);
}

void GraphicStripTestWindow::on_action1_triggered()
{
	_clock->setRate(1.0);
}

void GraphicStripTestWindow::on_action3_triggered()
{
	_clock->setRate(3.0);
}

void GraphicStripTestWindow::on_actionGo_to_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType(), _clock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setFrame(dlg.frame());
}

void GraphicStripTestWindow::on_actionPrevious_Element_triggered()
{
	PhTime time = _doc->previousElementTime(_clock->time());
	if(time > PHTIMEMIN)
		_clock->setTime(time);
}

void GraphicStripTestWindow::on_actionNext_Element_triggered()
{
	PhTime time = _doc->nextElementTime(_clock->time());
	if(time < PHTIMEMAX)
		_clock->setTime(time);
}

void GraphicStripTestWindow::on_actionStrip_Properties_triggered()
{
	dlg = new StripPropertiesDialog(_doc, this);
	dlg->show();
}

void GraphicStripTestWindow::on_actionInvert_colors_triggered(bool checked)
{
	_settings->setInvertColor(checked);
}


void GraphicStripTestWindow::on_actionRuler_triggered(bool checked)
{
	_settings->setDisplayRuler(checked);
	if(checked && _settings->rulerTimeIn() == 0)
		on_actionChange_ruler_timestamp_triggered();
}

void GraphicStripTestWindow::on_actionChange_ruler_timestamp_triggered()
{
	PhTimeCodeType tcType = _doc->timeCodeType();
	PhTimeCodeDialog dlg(tcType, _settings->rulerTimeIn() / PhTimeCode::timePerFrame(tcType), this);
	if(dlg.exec())
		_settings->setRulerTimeIn(dlg.frame() * PhTimeCode::timePerFrame(tcType));
}

void GraphicStripTestWindow::onPaint(int width, int height)
{
	int h = height;
	if(_settings)
		h = height * _settings->stripHeight();
	_strip.draw(0, height - h, width, h);
	foreach(QString info, _strip.infos()) {
		ui->stripView->addInfo(info);
	}
}

void GraphicStripTestWindow::on_actionChange_font_triggered()
{
	QString fontFile = QFileDialog::getOpenFileName(this, "Change font...", "", "Font files (*.ttf)");
	if(QFile(fontFile).exists()) {
		if(PhFont::computeMaxFontSize(fontFile) == 0)
			QMessageBox::critical(this, "Error", "Unable to open " + fontFile);
		else
			_settings->setTextFontFile(fontFile);
	}
}
