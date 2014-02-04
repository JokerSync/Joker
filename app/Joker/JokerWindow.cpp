#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFileOpenEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFontDialog.h"
#include "AboutMenu.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(QSettings *settings) :
	QMainWindow(NULL),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, settings),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_needToSave(false),
	_currentStripFile(""),
	_ltcReader()
{
	// Setting up UI
	ui->setupUi(this);

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	setupOpenRecentMenu();

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

	if(_settings->value("stripTestMode").toBool())
	{
#warning TODO do we warn the user that test mode is on?
		ui->actionTest_mode->setChecked(true);
	}

#warning TODO fix fullscreen on startup
	if(_settings->value("startFullScreen", false).toBool())
		this->showFullScreen();

	// This is for the drag and drop feature
	setAcceptDrops(true);

}

JokerWindow::~JokerWindow()
{
	delete ui;
}

void JokerWindow::openRecent()
{
	if(!checkSaveFile())
		return;

	// This slot is triggered by dynamic action
	openFile(sender()->objectName());
}

void JokerWindow::updateOpenRecent()
{
	if(!ui->menuOpen_recent->isEnabled())
		ui->menuOpen_recent->setEnabled(true);

	QAction * menu = QObject::findChild<QAction *>(_doc->getFilePath(), Qt::FindDirectChildrenOnly);
	// If the object already belong to the list, set it on top
	if(menu)
	{
		ui->menuOpen_recent->removeAction(menu);
		ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().first(), menu);
	}
	// Else, add it
	else
	{
		// Set the corresponding button
		QAction * action = new QAction(_doc->getFilePath(), this);
		// Set the ObjectName, very important for openRecent()
		action->setObjectName(_doc->getFilePath());
		connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));
		// Insert it in the button list
		_recentFileButtons.insert(0, action);
		// Insert it at the first place on the menu
		ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().first(), action);
	}

	// Open the settings group of recent files
	_settings->beginGroup("openRecent");
	int i = 1;
	// Rewrite the setting files from the menu items
	foreach(QAction * action, ui->menuOpen_recent->actions())
	{
		// Break if the separator or the max number is reached
		if(action->isSeparator() or i > 10)
			break;
		// Write the setting
		_settings->setValue(QString::number(i), action->objectName());
		i++;
	}
	// Close the setting group
	_settings->endGroup();
}

void JokerWindow::setupOpenRecentMenu()
{
	// Open the settings group of recent files
	_settings->beginGroup("openRecent");
	// List them
	QStringList recentList = _settings->childKeys();
	foreach(QString recentFileIndex, recentList)
	{
		QString fileName = _settings->value(recentFileIndex, "empty").toString();
		QAction * action = new QAction(fileName, this);
		action->setObjectName(fileName);
		connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));
		_recentFileButtons.append(action);
	}

	// Add all the actions to the menu
	ui->menuOpen_recent->insertActions(ui->menuOpen_recent->actions().last(), _recentFileButtons.toList());
	// Add a separator just above the "clear list" menu
	ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().last(), ui->menuOpen_recent->addSeparator());
	// Close the group
	_settings->endGroup();

}

void JokerWindow::setupSyncProtocol()
{
	PhClock* clock = NULL;

	// Disable old protocol
	_sonySlave.close();
	_ltcReader.close();
	VideoStripSynchronizer::SyncType type = (VideoStripSynchronizer::SyncType)_settings->value("synchroProtocol").toInt();

	switch(type)
	{
	case VideoStripSynchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open())
		{
			clock = _sonySlave.clock();
			ui->videoStripView->setSony(&_sonySlave);
		}
		else
		{
			type = VideoStripSynchronizer::NoSync;
			QMessageBox::critical(this, "", "Unable to connect to USB422v module");
		}
		break;
	case VideoStripSynchronizer::LTC:
		{
			QString input = _settings->value("ltcInputDevice").toString();
			if(_ltcReader.init(input))
				clock = _ltcReader.clock();
			else
			{
				QMessageBox::critical(this, "", "Unable to open " + input);
				type = VideoStripSynchronizer::NoSync;
			}
			break;
		}
	}

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setValue("synchroProtocol", type);
}

void JokerWindow::openFile(QString fileName)
{
	hideMediaPanel();

	// Checking if the file exists
	if(QFile::exists(fileName))
	{
		if(_doc->openStripFile(fileName))
		{
			setCurrentStripFile(fileName);

			// Opening the corresponding video file if it exists
			if(openVideoFile(_doc->getVideoPath()))
			{
				PhFrame frameStamp = _doc->getVideoTimestamp();
				_videoEngine->setFrameStamp(frameStamp);
				_mediaPanel.setFirstFrame(frameStamp);
			}
			else
				_videoEngine->close();

			// On succeed, synchronizing the clocks
			_strip->clock()->setTimeCodeType(_doc->getTCType());
			_strip->clock()->setFrame(_doc->getLastFrame());

			_needToSave = false;
		}
	}
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	switch (event->type())
	{
	case QEvent::FileOpen:
	{
		QString filePath = static_cast<QFileOpenEvent *>(event)->file();
		QString fileType = filePath.split(".").last().toLower();
		// As the plist file list all the supported format (which are .strip, .detx, .avi & .mov)
		// if the file is not a strip or a detx file, it's a video file, we don't need any protection
		if(fileType == "detx" or fileType == "strip")
		{
			if(checkSaveFile())
				openFile(filePath);
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
		if(this->hasFocus() and _settings->value("synchroProtocol", VideoStripSynchronizer::NoSync).toInt() == 0)
			fadeInMediaPanel();
		break;

	case QEvent::Drop:
	{
		const QMimeData* mimeData = static_cast<QDropEvent *>(event)->mimeData();

		// If there is one file (not more) we open it
		if (mimeData->urls().length() == 1)
		{
			QString filePath = mimeData->urls().first().toLocalFile();
			QString fileType = filePath.split(".").last().toLower();
			if(fileType == "detx" or fileType == "strip")
			{
				if(checkSaveFile())
					openFile(filePath);
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
		if(sender->objectName() == this->objectName() and !_videoEngine->fileName().length())
		{
			// It's useless to check for the x position because if it's out of the bounds, the sender will not be "this"
			if(QCursor::pos().y() > this->pos().y() and QCursor::pos().y() < this->pos().y() + this->height() * (1.0 - _settings->value("stripHeight", 0.25f).toFloat()))
				on_actionOpen_Video_triggered();
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	if(!checkSaveFile())
		event->ignore();
}

void JokerWindow::setCurrentStripFile(QString stripFile)
{
	_currentStripFile = stripFile;
	this->setWindowTitle(stripFile);
	_settings->setValue("lastFile", stripFile);
	_settings->setValue("lastFolder", QFileInfo(stripFile).absolutePath());

	updateOpenRecent();
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkSaveFile())
	{
		QString filter = tr("DetX files") + " (*.detx);; "
				+ tr("Joker files") + " (*.strip);; "
				+ tr("Rythmo files") + " (*.detx *.strip);; "
				+ tr("All files") + " (*.*)";
		QFileDialog dlg(this, tr("Open..."), _settings->value("lastFolder", QDir::homePath()).toString(), filter);

		dlg.selectNameFilter(_settings->value("selectedFilter", "Rythmo files (*.detx *.strip)").toString());
		dlg.setOption(QFileDialog::HideNameFilterDetails, false);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec())
		{
			QString fileName = dlg.selectedFiles()[0];
			openFile(fileName);
			_settings->setValue("selectedFilter", dlg.selectedNameFilter());
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

	QString lastFolder = _settings->value("lastVideoFolder", QDir::homePath()).toString();
	QFileDialog dlg(this, tr("Open..."), lastFolder, tr("Movie files") + " (*.avi *.mov)");
	if(dlg.exec())
	{
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
	if (fileInfo.exists() && _videoEngine->open(videoFile))
	{
		PhFrame frameStamp = _videoEngine->frameStamp();

		_mediaPanel.setFirstFrame(frameStamp);
		_mediaPanel.setMediaLength(_videoEngine->length());

		if(videoFile != _doc->getVideoPath())
		{
			_doc->setVideoPath(videoFile);
			if(frameStamp > 0)
				_doc->setVideoTimestamp(frameStamp);
			_needToSave = true;
		}

		if(frameStamp == 0)
		{
			frameStamp = _doc->getVideoTimestamp();
			_videoEngine->setFrameStamp(frameStamp);
			_videoEngine->clock()->setFrame(frameStamp);
			if(fileInfo.fileName() != lastFileInfo.fileName())
			{
				on_actionChange_timestamp_triggered();
				frameStamp = _videoEngine->frameStamp();
			}
		}

		_videoEngine->clock()->setFrame(frameStamp);

		_settings->setValue("lastVideoFolder", fileInfo.absolutePath());
		return true;
	}
	return false;
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _synchronizer.videoClock()->frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = _videoEngine->frameStamp();
		frameStamp += dlg.frame() - _synchronizer.videoClock()->frame();
		_videoEngine->setFrameStamp(frameStamp);
		_strip->clock()->setFrame(dlg.frame());
		_doc->setVideoTimestamp(frameStamp);
		_needToSave = true;
	}

	fadeInMediaPanel();
}



void JokerWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutMenu menu;
	menu.exec();

	fadeInMediaPanel();
}


void JokerWindow::on_actionPreferences_triggered()
{
	hideMediaPanel();
    int syncProtocol = _settings->value("synchroProtocol").toInt();
    QString inputLTC = _settings->value("ltcInputDevice").toString();
	PreferencesDialog dlg(_settings);
    dlg.exec();
    if(syncProtocol != _settings->value("synchroProtocol").toInt() or inputLTC != _settings->value("ltcInputDevice", ""))
    {
        PHDEBUG << "Set protocol:" << _settings->value("synchroProtocol").toInt();
        setupSyncProtocol();
    }

	fadeInMediaPanel();
}

void JokerWindow::fadeInMediaPanel()
{
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
	if(_mediaPanel.underMouse() or _mediaPanel.isMousePressed())
    {
		PHDEBUG << "Don't hide";
        _mediaPanelTimer.start(3000);
        return;
    }

	PHDEBUG << _mediaPanelState;
	switch(_mediaPanelState)
	{
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
	if(_settings->value("stripTestMode", false).toBool())
		_settings->setValue("stripTestMode", false);
	else
		_settings->setValue("stripTestMode", true);
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
	_settings->beginGroup("openRecent");
	//List all keys
	QStringList indexes = _settings->allKeys();
	//Remove them from
	foreach(QString index, indexes)
		_settings->remove(index);

	//Close the group
	_settings->endGroup();

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
	_recentFileButtons.clear();
	ui->menuOpen_recent->setEnabled(false);
}

void JokerWindow::on_actionSave_triggered()
{
	QFileInfo info(_currentStripFile);
	if(!info.exists() || (info.suffix() != "strip"))
		on_actionSave_as_triggered();
	else if(_doc->saveStrip(_currentStripFile, _strip->clock()->timeCode()))
		_needToSave = false;
	else
		QMessageBox::critical(this, "", tr("Unable to save ") + _currentStripFile);
}

void JokerWindow::on_actionSave_as_triggered()
{
	hideMediaPanel();

	QString stripFile = _currentStripFile;
	QString lastFolder = _settings->value("lastFolder", QDir::homePath()).toString();
	// If there is no current strip file, ask for a name
	if(stripFile == "")
		stripFile = lastFolder;
	else
	{
		QFileInfo info(stripFile);
		if(info.suffix() != "strip")
			stripFile = lastFolder + "/" + info.completeBaseName() + ".strip";
	}

	stripFile = QFileDialog::getSaveFileName(this, tr("Save..."), stripFile,"*.strip");
	if(stripFile != "")
	{
		if(_doc->saveStrip(stripFile, _strip->clock()->timeCode()))
		{
			_needToSave = false;
			setCurrentStripFile(stripFile);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + stripFile);
	}
}

bool JokerWindow::checkSaveFile()
{
	if(_needToSave)
	{
		QString msg = tr("Do you want to save your changes ?");
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		switch(box.exec())
		{
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
