/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef JOKERWINDOW_H
#define JOKERWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QPropertyAnimation>

#include "VideoStripView.h"
#include "PhSync/PhSonySlaveController.h"
#include "PhCommonUI/PhFloatingMediaPanel.h"
#if USE_LTC
#include "PhSync/PhLtcReader.h"
#endif

#include "SonyVideoStripSynchronizer.h"
#include "PropertyDialog.h"

namespace Ui {
class JokerWindow;
}
/**
   *\file JokerWindow.h
 */

/**
 * \brief Joker main application window
 *
 * The JokerWindow class is used to display the rythmo band and the video via the VideoStripView.
 *
 */
class JokerWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * \brief JokerWindow The JokerWindow constructor
	 *
	 * \param settings The application settings
	 */
	explicit JokerWindow(QSettings *settings);
	~JokerWindow();

	/**
	 * \brief Open all supported strip file
	 *
	 * First the file existance is checked then,
	 * If the file is a supported rythmo file, it will call the PhStripDoc function openStripFile().
	 *
	 * \param filePath The file path
	 */
	void openFile(QString filePath);

	/**
	 * \brief Open a video file
	 *
	 * Open a videofile and set the framestamp to the videofile's value or the strip's value if the first one is not usable.
	 *
	 * \param videoFile The videofile path
	 *
	 * \return True if the videoFile opened well, false otherwise.
	 */
	bool openVideoFile(QString videoFile);

protected:
	/**
	 * \brief event Filter
	 *
	 * The event filter allow Joker to react with external controls like when the mouse mouve or when a
	 * file is dropped on the application.
	 *
	 * \param sender The object sending the event
	 * \param event The event
	 * \return The return boolean is :
	 *	- True if we totaly handle the event
	 *  - False if we only want to do pre-treatment.
	 */
	bool eventFilter(QObject *sender, QEvent *event);

	/**
	 * \brief The PhFloatingMediaPanel state enumeration
	 *
	 * The enumeration is used to handle the different state of the PhFloatingMediaPanel:
	 * visible, hidding and hidden for a best fade-in and fade-out effect.
	 */
	enum MediaPanelState {
		MediaPanelVisible,
		MediaPanelHidding,
		MediaPanelHidden
	};


private slots:
	// Qt Designer slots
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionOpen_Video_triggered();

	void on_actionChange_timestamp_triggered();

	void on_actionAbout_triggered();

	void on_actionPreferences_triggered();

	void on_actionClear_list_triggered();


	// Custom slots

	void fadeInMediaPanel();

	void fadeOutMediaPanel();

	void hideMediaPanel();

	void on_actionProperties_triggered();

	void on_actionTest_mode_triggered();

	void on_actionTimecode_triggered();

	void on_actionNext_element_triggered();

	void on_actionPrevious_element_triggered();

	void openRecent();

	void on_actionSave_triggered();

	void on_actionSave_as_triggered();

	void on_actionSelect_character_triggered();

private:
	Ui::JokerWindow *ui;
	PhGraphicStrip * _strip;
	PhVideoEngine * _videoEngine;
	PhStripDoc *_doc;
	QSettings *_settings;
	PhSonySlaveController _sonySlave;
	VideoStripSynchronizer _synchronizer;
#if USE_LTC
	PhLtcReader _ltcReader;
#endif

	PhFloatingMediaPanel _mediaPanel;
	QTimer _mediaPanelTimer;
	MediaPanelState _mediaPanelState;
	QPropertyAnimation _mediaPanelAnimation;

	PropertyDialog _propertyDialog;

	QVector<QAction *> _recentFileButtons;

	bool _needToSave;
	QString _currentStripFile;

	void updateOpenRecent();
	void setupOpenRecentMenu();
	void setupSyncProtocol();
	void closeEvent(QCloseEvent *event);

	void setCurrentStripFile(QString stripFile);

	bool checkSaveFile();
};

#endif // MAINWINDOW_H
