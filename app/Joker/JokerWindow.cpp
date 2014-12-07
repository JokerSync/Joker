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
#include <QWindowStateChangeEvent>
#include <QMouseEvent>
#include <QQmlEngine>
#include <QQuickView>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"
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
	_sonySlave(PhTimeCodeType25, settings),
	_mtcReader(PhTimeCodeType25),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_firstDoc(true),
	_resizingStrip(false),
	_numberOfDraw(0),
	_stripTime(0)
{
	qApp->installEventFilter(this);

	// Setting up UI
	ui->setupUi(this);

	//_view = ui->videoStripView;
	_view = new PhGraphicView();
	_context = _view->engine()->rootContext();

	_context->setContextProperty("doc", _doc);
	_context->setContextProperty("jokerWindow", this);
	_context->setContextProperty("selectedPeopleModel", &_selectedPeopleModel);
	_context->setContextProperty("nextPeopleModel", _strip.nextPeopleModel());
	_context->setContextProperty("rulerModel", _strip.rulerModel());
	_context->setContextProperty("cutModel", _strip.cutModel());
	_context->setContextProperty("loopModel", _strip.loopModel());
	_context->setContextProperty("trackModel", _strip.trackModel());
	_context->setContextProperty("verticalTimePerPixel", _settings->verticalTimePerPixel());
	_context->setContextProperty("horizontalTimePerPixel", _settings->horizontalTimePerPixel());
	_context->setContextProperty("textFontUrl", QUrl::fromLocalFile(_settings->textFontFile()));
	_context->setContextProperty("textBoldness", _settings->textBoldness());
	_context->setContextProperty("cutWidth", _settings->cutWidth());
	_context->setContextProperty("displayCuts", _settings->displayCuts());
	_context->setContextProperty("invertColor", _settings->invertColor());
	_context->setContextProperty("displayRuler", _settings->displayRuler());
	_context->setContextProperty("videoLogoUrl", QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png"));
	_context->setContextProperty("stripBackgroundUrl", QUrl::fromLocalFile(_settings->backgroundImageLight()));
	_context->setContextProperty("videoSource", &_videoSurface);

	// the following are updated in onPaint. They should probably be properties with signals instead
	_context->setContextProperty("selectedPeopleListVisible", _settings->displayNextTC() && _settings->selectedPeopleNameList().count());
	_context->setContextProperty("titleRectVisible", _settings->displayTitle());
	_context->setContextProperty("videoLogoVisible", (_videoEngine.height() <= 0) && _settings->displayLogo());
	_context->setContextProperty("tcLabelVisible", _settings->displayTC());
	_context->setContextProperty("tcLabelText", "#");
	_context->setContextProperty("infosVisible", _settings->displayInfo());
	_context->setContextProperty("nextTcLabelVisible", _settings->displayNextTC());
	_context->setContextProperty("nextTcLabelText", "#");
	_context->setContextProperty("noSyncLabelVisible", false);
	_context->setContextProperty("noSyncLabelOpacity", 0);

	connect(_view, &QQuickView::statusChanged, this, &JokerWindow::qmlStatusChanged);

	//view->setResizeMode(QQuickWidget::SizeRootObjectToView);
	_view->setResizeMode(QQuickView::SizeRootObjectToView);

	// in resources or in full path, the qml sub-files are not found if launched from outside Qt Creator !
	//_view->setSource(QUrl("qrc:///Phonations/Joker/main.qml"));
	_view->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/main.qml"));
	_view->show();

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	connect(&_videoEngine, &PhVideoEngine::newVideoContentProduced, &_videoSurface, &PhVideoSurface::onNewVideoContentReceived);

	_view->setGraphicSettings(_settings);

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

	this->setFocus();

	if(_settings->stripTestMode()) {
#warning /// @todo do we warn the user that test mode is on?
		ui->actionTest_mode->setChecked(true);
	}

#warning /// @todo move to PhDocumentWindow
	// This is for the drag and drop feature
	setAcceptDrops(true);

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionShow_ruler->setChecked(_settings->displayRuler());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);

	this->connect(_view, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(_view, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::tick);
	this->connect(_view, &PhGraphicView::beforePaint, this, &JokerWindow::onPaint);
}

void JokerWindow::qmlStatusChanged(QQuickView::Status status)
{
	if (status == QQuickView::Error)
	{
		foreach (QQmlError error, _view->errors())
		{
			PHDEBUG << error.toString();

			QMessageBox msgBox;
			msgBox.setText(error.toString());
			msgBox.exec();
		}
	}
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
			QMessageBox::critical(this, tr("Error"), "Unable to connect to USB422v module");
		}
		break;
	case PhSynchronizer::LTC:
		{
			QString input = _settings->ltcInputDevice();
			if(_ltcReader.init(input))
				clock = _ltcReader.clock();
			else {
				QMessageBox::critical(this, tr("Error"), "Unable to open " + input);
				type = PhSynchronizer::NoSync;
			}
			break;
		}
	case PhSynchronizer::MTC:
		if(_mtcReader.open(_settings->midiTimeCodePortName()))
			clock = _mtcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to create \"%1\" midi port")).arg(_settings->midiTimeCodePortName()));
			type = PhSynchronizer::NoSync;
		}
	case PhSynchronizer::NoSync:
		break;
	}

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(QString fileName)
{
	/// Clear the selected people name list (except for the first document).
	if(!_firstDoc) {
		_settings->setSelectedPeopleNameList(QStringList());
		_selectedPeopleModel.setStringList(_settings->selectedPeopleNameList());
	} else
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
	_strip.clock()->setTime(_doc->lastTime());
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
		{
			fadeInMediaPanel();

			break;

			// FIXME
			// do this from QML

			// Check if it is near the video/strip border
			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			if(_resizingStrip) {
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
				if(mouseEvent->buttons() & Qt::LeftButton)
					_settings->setStripHeight(1.0 - ((float) mouseEvent->pos().y() /(float) this->height()));
			}
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);
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
		_resizingStrip = false;
		if(sender == this)
			toggleFullScreen();
		break;
	case QEvent::MouseButtonRelease:
		QApplication::setOverrideCursor(Qt::ArrowCursor);
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

			break;
			// FIXME
			// do this from QML

			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10)) {
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
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
	if(_strip.clock()->rate() == 0.0)
		_strip.clock()->setRate(1.0);
	else
		_strip.clock()->setRate(0.0);
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() + PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() - PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	_strip.clock()->setRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	_strip.clock()->setRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	_strip.clock()->setRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	_strip.clock()->setRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	_strip.clock()->setRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	_strip.clock()->setRate(3.0);
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
			_strip.clock()->setTime(_doc->videoTimeIn());
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
	if(_strip.clock()->rate() == 1 && (PhSynchronizer::SyncType)_settings->synchroProtocol() != PhSynchronizer::NoSync) {
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
	_strip.clock()->setRate(0);
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
		_strip.clock()->setTime(dlg.time());
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
	// Don't show the mediaPanel if Joker has not thefocus.
	//if(!(this->hasFocus() || ui->videoStripView->hasFocus()))
	if(!this->hasFocus())
		return;
	// Don't show the mediaPanel if Joker is remote controled.
	if(_settings->synchroProtocol() != PhSynchronizer::NoSync)
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

	PhTimeCodeDialog dlg(_videoEngine.timeCodeType(), _strip.clock()->time());
	if(dlg.exec() == QDialog::Accepted)
		_strip.clock()->setTime(dlg.time());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhTime time = _doc->nextElementTime(_strip.clock()->time());
	if(time < PHTIMEMAX)
		_strip.clock()->setTime(time);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhTime time = _doc->previousElementTime(_strip.clock()->time());
	if(time > PHTIMEMIN)
		_strip.clock()->setTime(time);
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
	else if(_doc->saveStripFile(fileName, _strip.clock()->time()))
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
		if(_doc->saveStripFile(fileName, _strip.clock()->time())) {
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

	PeopleDialog dlg(this, _doc, _settings, &_selectedPeopleModel);

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

//void JokerWindow::onPaint(int width, int height)
void JokerWindow::onPaint(PhTimeScale frequency)
{
	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	// FIXME the font size for the list of selected peoples is fixed, whereas it depended on the window size before
	_context->setContextProperty("selectedPeopleListVisible", _settings->displayNextTC() && _settings->selectedPeopleNameList().count());
	_context->setContextProperty("titleRectVisible", _settings->displayTitle() && (_strip.doc()->fullTitle().length() > 0));

	// FIXME
//	float stripHeightRatio = 0.25f;
//	if(_settings) {
//		if(_settings->hideStrip())
//			stripHeightRatio = 0;
//		else
//			stripHeightRatio = _settings->stripHeight();
//	}

	_videoEngine.decodeVideo();

//	if((videoHeight > 0)) {
//		int tcWidth = 200 * this->devicePixelRatio();
//		if(_videoEngine.height() > 0) {
//			int videoWidth;
//			if(_doc->forceRatio169())
//				videoWidth = videoHeight * 16 / 9;
//			else
//				videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();

//			if(videoWidth > width) {
//				videoWidth = width;
//			}
//			int videoX = (width - videoWidth) / 2;
//			// adjust tc size
//			// FIXME this does not work now that QML is used for the timecode labels.
//			if(videoX > tcWidth) {
//				// maximize the timecode if we have more room
//				tcWidth = videoX;
//			} else if( width < 2 * tcWidth) {
//				// make sure the timecode labels do not overlap
//				tcWidth = width / 2;
//			}
//		}
//	}

	PhClock *clock = _videoEngine.clock();
	long delay = (int)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;
	setStripTime(clockTime);

	int tcOffset = 0;
	int y = 0;
	int width = 100;
	int stripHeight = 100;

	_strip.draw(0, y, width, stripHeight, tcOffset, selectedPeoples);

	// prepare the string list that is used to display the infos
	setRefreshInfo(QString("refresh: %1x%2, %3 / %4")
				   .arg(_view->width())
				   .arg(_view->height())
				   .arg(_view->maxRefreshRate())
				   .arg(_view->refreshRate()));
	setUpdateInfo(QString("Update : %1 %2").arg(_view->maxUpdateDuration())
										.arg(_view->lastUpdateDuration()));
	setDropInfo(QString("drop: %1 %2").arg(_view->dropDetected()).arg(_view->secondsSinceLastDrop()));
	#warning /// @todo measure fps with a custom QML element
	// The actual painting duration should be measured using a custom QML element.
	// See: http://developer.nokia.com/community/wiki/QML_Performance_Meter
	// (anyway, the QML profiler will provide much more details to the developer.)

	QString stripInfoText;
	foreach(QString info, _strip.infos()) {
		stripInfoText.append(info);
	}
	setStripInfo(stripInfoText);

	PhStripText *nextText = NULL;
	if(_settings->displayNextTC()) {
		/// The next time code will be the next element of the people from the list.
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
	}

	_context->setContextProperty("videoLogoVisible", (_videoEngine.height() <= 0) && _settings->displayLogo());
	_context->setContextProperty("tcLabelVisible", _settings->displayTC());
	_context->setContextProperty("tcLabelText", PhTimeCode::stringFromTime(clockTime, _videoEngine.timeCodeType()));
	_context->setContextProperty("infosVisible", _settings->displayInfo());

	_context->setContextProperty("nextTcLabelVisible", _settings->displayNextTC() && nextText != NULL);
	if (nextText != NULL) {
		_context->setContextProperty("nextTcLabelText", PhTimeCode::stringFromTime(nextText->timeIn(), _videoEngine.timeCodeType()));
	}

	PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
	setCurrentLoopLabel(currentLoop ? currentLoop->label(): "");

	_context->setContextProperty("noSyncLabelVisible", _lastVideoSyncElapsed.elapsed() > 1000);
	double opacity = (_lastVideoSyncElapsed.elapsed() - 1000.0d) / 1000.0d;
	_context->setContextProperty("noSyncLabelOpacity", opacity <= 0 ? 0 : opacity >= 1 ? 1 : opacity);
}

void JokerWindow::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}

void JokerWindow::on_actionPrevious_loop_triggered()
{
	PhTime time = _doc->previousLoopTime(_strip.clock()->time());
	if(time > PHTIMEMIN)
		_strip.clock()->setTime(time);
}

void JokerWindow::on_actionNext_loop_triggered()
{
	PhTime time = _doc->nextLoopTime(_strip.clock()->time());
	if(time < PHTIMEMAX)
		_strip.clock()->setTime(time);
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

QString JokerWindow::currentLoopLabel()
{
	return _currentLoopLabel;
}

void JokerWindow::setCurrentLoopLabel(QString label)
{
	if (label != _currentLoopLabel) {
		_currentLoopLabel = label;
		emit currentLoopLabelChanged();
	}
}

PhTime JokerWindow::stripTime()
{
	return _stripTime;
}

void JokerWindow::setStripTime(PhTime time)
{
	if (time != _stripTime) {
		_stripTime = time;
		emit stripTimeChanged();
	}
}

void JokerWindow::setRefreshInfo(QString text)
{
	if (text != _refreshInfo) {
		_refreshInfo = text;
		emit refreshInfoChanged();
	}
}

void JokerWindow::setUpdateInfo(QString text)
{
	if (text != _updateInfo) {
		_updateInfo = text;
		emit updateInfoChanged();
	}
}

void JokerWindow::setDropInfo(QString text)
{
	if (text != _dropInfo) {
		_dropInfo = text;
		emit dropInfoChanged();
	}
}

void JokerWindow::setStripInfo(QString text)
{
	if (text != _stripInfo) {
		_stripInfo = text;
		emit stripInfoChanged();
	}
}
