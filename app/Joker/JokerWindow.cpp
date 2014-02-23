/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFileOpenEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(JokerSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, settings),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	#if USE_LTC
	_ltcReader(),
	#endif
	_needToSave(false)
{
	// Setting up UI
	ui->setupUi(this);

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	// Get the pointer to the differents objects :
	// strip, video engine and doc
	_strip = ui->videoStripView->strip();
	_videoEngine = ui->videoStripView->videoEngine();
	_doc = _strip->doc();

	// Pass the settings to the modules
	_strip->setSettings(_settings);
	_videoEngine->setSettings(_settings);
	ui->videoStripView->setSettings(_settings);

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
	_propertyDialog.setVideoEngine(_videoEngine);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip->clock());
	_synchronizer.setVideoClock(_videoEngine->clock());

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_strip->clock());
	_mediaPanel.setStyleSheet(
	    "* {"
	    "	  color: white;"
	    "  }"
	    "  PhMediaPanel { "
	    "	  background: qlineargradient(x1: 1, y1: 0, x2: 1, y2: 1, stop: 0 rgb(40,40,40), stop: 1 black);"
	    "	  border-style: solid;                                                                          "
	    "	  border-width: 4px;                                                                            "
	    "	  border-radius: 3px;                                                                           "
	    "	  border-color: white;                                                                          "
	    "  }                                                                                                "
	    "  QPushButton, QComboBox{                                                                          "
	    "	  background: grey;                                                                             "
	    "	  border-style: outset;                                                                         "
	    "	  border-width: 2px;                                                                            "
	    "	  border-radius: 5px;                                                                           "
	    "	  border-color: white;                                                                          "
	    "  }                                                                                                "
	    "  QLabel#_timecodeLabel{                                                                           "
	    "	  padding: 10px;                                                                                "
	    "  }                                                                                                "
	    );
	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);

	// Set up a filter for catching mouse move event (see eventFilter())
	// that will show the media panel back
	qApp->installEventFilter(this);

	this->setFocus();

	if(_settings->stripTestMode()) {
#warning TODO do we warn the user that test mode is on?
		ui->actionTest_mode->setChecked(true);
	}

#warning TODO fix fullscreen on startup
	if(_settings->fullScreen())
		this->showFullScreen();

	// This is for the drag and drop feature
	setAcceptDrops(true);
}

JokerWindow::~JokerWindow()
{
	delete ui;
}

void JokerWindow::setupSyncProtocol()
{
	PhClock* clock = NULL;

	// Disable old protocol
	_sonySlave.close();
#if USE_LTC
	_ltcReader.close();
#endif
	VideoStripSynchronizer::SyncType type = (VideoStripSynchronizer::SyncType)_settings->synchroProtocol();

	switch(type) {
	case VideoStripSynchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open()) {
			clock = _sonySlave.clock();
			ui->videoStripView->setSony(&_sonySlave);
		}
		else {
			type = VideoStripSynchronizer::NoSync;
			QMessageBox::critical(this, "", "Unable to connect to USB422v module");
		}
		break;
#if USE_LTC
	case VideoStripSynchronizer::LTC:
		{
			QString input = _settings->ltcInputDevice();
			if(_ltcReader.init(input))
				clock = _ltcReader.clock();
			else {
				QMessageBox::critical(this, "", "Unable to open " + input);
				type = VideoStripSynchronizer::NoSync;
			}
			break;
		}
#endif
	case VideoStripSynchronizer::NoSync:
		break;
	}

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(QString fileName)
{
	hideMediaPanel();

	if(!_doc->openStripFile(fileName))
		return false;

	setCurrentDocument(fileName);

	ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());
	ui->videoStripView->setForceRatio169(_doc->forceRatio169());

	// Opening the corresponding video file if it exists
	if(openVideoFile(_doc->getVideoPath())) {
		PhFrame frameStamp = _doc->getVideoTimestamp();
		_videoEngine->setFirstFrame(frameStamp);
		_mediaPanel.setFirstFrame(frameStamp);
	}
	else
		_videoEngine->close();

	// On succeed, synchronizing the clocks
	_strip->clock()->setTimeCodeType(_doc->getTCType());
	_strip->clock()->setFrame(_doc->getLastFrame());

	_needToSave = false;
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	switch (event->type()) {
	case QEvent::FileOpen:
		{
			QString filePath = static_cast<QFileOpenEvent *>(event)->file();
			QString fileType = filePath.split(".").last().toLower();
			// As the plist file list all the supported format (which are .strip, .detx, .avi & .mov)
			// if the file is not a strip or a detx file, it's a video file, we don't need any protection
			if(fileType == "detx" or fileType == "strip") {
				if(checkSaveFile())
					openDocument(filePath);
			}
			else
				openVideoFile(filePath);
			break;
		}

	case QEvent::ApplicationDeactivate:
		hideMediaPanel();
		break;
	case QEvent::MouseMove:
		// Show the mediaPanel only if Joker has focus and is not remote controlled.
		if(this->hasFocus() && _settings->synchroProtocol() == 0)
			// Show the mediaPanel only if Joker has focus.
			if(this->hasFocus())
				fadeInMediaPanel();
		break;

	case QEvent::Drop:
		{
			const QMimeData* mimeData = static_cast<QDropEvent *>(event)->mimeData();

			// If there is one file (not more) we open it
			if (mimeData->urls().length() == 1) {
				QString filePath = mimeData->urls().first().toLocalFile();
				QString fileType = filePath.split(".").last().toLower();
				if(fileType == "detx" or fileType == "strip") {
					if(checkSaveFile())
						openDocument(filePath);
				}
				else if (fileType == "avi" or fileType == "mov")
					openVideoFile(filePath);
			}
			break;
		}
	case QEvent::DragEnter:
		event->accept();
		break;
	case QEvent::MouseButtonDblClick:
		// If the sender is "this" and no videofile is loaded
		if(sender->objectName() == this->objectName() and !_videoEngine->fileName().length()) {
			// It's useless to check for the x position because if it's out of the bounds, the sender will not be "this"
			if(QCursor::pos().y() > this->pos().y() and QCursor::pos().y() < this->pos().y() + this->height() * (1.0 - _settings->stripHeight()))
				on_actionOpen_Video_triggered();
			return true;
		}
		if(sender->objectName() == this->objectName()) {
			if(isFullScreen())
				showNormal();
			else
				showFullScreen();
			return true;
		}
		break;
	case QEvent::KeyPress:
		if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_F10) {
			if(this->isFullScreen()) {
				this->showNormal();
			}
			else {
				this->showFullScreen();
			}
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

QMenu *JokerWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	if(!checkSaveFile())
		event->ignore();
	else
		_mediaPanel.close();
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkSaveFile()) {
		QString filter = tr("DetX files") + " (*.detx);; "
		                 + tr("Joker files") + " (*.strip);; "
		                 + tr("Rythmo files") + " (*.detx *.strip);; "
		                 + tr("All files") + " (*.*)";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.selectNameFilter(_settings->selectedFilter());
		dlg.setOption(QFileDialog::HideNameFilterDetails, false);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			openDocument(fileName);
			_settings->setSelectedFilter(dlg.selectedNameFilter());
		}
	}
	fadeInMediaPanel();
}

void JokerWindow::on_actionPlay_pause_triggered()
{
	if(_strip->clock()->rate() == 0.0)
		_strip->clock()->setRate(1.0);
	else
		_strip->clock()->setRate(0.0);
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() + 1);
}

void JokerWindow::on_actionStep_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() - 1);
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	_strip->clock()->setRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	_strip->clock()->setRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	_strip->clock()->setRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	_strip->clock()->setRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	_strip->clock()->setRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	_strip->clock()->setRate(3.0);
}

void JokerWindow::on_actionOpen_Video_triggered()
{
	hideMediaPanel();

	QString lastFolder = _settings->lastVideoFolder();
	QFileDialog dlg(this, tr("Open..."), lastFolder, tr("Movie files") + " (*.avi *.mov)");
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];
		if(openVideoFile(videoFile))
			_strip->clock()->setFrame(_doc->getVideoTimestamp());
	}

	fadeInMediaPanel();
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo lastFileInfo(_doc->getVideoPath());
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine->open(videoFile)) {
		PhFrame frameStamp = _videoEngine->firstFrame();

		_mediaPanel.setFirstFrame(frameStamp);
		_mediaPanel.setMediaLength(_videoEngine->length());

		if(videoFile != _doc->getVideoPath()) {
			_doc->setVideoPath(videoFile);
			if(frameStamp > 0)
				_doc->setVideoTimestamp(frameStamp);
			_needToSave = true;
		}

		if(frameStamp == 0) {
			frameStamp = _doc->getVideoTimestamp();
			_videoEngine->setFirstFrame(frameStamp);
			_videoEngine->clock()->setFrame(frameStamp);
			if(fileInfo.fileName() != lastFileInfo.fileName()) {
				on_actionChange_timestamp_triggered();
				frameStamp = _videoEngine->firstFrame();
			}
		}

		_videoEngine->clock()->setFrame(frameStamp);

		_settings->setLastVideoFolder(fileInfo.absolutePath());
		return true;
	}
	return false;
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();
	PhFrame frame;
	if(_synchronizer.videoClock()->frame() < _videoEngine->firstFrame())
		frame = _videoEngine->firstFrame();
	else if(_synchronizer.videoClock()->frame() > _videoEngine->firstFrame() + _videoEngine->length())
		frame = _videoEngine->lastFrame();
	else
		frame = _synchronizer.videoClock()->frame();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), frame);
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp;
		if(_synchronizer.videoClock()->frame() > _videoEngine->firstFrame() + _videoEngine->length())
			frameStamp = dlg.frame() - (_videoEngine->length() - 1);
		else if (_synchronizer.videoClock()->frame() < _videoEngine->firstFrame())
			frameStamp =  dlg.frame();
		else
			frameStamp = _videoEngine->firstFrame() + dlg.frame() - _synchronizer.videoClock()->frame();

		_videoEngine->setFirstFrame(frameStamp);
		_strip->clock()->setFrame(dlg.frame());
		_doc->setVideoTimestamp(frameStamp);
		_mediaPanel.setFirstFrame(frameStamp);
		_needToSave = true;
	}

	fadeInMediaPanel();
}



void JokerWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutDialog menu;
	menu.exec();

	fadeInMediaPanel();
}


void JokerWindow::on_actionPreferences_triggered()
{
	hideMediaPanel();
	int oldSynchroProtocol = _settings->synchroProtocol();
	QString oldLTCInputDevice = _settings->ltcInputDevice();

	PreferencesDialog dlg(_settings);
	dlg.exec();
	if((oldSynchroProtocol != _settings->synchroProtocol()) || (oldLTCInputDevice != _settings->ltcInputDevice())) {
		PHDEBUG << "Set protocol:" << _settings->synchroProtocol();
		setupSyncProtocol();
	}

	fadeInMediaPanel();
}

void JokerWindow::fadeInMediaPanel()
{
	// Don't show the mediaPanel if Joker is remote controled.
	if(_settings->synchroProtocol() != VideoStripSynchronizer::NoSync)
		return;

	_mediaPanel.show();
	_mediaPanelAnimation.stop();
	_mediaPanelAnimation.setDuration(300);
	_mediaPanelAnimation.setEndValue(1);
	_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
	_mediaPanelAnimation.start();
	_mediaPanelState = MediaPanelVisible;
	_mediaPanelTimer.start(3000);
}

void JokerWindow::fadeOutMediaPanel()
{
	// Don't fade out the media panel if the mouse is over it
	if(_mediaPanel.underMouse() or _mediaPanel.isMousePressed()) {
		PHDEBUG << "Don't hide";
		_mediaPanelTimer.start(3000);
		return;
	}

	PHDEBUG << _mediaPanelState;
	switch(_mediaPanelState) {
	case MediaPanelVisible:
		PHDEBUG << "Hiding";
		_mediaPanelAnimation.setDuration(1000);
		_mediaPanelAnimation.setEndValue(0);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelTimer.start(1000);
		_mediaPanelState = MediaPanelHidding;
		break;
	case MediaPanelHidding:
		hideMediaPanel();
		break;
	default:
		break;
	}
}

void JokerWindow::hideMediaPanel()
{
	_mediaPanel.hide();
	_mediaPanelState = MediaPanelHidden;
	_mediaPanelTimer.stop();
}


void JokerWindow::on_actionProperties_triggered()
{
	_propertyDialog.show();
}

void JokerWindow::on_actionTest_mode_triggered()
{
	_settings->setStripTestMode(!_settings->stripTestMode());
}

void JokerWindow::on_actionTimecode_triggered()
{
	hideMediaPanel();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip->clock()->setFrame(dlg.frame());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhFrame frame = _doc->getNextElementFrame(_strip->clock()->frame());
	if(frame < PHFRAMEMAX)
		_strip->clock()->setFrame(frame);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhFrame frame = _doc->getPreviousElementFrame(_strip->clock()->frame());
	if(frame > PHFRAMEMIN)
		_strip->clock()->setFrame(frame);
}

void JokerWindow::on_actionClear_list_triggered()
{
	//Open the recent group
//	_settings->beginGroup("openRecent");
//	//List all keys
//	QStringList indexes = _settings->allKeys();
//	//Remove them from
//	foreach(QString index, indexes)
//	_settings->remove(index);

//	//Close the group
//	_settings->endGroup();

	//Remove the buttons of the UI, keep the separator and the Clear button
	foreach(QAction * action, ui->menuOpen_recent->actions())
	{
		// Break if the separator is reached
		if(action->isSeparator())
			break;
		// Remove it
		ui->menuOpen_recent->removeAction(action);
		delete action;
	}

	// Remove all the buttons
//	_recentFileButtons.clear();
	ui->menuOpen_recent->setEnabled(false);
}

void JokerWindow::on_actionSave_triggered()
{
	QString fileName = _settings->currentDocument();
	QFileInfo info(fileName);
	if(!info.exists() || (info.suffix() != "strip"))
		on_actionSave_as_triggered();
	else if(_doc->saveStrip(fileName, _strip->clock()->timeCode(), ui->actionForce_16_9_ratio->isChecked()))
		_needToSave = false;
	else
		QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
}

void JokerWindow::on_actionSave_as_triggered()
{
	hideMediaPanel();

	QString fileName = _settings->currentDocument();
	QString lastFolder = _settings->lastDocumentFolder();
	// If there is no current strip file, ask for a name
	if(fileName == "")
		fileName = lastFolder;
	else {
		QFileInfo info(fileName);
		if(info.suffix() != "strip")
			fileName = lastFolder + "/" + info.completeBaseName() + ".strip";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName,"*.strip");
	if(fileName != "") {
		if(_doc->saveStrip(fileName, _strip->clock()->timeCode(), ui->actionForce_16_9_ratio->isChecked())) {
			_needToSave = false;
			setCurrentDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
	}
}

bool JokerWindow::checkSaveFile()
{
	if(_needToSave) {
		QString msg = tr("Do you want to save your changes ?");
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		switch(box.exec()) {
		case QMessageBox::Save:
			on_actionSave_triggered();
			if(_needToSave)
				return false;
			break;
		case QMessageBox::Cancel:
			return false;
		}
	}
	return true;
}

void JokerWindow::on_actionSelect_character_triggered()
{
	hideMediaPanel();

	PeopleDialog dlg(this, _doc, ui->videoStripView->getSelectedPeoples());

	dlg.exec();
}

void JokerWindow::on_actionForce_16_9_ratio_triggered()
{
	ui->videoStripView->setForceRatio169(ui->actionForce_16_9_ratio->isChecked());
	_needToSave = true;
}
