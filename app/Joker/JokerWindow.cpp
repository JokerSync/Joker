/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDatabase>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"

#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(JokerSettings *settings) :
	PhEditableDocumentWindow(settings),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_strip(settings),
	_doc(_strip.doc()),
#ifdef USE_VIDEO
	_videoEngine(settings),
	_secondScreenWindow(NULL),
#endif
	_synchronizer(settings),
#ifdef USE_SONY
	_sonySlave(settings),
#endif
#ifdef USE_LTC
	_ltcReader(settings),
#endif
#ifdef USE_MIDI
	_mtcReader(PhTimeCodeType25),
	_mtcWriter(PhTimeCodeType25),
#endif
	_firstDoc(true),
	_resizingStrip(false),
	_stripTime(0),
	_tcLabelText(""),
	_nextTcLabelText(""),
	_setCurrentTimeToVideoTimeIn(false),
	_syncTimeInToDoc(false),
	_timePlayed(settings->timePlayed()),
	_saving(false)
{
	qmlRegisterType<PhStripDetect>("PhImport", 1, 0, "PhStripDetect");
	qRegisterMetaType<PhStripDetect::PhDetectType>("PhStripDetect::PhDetectType");

	qApp->installEventFilter(this);

	// Setting up UI
	ui->setupUi(this);

	_filteredLineModel = new PhStripFilteredLineModel(this);
	_filteredLineModel->setSourceModel(_doc->lineModel());

	_view = new PhGraphicView();
	_context = _view->engine()->rootContext();

	foreach(QString path, _view->engine()->importPathList()) {
		PHDEBUG << path;
	}

	_context->setContextProperty("doc", _doc);
	_context->setContextProperty("jokerWindow", this);
	_context->setContextProperty("selectedPeopleModel", &_selectedPeopleModel);
	_context->setContextProperty("videoLogoUrl", "/Joker/joker_gray");
	_context->setContextProperty("stripBackgroundUrl", QUrl::fromLocalFile(_settings->backgroundImageLight()));
	_context->setContextProperty("videoSource", &_videoSurface);
	_context->setContextProperty("pipVideoSource", &_pipVideoSurface);
	_context->setContextProperty("secondScreenVideoSource", &_secondScreenVideoSurface);
	_context->setContextProperty("settings", _settings);
	_context->setContextProperty("playbackController", &_mediaPanel);

#ifdef USE_VIDEO
	_context->setContextProperty("videoEngine", &_videoEngine);
#endif

	connect(_view, &QQuickView::statusChanged, this, &JokerWindow::qmlStatusChanged);

	_view->setResizeMode(QQuickView::SizeRootObjectToView);

	_view->setSource(QUrl("qrc:///qml/main.qml"));

	QWidget *container = QWidget::createWindowContainer(_view);
	container->setFocusPolicy(Qt::StrongFocus);
	ui->verticalLayout->addWidget(container);

	QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Bookerly-BoldItalic.ttf");
	QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Cappella-Regular.ttf");

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, &QAction::triggered, this, &JokerWindow::toggleFullScreen);

	connect(&_videoEngine, &PhVideoEngine::timeCodeTypeChanged, this, &JokerWindow::onVideoTimeCodeTypeChanged);

	_videoEngine.registerVideoSurface(&_videoSurface);
	_videoEngine.registerVideoSurface(&_pipVideoSurface);

	this->restoreGeometry(_settings->windowGeometry());
	_mediaPanel.move(this->x() + this->width() / 2 - _mediaPanel.width() / 2,
	                 this->y() + this->height() * 2 / 3);

	_view->setGraphicSettings(_settings);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip.clock());

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
#ifdef USE_VIDEO
	_propertyDialog.setVideoEngine(&_videoEngine);
	_synchronizer.setVideoClock(_videoEngine.clock());
	connect(&_videoEngine, &PhVideoEngine::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
	ui->actionPicture_in_picture->setChecked(_settings->videoPictureInPicture());
	ui->actionSecond_screen->setChecked(_settings->videoSecondScreen());
	if(_settings->videoSecondScreen())
		on_actionSecond_screen_triggered(true);
#else
	ui->actionOpen_Video->setEnabled(false);
	ui->actionClose_video->setEnabled(false);
	ui->actionDeinterlace_video->setEnabled(false);
	ui->actionForce_16_9_ratio->setEnabled(false);
	ui->actionUse_native_video_size->setEnabled(false);
	ui->actionPicture_in_picture->setEnabled(false);
#endif

#ifdef USE_SONY
	connect(&_sonySlave, &PhSonySlaveController::videoSync, this, &JokerWindow::videoSync);
#endif

#ifdef USE_LTC
	connect(&_ltcReader, &PhLtcReader::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
#endif

#ifdef USE_MIDI
	_mtcReader.force24as2398(_settings->mtcForce24as2398());
	connect(&_mtcReader, &PhMidiTimeCodeReader::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
#endif // USE_MIDI

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_strip.clock());
	_mediaPanel.setDropdownEnable(false);
	onTimecodeTypeChanged(this->synchroTimeCodeType());

	ui->actionDisplay_the_control_panel->setChecked(_settings->displayControlPanel());

	ui->actionDisplay_the_information_panel->setChecked(_settings->displayNextText());

	ui->actionTest_mode->setChecked(_settings->stripTestMode());

	ui->actionLoop->setChecked(_settings->syncLooping());

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionDisplay_the_vertical_scale->setChecked(_settings->displayVerticalScale());

	ui->actionDisplay_feet->setChecked(_settings->displayFeet());

	this->connect(_view, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(_view, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::elapse);
	this->connect(_view, &PhGraphicView::beforePaint, this, &JokerWindow::onPaint);

#ifdef USE_VIDEO
	this->connect(&_videoEngine, &PhVideoEngine::opened, this, &JokerWindow::videoFileOpened);
#endif
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

void JokerWindow::onVideoTimeCodeTypeChanged(PhTimeCodeType)
{
	emit timePerFrameChanged();
}

JokerWindow::~JokerWindow()
{
	_mediaPanel.close();

	delete ui;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	// The user will be asked if the document has to be saved
	PhEditableDocumentWindow::closeEvent(event);

	// If the close operation is not cancelled by the user
	if (event->isAccepted()) {
		// The media panel has to be closed manually, or the application
		// will stay open forever in the background
		_mediaPanel.close();

		if(_secondScreenWindow) {
			_secondScreenWindow->close();
			delete _secondScreenWindow;
			_secondScreenWindow = NULL;
		}

		this->disconnect(_view, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
		this->disconnect(_view, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::elapse);
		this->disconnect(_view, &PhGraphicView::paint, this, &JokerWindow::onPaint);

		// Force doc to unmodified to avoid double confirmation
		// since closeEvent is called twice
		// https://bugreports.qt.io/browse/QTBUG-43344
		_doc->setModified(false);

		_settings->setTimePlayed(_timePlayed);
	}
}

void JokerWindow::setupSyncProtocol()
{
	PhClock* clock = NULL;
	QString mtcPortName;

	// Disable old protocol
#ifdef USE_SONY
	_sonySlave.close();
#endif
#ifdef USE_LTC
	_ltcReader.close();
#endif
#ifdef USE_MIDI
	_mtcReader.close();
	_mtcWriter.close();
#endif

	PhSynchronizer::SyncType type = (PhSynchronizer::SyncType)_settings->synchroProtocol();

	switch(type) {
#ifdef USE_SONY
	case PhSynchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open()) {
			clock = _sonySlave.clock();
			onTimecodeTypeChanged((PhTimeCodeType)_settings->sonySlaveCommunicationTimeCodeType());
			emit videoSync();
		}
		else {
			type = PhSynchronizer::NoSync;
			QMessageBox::critical(this, tr("Error"), tr("Unable to connect to USB422v module"));
		}
		break;
#endif
	case PhSynchronizer::LTC:
#ifdef USE_LTC
		_ltcReader.setTimeCodeType((PhTimeCodeType)_settings->ltcReaderTimeCodeType());
		if(_ltcReader.init(_settings->ltcInputPort()))
			clock = _ltcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0")).arg(_settings->ltcInputPort()));
			type = PhSynchronizer::NoSync;
		}
		break;
#endif
#ifdef USE_MIDI
	case PhSynchronizer::MTC:
		if (_settings->mtcInputUseExistingPort()) {
			mtcPortName = _settings->mtcInputPort();
		}
		else {
			mtcPortName = _settings->mtcVirtualInputPort();
		}

		if(_mtcReader.open(mtcPortName))
			clock = _mtcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(mtcPortName));
			type = PhSynchronizer::NoSync;
		}
		break;
#endif
	default:
		type = PhSynchronizer::NoSync;
		break;
	}

#ifdef USE_MIDI
	if(_settings->sendMmcMessage()) {
		if(!_mtcWriter.open(_settings->mmcOutputPort())) {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(_settings->mmcOutputPort()));
			_settings->setSendMmcMessage(false);
		}
	}
#endif

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(const QString &fileName)
{
	QFileInfo info(fileName);
	if(_settings->videoFileType().contains(info.suffix().toLower())) {
#ifdef USE_VIDEO
		return openVideoFile(fileName);
#else
		return false;
#endif
	}

	/// Clear the selected people name list (except for the first document).
	if(!_firstDoc && (fileName != _settings->currentDocument())) {
		_settings->setSelectedPeopleNameList(QStringList());
		_selectedPeopleModel.setStringList(_settings->selectedPeopleNameList());
	} else
		_firstDoc = false;

	if(!_doc->openStripFile(fileName))
		return false;

	/// If the document is opened successfully :
	/// - Update the current document name (settings, windows title)
	PhEditableDocumentWindow::openDocument(fileName);
	if(fileName != _doc->filePath()) {
		PHDEBUG << "Adding to watch: " << _doc->filePath();
		_watcher.addPath(_doc->filePath());
	}

#ifdef USE_VIDEO
	/// - Load the deinterlace settings
	on_actionDeinterlace_video_triggered(_doc->videoDeinterlace());
	/// - Open the corresponding video file if it exists.
	this->checkFilePermission(_doc->videoFilePath());
	_syncTimeInToDoc = true;
	openVideoFile(_doc->videoFilePath());
#endif

	return true;
}

void JokerWindow::saveDocument(const QString &fileName)
{
	setSaving(true);

	// prevent from reloading the document when we are the ones changing it
	bool updateWatcher;
	if (_watcher.files().contains(fileName)) {
		_watcher.removePath(_doc->filePath());
		updateWatcher = true;
	}

	if(_doc->exportDetXFile(fileName, currentTime())) {
		_doc->setModified(false);
		PhEditableDocumentWindow::saveDocument(fileName);

		if (updateWatcher) {
			_watcher.addPath(fileName);
		}

		setSaving(false);
	}
	else {
		if (updateWatcher) {
			_watcher.addPath(fileName);
		}

		setSaving(false);
		QMessageBox::critical(this, "", QString(tr("Unable to save %1")).arg(fileName));
	}
}

void JokerWindow::onExternalChange(const QString &path)
{
	PhTime currentTime = _videoEngine.clock()->time();
	PhDocumentWindow::onExternalChange(path);
	_videoEngine.clock()->setTime(currentTime);
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	/// The event filter catch the following event:
	switch (event->type()) {
	case QEvent::MouseButtonDblClick: /// - Double mouse click toggle fullscreen mode
		if(sender == this)
			toggleFullScreen();
		break;
		// TODO
		// Right mouse click on the video open the video file dialog.
		// Left mouse click on the strip open the strip file dialog
	// Make sure the QML view always gets focus back
	case QEvent::ActivationChange:
	case QEvent::WindowUnblocked:
		if(_view->isActive()) {
			window()->activateWindow();
			return true;
		}
		break;
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

void JokerWindow::onApplicationActivate()
{
	PhDocumentWindow::onApplicationActivate();
	//showMediaPanel();
}

void JokerWindow::onApplicationDeactivate()
{
	PhDocumentWindow::onApplicationDeactivate();
	hideMediaPanel();
}

void JokerWindow::showMediaPanel()
{
	// Don't show the mediaPanel if Joker has not thefocus.
	if(_settings->displayControlPanel()) {
		//_mediaPanel.show();
	}
}

void JokerWindow::hideMediaPanel()
{
	_mediaPanel.hide();
}

void JokerWindow::setCurrentTime(PhTime time)
{
	_strip.clock()->setTime(time);
#ifdef USE_MIDI
	if(_settings->sendMmcMessage())
		_mtcWriter.sendMMCGotoFromTime(time);
#endif

}

void JokerWindow::setCurrentRate(PhRate rate)
{
	_strip.clock()->setRate(rate);
#ifdef USE_MIDI
	if(_settings->sendMmcMessage()) {
		_mtcWriter.sendMMCGotoFromTime(currentTime());
		if(rate == 0.0f)
			_mtcWriter.sendMMCStop();
		else if(rate == 1.0f)
			_mtcWriter.sendMMCPlay();
	}
#endif // USE_MIDI
}

void JokerWindow::onTimecodeTypeChanged(PhTimeCodeType)
{
	PHDEBUG << this->synchroTimeCodeType();
	_mediaPanel.setTimeCodeType(this->synchroTimeCodeType());
}

PhTimeCodeType JokerWindow::localTimeCodeType()
{
#ifdef USE_VIDEO
	return _videoEngine.timeCodeType();
#else
	return _doc->videoTimeCodeType();
#endif

}

PhTimeCodeType JokerWindow::synchroTimeCodeType()
{
	switch (_settings->synchroProtocol()) {
#ifdef USE_SONY
	case PhSynchronizer::Sony:
		return _sonySlave.timeCodeType();
#endif
#ifdef USE_LTC
	case PhSynchronizer::LTC:
		return _ltcReader.timeCodeType();
#endif
#ifdef USE_MIDI
	case PhSynchronizer::MTC:
		return _mtcReader.timeCodeType();
#endif
	default:
		return localTimeCodeType();
	}
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkDocumentModification()) {
		QString filter = tr("Rythmo files") + " (";
		foreach(QString type, _settings->stripFileType())
			filter += "*." + type + " ";
		filter += ")";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			if(!openDocument(fileName))
				QMessageBox::critical(this, "", QString(tr("Unable to open %1")).arg(fileName));
		}
	}
	//showMediaPanel();
}

void JokerWindow::on_actionStep_start_triggered()
{
	setCurrentTime(_doc->videoTimeIn());
	//showMediaPanel();
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
	setCurrentTime(currentTime() + PhTimeCode::timePerFrame(localTimeCodeType()));
}

void JokerWindow::on_actionStep_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - PhTimeCode::timePerFrame(localTimeCodeType()));
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
#ifdef USE_VIDEO
	hideMediaPanel();

	QString lastFolder = _settings->lastVideoFolder();
	QString filter = tr("Movie files") + " (";
	foreach(QString type, _settings->videoFileType())
		filter += "*." + type + " ";
	filter += ");";

	QFileDialog dlg(this, tr("Open a video..."), lastFolder, filter);
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];

		_setCurrentTimeToVideoTimeIn = true;
		openVideoFile(videoFile);
	}
	//showMediaPanel();
#endif
}

#ifdef USE_VIDEO
void JokerWindow::videoFileOpened(bool success)
{
	PHDEBUG << "video file opened";

	if (success) {
		PhTime videoTimeIn = _videoEngine.timeIn();
		QFileInfo lastFileInfo(_doc->videoFilePath());
		QFileInfo fileInfo(_videoEngine.fileName());

		if(videoTimeIn == PHTIMEMAX || videoTimeIn == 0) {
			/* the video itself has no timestamp, and until now we have not
			 * propagated the doc videoTimeIn to the videoEngine */
			videoTimeIn = _doc->videoTimeIn();
			_videoEngine.setTimeIn(videoTimeIn);
			_videoEngine.clock()->setTime(videoTimeIn);

			/* ask the user if he wants to change the video time in */
//			if(fileInfo.fileName() != lastFileInfo.fileName()) {
//				on_actionChange_timestamp_triggered();
//				videoTimeIn = _videoEngine.timeIn();
//			}
			setVideoTimeInToOneHour();
			videoTimeIn = _videoEngine.timeIn();
		}

		if(_videoEngine.fileName() != _doc->videoFilePath()) {
			_doc->setVideoFilePath(_videoEngine.fileName());
			_doc->setVideoTimeIn(videoTimeIn, _videoEngine.timeCodeType());
			_doc->setModified(true);
		}

		_videoEngine.clock()->setTime(videoTimeIn);
		_mediaPanel.setTimeIn(videoTimeIn);
		_mediaPanel.setLength(_videoEngine.length());

		_settings->setLastVideoFolder(fileInfo.absolutePath());

		this->addFilePermission(_doc->videoFilePath());

		if (_setCurrentTimeToVideoTimeIn) {
			setCurrentTime(_doc->videoTimeIn());
			_setCurrentTimeToVideoTimeIn = false;
		}

		if (_syncTimeInToDoc) {
			_videoEngine.setTimeIn(_doc->videoTimeIn());
			_mediaPanel.setTimeIn(_doc->videoTimeIn());
		}
	}

	if (_syncTimeInToDoc) {
		/// - Set the video aspect ratio.
		ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());

		/// - Goto to the document last position.
		setCurrentTime(_doc->lastTime());

		_syncTimeInToDoc = false;
	}
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine.open(videoFile)) {
		return true;
	}
	return false;
}
#endif

void JokerWindow::timeCounter(PhTime elapsedTime)
{
	if(currentRate() == 1 && (PhSynchronizer::SyncType)_settings->synchroProtocol() != PhSynchronizer::NoSync) {
		_timePlayed += elapsedTime;
	}
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();
	setCurrentRate(0);
	PhTime time = _synchronizer.videoClock()->time();

#ifdef USE_VIDEO
	if(_synchronizer.videoClock()->time() < _videoEngine.timeIn())
		time = _videoEngine.timeIn();
	else if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
		time = _videoEngine.timeOut();

	PhTimeCodeDialog dlg(localTimeCodeType(), time);
	if(dlg.exec() == QDialog::Accepted) {
		PhTime timeStamp = 0;
		if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
			timeStamp = dlg.time() - (_videoEngine.length() - PhTimeCode::timePerFrame(localTimeCodeType()));
		else if (_synchronizer.videoClock()->time() < _videoEngine.timeIn())
			timeStamp =  dlg.time();
		else
			timeStamp = _videoEngine.timeIn() + dlg.time() - _synchronizer.videoClock()->time();

		_videoEngine.setTimeIn(timeStamp);
		setCurrentTime(dlg.time());
		_doc->setVideoTimeIn(timeStamp, localTimeCodeType());
		_mediaPanel.setTimeIn(timeStamp);
		_doc->setModified(true);
	}
#endif

	//showMediaPanel();
}

void JokerWindow::setVideoTimeInToOneHour()
{
	setCurrentRate(0);

	PhTime timeStamp = PhTimeCode::timeFromHhMmSsFf(1, 0, 0, 0, localTimeCodeType());

	PHDEBUG << timeStamp;

	_videoEngine.setTimeIn(timeStamp);
	setCurrentTime(timeStamp);
	_doc->setVideoTimeIn(timeStamp, localTimeCodeType());
	_mediaPanel.setTimeIn(timeStamp);
	_doc->setModified(true);
}

bool JokerWindow::saving() const
{
	return _saving;
}

void JokerWindow::setSaving(bool saving)
{
	if (saving != _saving) {
		_saving = saving;
		emit savingChanged();
	}
}

void JokerWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutDialog dlg;
	dlg.setTimePlayed(_timePlayed);
	dlg.exec();

	//showMediaPanel();
}


void JokerWindow::on_actionPreferences_triggered()
{
	hideMediaPanel();
	int oldSynchroProtocol = _settings->synchroProtocol();
#ifdef USE_LTC
	QString oldLtcInputPort = _settings->ltcInputPort();
	PhTimeCodeType oldLtcTimeCodeType = (PhTimeCodeType)_settings->ltcReaderTimeCodeType();
#endif // USE_LTC
#ifdef USE_MIDI
	QString oldMtcInputPort = _settings->mtcInputPort();
	QString oldMtcVirtualInputPort = _settings->mtcVirtualInputPort();
	bool oldMtcInputUseExistingPort = _settings->mtcInputUseExistingPort();
	bool oldSendMmcMessage = _settings->sendMmcMessage();
	QString oldMmcOutputPort = _settings->mmcOutputPort();
#endif // USE_MIDI

	PreferencesDialog dlg(_settings);
	if(dlg.exec() == QDialog::Accepted) {
		if((oldSynchroProtocol != _settings->synchroProtocol())
#ifdef USE_LTC
		   || (oldLtcInputPort != _settings->ltcInputPort())
		   || (oldLtcTimeCodeType != (PhTimeCodeType)_settings->ltcReaderTimeCodeType())
#endif // USE_LTC
#ifdef USE_MIDI
		   || (oldMtcInputPort != _settings->mtcInputPort())
		   || (oldMtcVirtualInputPort != _settings->mtcVirtualInputPort())
		   || (oldMtcInputUseExistingPort != _settings->mtcInputUseExistingPort())
		   || (oldSendMmcMessage != _settings->sendMmcMessage())
		   || (oldMmcOutputPort != _settings->mmcOutputPort())
#endif // USE_MIDI
		   ) {
			PHDEBUG << "Set protocol:" << _settings->synchroProtocol();
			setupSyncProtocol();
		}
#ifdef USE_MIDI
		_mtcReader.force24as2398(_settings->mtcForce24as2398());
#endif // USE_MIDI
	}

	//showMediaPanel();
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

	PhTimeCodeDialog dlg(localTimeCodeType(), currentTime());
	if(dlg.exec() == QDialog::Accepted)
		setCurrentTime(dlg.time());

	//showMediaPanel();
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
	if(!info.exists() || (info.suffix() != "detx"))
		on_actionSave_as_triggered();
	else
		saveDocument(fileName);
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
		if(info.suffix() != "detx")
			fileName = lastFolder + "/" + info.completeBaseName() + ".detx";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName, "*.detx");

	if (!fileName.endsWith(".detx", Qt::CaseInsensitive)) {
		fileName.append(".detx");
	}

	if(fileName != "")
		saveDocument(fileName);
}

bool JokerWindow::isDocumentModified()
{
	return _doc->modified();
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

void JokerWindow::on_actionDisplay_feet_triggered(bool checked)
{
	_settings->setDisplayFeet(checked);
}

void JokerWindow::on_actionSet_first_foot_timecode_triggered()
{
	PhTimeCodeDialog dlg(localTimeCodeType(), _settings->firstFootTime(), this);
	if(dlg.exec())
		_settings->setFirstFootTime(dlg.time());
}

void JokerWindow::on_actionNew_triggered()
{
	_doc->reset();
	on_actionClose_video_triggered();
	this->resetDocument();
}

void JokerWindow::on_actionClose_video_triggered()
{
#ifdef USE_VIDEO
	_videoEngine.close();
#endif
}

void JokerWindow::on_actionSend_feedback_triggered()
{
	hideMediaPanel();

	// do not pass the parent here, it causes a sort of focus loop between the dialog and the parent
	PhFeedbackDialog dlg;
	dlg.exec();
	//showMediaPanel();
}

void JokerWindow::on_actionDeinterlace_video_triggered(bool checked)
{
#ifdef USE_VIDEO
	_videoEngine.setDeinterlace(checked);
	if(checked != _doc->videoDeinterlace()) {
		_doc->setVideoDeinterlace(checked);
		_doc->setModified(true);
	}
#endif
}

void JokerWindow::on_actionHide_the_rythmo_triggered(bool checked)
{
	_settings->setHideStrip(checked);
}

void JokerWindow::onPaint(PhTime elapsedTime)
{
#ifdef USE_VIDEO
	PhClock *clock = _videoEngine.clock();
#else
	PhClock *clock = _strip.clock();
#endif
	PhTime delay = (PhTime)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;
	setStripTime(clockTime);

	PhTime second = 24000;
	_filteredLineModel->setFilterTimeBoundaries(clockTime - 5*second, clockTime + 20*second);

	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	// FIXME the font size for the list of selected peoples is fixed, whereas it depended on the window size before

	// FIXME
	float stripHeightRatio = 0.0f;
	if(!_settings->hideStrip())
		stripHeightRatio = _settings->stripHeight();

#ifdef USE_VIDEO
	// Display the video

	// TODO respect _doc->forceRatio169()
	// TODO respect _settings->hideStrip()

	_videoEngine.decodeVideo();

	// TODO  _settings->videoPictureInPicture()

//	if(_settings->videoPictureInPicture()) {
//		int pipWidth = videoWidth * _settings->videoPictureInPictureRatio();
//		int pipHeight = realVideoHeight * _settings->videoPictureInPictureRatio();
//		int pipX = 0;
//		if(_settings->videoPictureInPicturePositionRight()) {
//			pipX = width - pipWidth;
//			y += pipHeight;
//		}
//		_videoEngine.drawVideo(pipX, 0, pipWidth, pipHeight,
//							   delay + 24 * _settings->videoPictureInPictureOffset());
//}
#endif

	int x = 0;
	int y = 0;
	int width = 100;
	int stripHeight = 100;

	_strip.draw(0, y, width, stripHeight, x, 0, selectedPeoples);

	if (_settings->displayInfo()) {
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
		// The actual painting duration could be measured using a custom QML element.
		// See: http://developer.nokia.com/community/wiki/QML_Performance_Meter
		// (anyway, the QML profiler will provide much more details to the developer.)

		QString stripInfoText;
		foreach(QString info, _strip.infos()) {
			stripInfoText.append(info);
		}
		setStripInfo(stripInfoText);
	}

	PhStripLine *nextLine = NULL;
	if(_settings->displayNextText()) {
		/// The next time code will be the next element of the people from the list.
		if(selectedPeoples.count()) {
			nextLine = _strip.doc()->nextLine(selectedPeoples, clockTime);
			if(nextLine == NULL)
				nextLine = _strip.doc()->nextLine(selectedPeoples, 0);
		}
		else {
			nextLine = _strip.doc()->nextLine(clockTime);
			if(nextLine == NULL)
				nextLine = _strip.doc()->nextLine(0);
		}
	}

	setTcLabelText(PhTimeCode::stringFromTime(clockTime, _videoEngine.timeCodeType()));

	if (nextLine != NULL) {
		setNextTcLabelText(PhTimeCode::stringFromTime(nextLine->timeIn(), _videoEngine.timeCodeType()));
	}
	else {
		setNextTcLabelText("");
	}

	PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
	setCurrentLoopLabel(currentLoop ? currentLoop->label(): "");
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

void JokerWindow::on_actionSet_distance_between_two_feet_triggered()
{
	TimeBetweenTwoFeetDialog dlg(_settings);
	dlg.exec();
}

void JokerWindow::on_actionDisplay_the_vertical_scale_triggered(bool checked)
{
	_settings->setDisplayVerticalScale(checked);
}

PhTime JokerWindow::currentTime()
{
	return _strip.clock()->time();
}

PhRate JokerWindow::currentRate()
{
	return _strip.clock()->rate();
}

QString JokerWindow::nextTcLabelText() const
{
	return _nextTcLabelText;
}

void JokerWindow::setNextTcLabelText(const QString &nextTcLabelText)
{
	if (_nextTcLabelText != nextTcLabelText) {
		_nextTcLabelText = nextTcLabelText;
		emit nextTcLabelTextChanged();
	}
}

QString JokerWindow::tcLabelText() const
{
	return _tcLabelText;
}

void JokerWindow::setTcLabelText(const QString &tcLabelText)
{
	if (_tcLabelText != tcLabelText) {
		_tcLabelText = tcLabelText;
		emit tcLabelTextChanged();
	}
}

JokerSettings *JokerWindow::settings() const
{
	return _settings;
}

void JokerWindow::on_actionDisplay_the_control_panel_triggered(bool checked)
{
	_settings->setDisplayControlPanel(checked);
//	if(checked)
//		showMediaPanel();
//	else
//		hideMediaPanel();
}

void JokerWindow::on_actionDisplay_the_information_panel_triggered(bool checked)
{
	_settings->setDisplayNextText(checked);
}

void JokerWindow::on_actionHide_selected_peoples_triggered(bool checked)
{
	_settings->setHideSelectedPeoples(checked);
}

void JokerWindow::on_actionUse_native_video_size_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setUseNativeVideoSize(checked);
#endif
}

void JokerWindow::on_actionPicture_in_picture_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setVideoPictureInPicture(checked);
#endif
}

PhTime JokerWindow::timePerFrame()
{
	return PhTimeCode::timePerFrame(localTimeCodeType());
}

void JokerWindow::on_actionSet_TC_in_triggered()
{
	_settings->setSyncLoopTimeIn(_synchronizer.time());
}

void JokerWindow::on_actionSet_TC_out_triggered()
{
	_settings->setSyncLoopTimeOut(_synchronizer.time());
}

void JokerWindow::on_actionLoop_triggered(bool checked)
{
	_settings->setSyncLooping(checked);
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
void JokerWindow::on_actionSecond_screen_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setVideoSecondScreen(checked);
	if(checked) {
		_videoEngine.registerVideoSurface(&_secondScreenVideoSurface);

		_secondScreenWindow = new SecondScreenWindow(&_videoEngine, _settings);

		foreach(QString path, _secondScreenWindow->engine()->importPathList()) {
			PHDEBUG << path;
		}

		QQmlContext *context = _secondScreenWindow->engine()->rootContext();
		context->setContextProperty("videoLogoUrl", QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png"));
		context->setContextProperty("secondScreenVideoSource", &_secondScreenVideoSurface);
		context->setContextProperty("settings", _settings);

#ifdef USE_VIDEO
		context->setContextProperty("videoEngine", &_videoEngine);
#endif

		connect(_secondScreenWindow, &QQuickView::statusChanged, this, &JokerWindow::qmlStatusChanged);

		_secondScreenWindow->setResizeMode(QQuickView::SizeRootObjectToView);
		_secondScreenWindow->setSource(QUrl("qrc:///qml/SecondScreen.qml"));
		_secondScreenWindow->show();
		connect(_secondScreenWindow, &SecondScreenWindow::closing, this, &JokerWindow::onSecondScreenClosed);
	}
	else {
		_secondScreenWindow->close();
		delete _secondScreenWindow;
		_secondScreenWindow = NULL;

		_videoEngine.unregisterVideoSurface(&_secondScreenVideoSurface);
	}
#endif
}

void JokerWindow::onSecondScreenClosed(bool closedFromUser)
{
	ui->actionSecond_screen->setChecked(false);
	if(_secondScreenWindow) {
		delete _secondScreenWindow;
		_secondScreenWindow = NULL;
	}
	if(closedFromUser)
		_settings->setVideoSecondScreen(false);
}
