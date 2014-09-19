/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFileDialog>
#include <QFontDialog>
#include <QFileOpenEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QWindowStateChangeEvent>
#include <QMouseEvent>

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"

#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(JokerSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_strip(settings),
	_videoEngine(settings),
	_doc(_strip.doc()),
	_sonySlave(settings),
	_ltcReader(settings),
	_mtcReader(PhTimeCodeType25),
	_mtcWriter(PhTimeCodeType25),
	_mediaPanelState(MediaPanelHidden),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_firstDoc(true),
	_resizingStrip(false),
	_numberOfDraw(0)
{
	// Setting up UI
	ui->setupUi(this);

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	ui->videoStripView->setGraphicSettings(_settings);

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
	_propertyDialog.setVideoEngine(&_videoEngine);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip.clock());
	_synchronizer.setVideoClock(_videoEngine.clock());

	connect(&_sonySlave, &PhSonySlaveController::videoSync, this, &JokerWindow::onVideoSync);

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_doc->videoTimeCodeType(), _strip.clock());
#warning /// @todo move to CSS file
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

	ui->actionDisplay_the_control_panel->setChecked(_settings->displayControlPanel());

	fadeInMediaPanel();

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);

	this->setFocus();

	ui->actionDisplay_the_information_panel->setChecked(_settings->displayNextText());

#warning /// @todo do we warn the user that test mode is on?
	ui->actionTest_mode->setChecked(_settings->stripTestMode());

#warning /// @todo move to PhDocumentWindow
	// This is for the drag and drop feature
	setAcceptDrops(true);

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionDisplay_the_vertical_scale->setChecked(_settings->displayVerticalScale());

	ui->actionShow_ruler->setChecked(_settings->displayRuler());

	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::tick);

	this->connect(ui->videoStripView, &PhGraphicView::paint, this, &JokerWindow::onPaint);

	_videoLogo.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png");
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
	_ltcReader.close();
	_mtcReader.close();
	_mtcWriter.close();

	PhSynchronizer::SyncType type = (PhSynchronizer::SyncType)_settings->synchroProtocol();

	switch(type) {
	case PhSynchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open()) {
			clock = _sonySlave.clock();
			_lastVideoSyncElapsed.start();
		}
		else {
			type = PhSynchronizer::NoSync;
			QMessageBox::critical(this, tr("Error"), tr("Unable to connect to USB422v module"));
		}
		break;
	case PhSynchronizer::LTC:
		if(_ltcReader.init(_settings->ltcInputPort()))
			clock = _ltcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0")).arg(_settings->ltcInputPort()));
			type = PhSynchronizer::NoSync;
		}
		break;
	case PhSynchronizer::MTC:
		if(_mtcReader.open(_settings->mtcInputPort()))
			clock = _mtcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(_settings->mtcInputPort()));
			type = PhSynchronizer::NoSync;
		}
	case PhSynchronizer::NoSync:
		break;
	}

	if(_settings->sendMmcMessage()) {
		if(!_mtcWriter.open(_settings->mmcOutputPort())) {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(_settings->mmcOutputPort()));
			_settings->setSendMmcMessage(false);
		}
	}

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(QString fileName)
{
	/// Clear the selected people name list (except for the first document).
	if(!_firstDoc)
		_settings->setSelectedPeopleNameList(QStringList());
	else
		_firstDoc = false;

	if(!_doc->openStripFile(fileName))
		return false;

	/// If the document is opened successfully :
	/// - Update the current document name (settings, windows title)
	setCurrentDocument(fileName);
	_watcher.addPath(_doc->filePath());

	/// - Load the deinterlace settings
	_videoEngine.setDeinterlace(_doc->videoDeinterlace());
	ui->actionDeinterlace_video->setChecked(_doc->videoDeinterlace());

	/// - Open the corresponding video file if it exists.
	if(openVideoFile(_doc->videoFilePath())) {
		_videoEngine.setTimeIn(_doc->videoTimeIn());
		_mediaPanel.setTimeIn(_doc->videoTimeIn());
	}
	else
		_videoEngine.close();


	/// - Set the video aspect ratio.
	ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());

	/// - Goto to the document last position.
	setCurrentTime(_doc->lastTime());
	/// - Disable the need to save flag.

	return true;
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	/// The event filter catch the following event:
	switch (event->type()) {
	case QEvent::FileOpen: /// - FileOpen : To process a file dragged on the application dock icon (MacOS)
		{
#warning /// @todo move to PhDocumentWindow
			QString filePath = static_cast<QFileOpenEvent *>(event)->file();
			QString fileType = filePath.split(".").last().toLower();
			// As the plist file list all the supported format
			// if the file is not a strip file, it's a video file, we don't need any protection
			if(_settings->stripFileType().contains(fileType)) {
				if(checkSaveFile())
					openDocument(filePath);
			}
			else if(_settings->videoFileType().contains(fileType))
				openVideoFile(filePath);
			break;
		}
	case QEvent::ApplicationDeactivate: /// - ApplicationDeactivate : to hide the mediapanel
		hideMediaPanel();
		break;
	case QEvent::MouseMove: /// - Mouse move show the media panel
	case QEvent::HoverEnter:
	case QEvent::HoverMove:
		{
			fadeInMediaPanel();

			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			// Check if it is near the video/strip border
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10))
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);

			if(_resizingStrip && (mouseEvent->buttons() & Qt::LeftButton)) {
				PHDEBUG << "resizing strip:" << mouseEvent->pos();
				_settings->setStripHeight(1.0 - ((float) mouseEvent->pos().y() /(float) this->height()));
			}
			break;
		}
	case QEvent::DragEnter: /// - Accept and process a file drop on the window
		event->accept();
		break;
	case QEvent::Drop:
		{
#warning /// @todo move to PhDocumentWindow
			const QMimeData* mimeData = static_cast<QDropEvent *>(event)->mimeData();

			// If there is one file (not more) we open it
			if (mimeData->urls().length() == 1) {
				QString filePath = mimeData->urls().first().toLocalFile();
				QString fileType = filePath.split(".").last().toLower();
				if(fileType == "detx" or fileType == "strip" or fileType == "joker") {
					if(checkSaveFile())
						openDocument(filePath);
				}
				else if (fileType == "avi" or fileType == "mov")
					openVideoFile(filePath);
			}
			break;
		}
	case QEvent::MouseButtonDblClick: /// - Double mouse click toggle fullscreen mode
		if(sender == this)
			toggleFullScreen();
		break;
	case QEvent::MouseButtonRelease:
		PHDEBUG << "end resizing strip";
		_resizingStrip = false;
		break;
	case QEvent::MouseButtonPress:
		{
			QMouseEvent *mouseEvent = (QMouseEvent*)event;
			if((sender == this) && (mouseEvent->buttons() & Qt::RightButton)) {
				/// - Right mouse click on the video open the video file dialog.
				if(mouseEvent->y() < this->height() * (1.0f - _settings->stripHeight()))
					on_actionOpen_Video_triggered();
				else /// - Left mouse click on the strip open the strip file dialog.
					on_actionOpen_triggered();
				return true;
			}
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10)) {
				PHDEBUG << "start resizing strip";
				_resizingStrip = true;
			}
		}
	default:
		break;
	}

	return PhDocumentWindow::eventFilter(sender, event);
}

QMenu *JokerWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QAction *JokerWindow::fullScreenAction()
{
	return ui->actionFullscreen;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	/// Check if the current document has to be saved (it might cancel the action).
	if(!checkSaveFile())
		event->ignore();
	else /// Close the PhMediaPanel.
		_mediaPanel.close();
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkSaveFile()) {
		QString filter = tr("Rythmo files") + " (";
		foreach(QString type, _settings->stripFileType())
			filter += "*." + type + " ";
		filter += ");;";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.setOption(QFileDialog::HideNameFilterDetails, false);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			openDocument(fileName);
		}
	}
	fadeInMediaPanel();
}

void JokerWindow::on_actionPlay_pause_triggered()
{
	if(currentRate() == 0.0) {
		setCurrentRate(1.0);
	}
	else {
		setCurrentRate(0.0);
	}
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	setCurrentRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() + PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	setCurrentRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	setCurrentRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	setCurrentRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	setCurrentRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	setCurrentRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	setCurrentRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	setCurrentRate(3.0);
}

void JokerWindow::on_actionOpen_Video_triggered()
{
	hideMediaPanel();

	QString lastFolder = _settings->lastVideoFolder();
	QString filter = tr("Movie files") + " (";
	foreach(QString type, _settings->videoFileType())
		filter += "*." + type + " ";
	filter += ");;";

	QFileDialog dlg(this, tr("Open a video..."), lastFolder, filter);
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];
		if(openVideoFile(videoFile))
			setCurrentTime(_doc->videoTimeIn());
	}

	fadeInMediaPanel();
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo lastFileInfo(_doc->videoFilePath());
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine.open(videoFile)) {
		PhTime videoTimeIn = _videoEngine.timeIn();

		if(videoFile != _doc->videoFilePath()) {
			_doc->setVideoFilePath(videoFile);
			_doc->setVideoTimeIn(videoTimeIn, _videoEngine.timeCodeType());
			_doc->setModified(true);
		}

		if(videoTimeIn == 0) {
			videoTimeIn = _doc->videoTimeIn();
			_videoEngine.setTimeIn(videoTimeIn);
			_videoEngine.clock()->setTime(videoTimeIn);
			if(fileInfo.fileName() != lastFileInfo.fileName()) {
				on_actionChange_timestamp_triggered();
				videoTimeIn = _videoEngine.timeIn();
			}
		}

		_videoEngine.clock()->setTime(videoTimeIn);
		_mediaPanel.setTimeIn(videoTimeIn);
		_mediaPanel.setLength(_videoEngine.length());

		_settings->setLastVideoFolder(fileInfo.absolutePath());
		return true;
	}
	return false;
}

void JokerWindow::timeCounter(PhTimeScale frequency)
{
	if(currentRate() == 1 && (PhSynchronizer::SyncType)_settings->synchroProtocol() != PhSynchronizer::NoSync) {
		_numberOfDraw++;
		if(_numberOfDraw >= frequency) {
			_numberOfDraw = 0;
			_settings->setTimePlayed(_settings->timePlayed() + 1);
		}
	}
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();
	setCurrentRate(0);
	PhTime time;
	if(_synchronizer.videoClock()->time() < _videoEngine.timeIn())
		time = _videoEngine.timeIn();
	else if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
		time = _videoEngine.timeOut();
	else
		time = _synchronizer.videoClock()->time();

	PhTimeCodeDialog dlg(_doc->videoTimeCodeType(), time);
	if(dlg.exec() == QDialog::Accepted) {
		PhTime timeStamp;
		if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
			timeStamp = dlg.time() - (_videoEngine.length() - PhTimeCode::timePerFrame(_videoEngine.timeCodeType()));
		else if (_synchronizer.videoClock()->time() < _videoEngine.timeIn())
			timeStamp =  dlg.time();
		else
			timeStamp = _videoEngine.timeIn() + dlg.time() - _synchronizer.videoClock()->time();

		_videoEngine.setTimeIn(timeStamp);
		setCurrentTime(dlg.time());
		_doc->setVideoTimeIn(timeStamp, _videoEngine.timeCodeType());
		_mediaPanel.setTimeIn(timeStamp);
		_doc->setModified(true);
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
	QString oldLtcInputPort = _settings->ltcInputPort();
	QString oldMtcInputPort = _settings->mtcInputPort();
	bool oldSendMmcMessage = _settings->sendMmcMessage();
	QString oldMmcOutputPort = _settings->mmcOutputPort();

	PreferencesDialog dlg(_settings);
	if(dlg.exec() == QDialog::Accepted) {
		if(((oldSynchroProtocol != _settings->synchroProtocol())
		    || (oldLtcInputPort  != _settings->ltcInputPort())
		    || (oldMtcInputPort != _settings->mtcInputPort()))
		   || (oldSendMmcMessage != _settings->sendMmcMessage())
		   || (oldMmcOutputPort != _settings->mmcOutputPort())) {
			PHDEBUG << "Set protocol:" << _settings->synchroProtocol();
			setupSyncProtocol();
		}
	}

	fadeInMediaPanel();
}

void JokerWindow::fadeInMediaPanel()
{
	// Don't show the mediaPanel if Joker has not thefocus.
	if(_settings->displayControlPanel() && this->hasFocus()) {
		_mediaPanel.show();
		_mediaPanelAnimation.stop();
		_mediaPanelAnimation.setDuration(300);
		_mediaPanelAnimation.setEndValue(1);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelState = MediaPanelVisible;
		_mediaPanelTimer.start(3000);
	}
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

	PhTimeCodeDialog dlg(_videoEngine.timeCodeType(), currentTime());
	if(dlg.exec() == QDialog::Accepted)
		setCurrentTime(dlg.time());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhTime time = _doc->nextElementTime(currentTime());
	if(time < PHTIMEMAX)
		setCurrentTime(time);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhTime time = _doc->previousElementTime(currentTime());
	if(time > PHTIMEMIN)
		setCurrentTime(time);
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
	foreach(QAction * action, ui->menuOpen_recent->actions()) {
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
	if(!info.exists() || (info.suffix() != "joker"))
		on_actionSave_as_triggered();
	else if(_doc->saveStripFile(fileName, currentTime()))
		_doc->setModified(false);
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
		if(info.suffix() != "joker")
			fileName = lastFolder + "/" + info.completeBaseName() + ".joker";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName, "*.joker");
	if(fileName != "") {
		if(_doc->saveStripFile(fileName, currentTime())) {
			_doc->setModified(false);
			setCurrentDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
	}
}

bool JokerWindow::checkSaveFile()
{

	if(_doc->modified()) {
		/// If the document need to be saved, ask the user
		/// whether he wants to save his changes.
		QString msg = tr("Do you want to save your changes ?");
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		switch(box.exec()) {
		/// Cancel the caller action if clicking cancel.
		case QMessageBox::Cancel:
			return false;
		/// Trigger the document save if clicking save:
		case QMessageBox::Save:
			on_actionSave_triggered();
			/// If the user cancel the save operation, cancel the operation.
			if(_doc->modified())
				return false;
			break;
		}
	}
	/// @return False to interrupt the caller action, true otherwhise.
	return true;
}

void JokerWindow::on_actionSelect_character_triggered()
{
	hideMediaPanel();

	PeopleDialog dlg(this, _doc, _settings);

	dlg.restoreGeometry(_settings->peopleDialogGeometry());
	dlg.exec();
	_settings->setPeopleDialogGeometry(dlg.saveGeometry());
}

void JokerWindow::on_actionForce_16_9_ratio_triggered(bool checked)
{
	_doc->setForceRatio169(checked);
	_doc->setModified(true);
}

void JokerWindow::on_actionInvert_colors_toggled(bool checked)
{
	_settings->setInvertColor(checked);
}

void JokerWindow::on_actionShow_ruler_toggled(bool checked)
{
	_settings->setDisplayRuler(checked);
}

void JokerWindow::on_actionChange_ruler_timestamp_triggered()
{
	PhTimeCodeType tcType = _videoEngine.timeCodeType();
	PhTimeCodeDialog dlg(tcType, _settings->rulerTimeIn(), this);
	if(dlg.exec())
		_settings->setRulerTimeIn(dlg.time());
}

void JokerWindow::on_actionNew_triggered()
{
	_doc->reset();
	on_actionClose_video_triggered();
}

void JokerWindow::on_actionClose_video_triggered()
{
	_videoEngine.close();
}

void JokerWindow::on_actionSend_feedback_triggered()
{
	hideMediaPanel();
	PhFeedbackDialog dlg(_settings, this);
	dlg.exec();
	fadeInMediaPanel();
}

void JokerWindow::on_actionDeinterlace_video_triggered(bool checked)
{
	_videoEngine.setDeinterlace(checked);
	if(checked != _doc->videoDeinterlace()) {
		_doc->setVideoDeinterlace(checked);
		_doc->setModified(true);
	}
}

void JokerWindow::on_actionHide_the_rythmo_triggered(bool checked)
{
	_settings->setHideStrip(checked);
}

void JokerWindow::onPaint(int width, int height)
{
	PhClock *clock = _videoEngine.clock();
	long delay = (int)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;

	float stripHeightRatio = 0.0f;
	if(!_settings->hideStrip())
		stripHeightRatio = _settings->stripHeight();

	int stripHeight = height * stripHeightRatio;
	int videoHeight = height - stripHeight;
	int videoWidth = videoHeight * 16 / 9;
	if(!_doc->forceRatio169() && (_videoEngine.height() > 0))
		videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();

	int videoX = 0;
	// Center video if no information panel with next text
	if(!_settings->displayNextText())
		videoX = (width - videoWidth) / 2;

	// Display the video
	if((videoHeight > 0)) {
		if(_videoEngine.height() > 0) {

			int blackStripHeight = 0; // Height of the upper black strip when video is too large
			int realVideoHeight = videoHeight;
			if(videoWidth > width) {
				videoWidth = width;
				if(_doc->forceRatio169())
					realVideoHeight = videoWidth  * 9 / 16;
				else
					realVideoHeight = videoWidth  * _videoEngine.height() / _videoEngine.width();
			}
			blackStripHeight = (height - stripHeight - realVideoHeight) / 2;

			_videoEngine.drawVideo(videoX, blackStripHeight, videoWidth, realVideoHeight);
		}
		else if(_settings->displayLogo()) {
			// The logo file is 500px in native format
			int logoHeight = _videoLogo.originalSize().height();
			int logoWidth = _videoLogo.originalSize().width();
			if(videoHeight < logoHeight) {
				logoHeight = videoHeight;
				logoWidth = _videoLogo.originalSize().width() * logoHeight / _videoLogo.originalSize().height();
			}
			if(width < logoWidth) {
				logoWidth = width;
				logoHeight = _videoLogo.originalSize().height() * logoWidth / _videoLogo.originalSize().width();
			}
			_videoLogo.setRect((width - logoHeight) / 2, (videoHeight - logoHeight) / 2, logoHeight, logoHeight);
			_videoLogo.draw();
		}
	}

	// Get the selected people list
	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	int x = videoX + videoWidth;
	int y = 0;
	if(_settings->displayNextText()) {
		QColor infoColor = _settings->backgroundColorLight();
		int infoWidth = width - videoWidth;
		int spacing = 4;

		// Display the title
		{
			QString title = _strip.doc()->title().toLower();
			if(_strip.doc()->episode().length() > 0)
				title += " #" + _strip.doc()->episode().toLower();

			int titleHeight = height / 40;
			int titleWidth = _strip.getHUDFont()->getNominalWidth(title) / 2;
			PhGraphicText titleText(_strip.getHUDFont());
			titleText.setColor(infoColor);
			titleText.setRect(x + spacing, y, titleWidth, titleHeight);
			y += titleHeight;
			titleText.setContent(title);
			titleText.setZ(5);
			titleText.draw();
		}

		// Display the current timecode
		{
			int tcWidth = infoWidth - 2 * spacing;
			int tcHeight = infoWidth / 6;
			PhGraphicText tcText(_strip.getHUDFont());
			tcText.setColor(infoColor);
			tcText.setRect(x + 4, y, tcWidth, tcHeight);
			tcText.setContent(PhTimeCode::stringFromTime(clockTime, _videoEngine.timeCodeType()));
			tcText.draw();

			y += tcHeight;
		}

		// Display the box around current loop number
		int boxWidth = infoWidth / 4;
		int boxHeight = infoWidth / 6;
		int nextTcWidth = infoWidth - boxWidth - 12;
		int nextTcHeight = nextTcWidth / 6;
		{
			int borderWidth = 2;
			PhGraphicSolidRect outsideLoopRect(x + spacing, y, boxWidth, boxHeight);
			outsideLoopRect.setColor(infoColor);
			outsideLoopRect.draw();

			PhGraphicSolidRect insideLoopRect(x + spacing + borderWidth, y + borderWidth, boxWidth - 2 * borderWidth, boxHeight - 2 * borderWidth);
			insideLoopRect.setColor(Qt::black);
			insideLoopRect.draw();

			// Display the current loop number
			QString loopLabel = "0";
			PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
			if(currentLoop)
				loopLabel = currentLoop->label();
			int loopWidth = _strip.getHUDFont()->getNominalWidth(loopLabel) / 2;
			int loopHeight = nextTcHeight;
			int loopX = x + spacing + (boxWidth - loopWidth) / 2;
			int loopY = y + (boxHeight - loopHeight) / 2;

			PhGraphicText gCurrentLoop(_strip.getHUDFont(), loopLabel);

			gCurrentLoop.setRect(loopX, loopY, loopWidth, loopHeight);
			gCurrentLoop.setColor(infoColor);
			gCurrentLoop.draw();
		}

		// Display the next timecode
		{
			/// The next time code will be the next element of the people from the list.
			PhStripText *nextText = NULL;
			if(selectedPeoples.count()) {
				nextText = _strip.doc()->nextText(selectedPeoples, clockTime);
				if(nextText == NULL)
					nextText = _strip.doc()->nextText(selectedPeoples, 0);
			}
			else {
				nextText = _strip.doc()->nextText(clockTime);
				if(nextText == NULL)
					nextText = _strip.doc()->nextText(0);
			}

			PhTime nextTextTime = 0;
			if(nextText != NULL)
				nextTextTime = nextText->timeIn();

			PhGraphicText nextTCText(_strip.getHUDFont());
			nextTCText.setColor(infoColor);

			int nextTcX = x + 2 * spacing + boxWidth;
			int nextTcY = y + (boxHeight - nextTcHeight) / 2;
			nextTCText.setRect(nextTcX, nextTcY, nextTcWidth, nextTcHeight);

			nextTCText.setContent(PhTimeCode::stringFromTime(nextTextTime, _videoEngine.timeCodeType()));
			nextTCText.draw();

			y += boxHeight;
		}
	}

	_strip.draw(0, videoHeight, width, stripHeight, x, y, selectedPeoples);
	foreach(QString info, _strip.infos()) {
		ui->videoStripView->addInfo(info);
	}

	if((_settings->synchroProtocol() == PhSynchronizer::Sony) && (_lastVideoSyncElapsed.elapsed() > 1000)) {
		PhGraphicText errorText(_strip.getHUDFont(), tr("No video sync"));
		errorText.setRect(width / 2 - 100, height / 2 - 25, 200, 50);
		int red = (_lastVideoSyncElapsed.elapsed() - 1000) / 4;
		if (red > 255)
			red = 255;
		errorText.setColor(QColor(red, 0, 0));
		errorText.draw();
	}
}

void JokerWindow::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}

void JokerWindow::on_actionPrevious_loop_triggered()
{
	PhTime time = _doc->previousLoopTime(currentTime());
	if(time > PHTIMEMIN)
		setCurrentTime(time);
}

void JokerWindow::on_actionNext_loop_triggered()
{
	PhTime time = _doc->nextLoopTime(currentTime());
	if(time < PHTIMEMAX)
		setCurrentTime(time);
}

void JokerWindow::on_actionDisplay_the_cuts_toggled(bool checked)
{
	_settings->setDisplayCuts(checked);
}

void JokerWindow::on_actionSet_space_between_two_ruler_graduation_triggered()
{
	RulerSpaceDialog dlg(_settings);
	dlg.exec();
}

void JokerWindow::on_actionDisplay_the_vertical_scale_triggered(bool checked)
{
	_settings->setDisplayVerticalScale(checked);
}

void JokerWindow::setCurrentTime(PhTime time)
{
	_strip.clock()->setTime(time);
	if(_settings->sendMmcMessage())
		_mtcWriter.sendMMCGotoFromTime(time);

}

void JokerWindow::setCurrentRate(PhRate rate)
{
	_strip.clock()->setRate(rate);
	if(_settings->sendMmcMessage()) {
		_mtcWriter.sendMMCGotoFromTime(currentTime());
		if(rate == 0.0f)
			_mtcWriter.sendMMCStop();
		else if(rate == 1.0f)
			_mtcWriter.sendMMCPlay();
	}
}

PhTime JokerWindow::currentTime()
{
	return _strip.clock()->time();
}

PhRate JokerWindow::currentRate()
{
	return _strip.clock()->rate();
}

void JokerWindow::on_actionDisplay_the_control_panel_triggered(bool checked)
{
	_settings->setDisplayControlPanel(checked);
	if(checked)
		fadeInMediaPanel();
	else
		fadeOutMediaPanel();
}

void JokerWindow::on_actionDisplay_the_information_panel_triggered(bool checked)
{
	_settings->setDisplayNextText(checked);
}
