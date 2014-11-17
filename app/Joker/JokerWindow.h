///
/// @file
/// @copyright (C) 2012-2014 Phonations
/// @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
///

#ifndef JOKERWINDOW_H
#define JOKERWINDOW_H

#include <QTimer>
#include <QPropertyAnimation>

#include "PhCommonUI/PhFloatingMediaPanel.h"
#include "PhCommonUI/PhEditableDocumentWindow.h"
#ifdef USE_VIDEO
#include "PhVideo/PhVideoEngine.h"
#endif
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhSynchronizer.h"
#ifdef USE_SONY
#include "PhSony/PhSonySlaveController.h"
#endif
#ifdef USE_LTC
#include "PhLtc/PhLtcReader.h"
#endif
#ifdef USE_MIDI
#include "PhMidi/PhMidiTimeCodeReader.h"
#include "PhMidi/PhMidiTimeCodeWriter.h"
#endif

#include "PropertyDialog.h"
#include "JokerSettings.h"
#include "TimeBetweenTwoFeetDialog.h"

namespace Ui {
class JokerWindow;
}

///
/// @brief Joker main application window
///
/// The JokerWindow class implements the main screen user interface behaviour:
/// - Display the VideoStripView
/// - Handling PhFloatingMediaPanel behaviour
/// - Opening application dialog : preferences, open file, open video file, save , display properties, timestamp, people selection
/// - Display properties dialog
/// - Handling controls command
/// - Connect the application modules: PhVideoEngine, PhGraphicStrip, Synchronizer, PhSonySlaveController, PhLtcReader
///
class JokerWindow : public PhEditableDocumentWindow
{
	Q_OBJECT

public:
	///
	/// @brief JokerWindow The JokerWindow constructor
	///
	/// @param settings The application settings
	///
	explicit JokerWindow(JokerSettings *settings);
	~JokerWindow();

#ifdef USE_VIDEO
	///
	/// @brief Open a video file
	///
	/// Open a videofile and set the framestamp to the videofile's value or the strip's value if the first one is not usable.
	///
	/// @param videoFile The videofile path
	///
	/// @return True if the videoFile opened well, false otherwise.
	///
	bool openVideoFile(QString videoFile);
#endif

public slots:
	///
	/// \brief timeCounter Slot used to count the time played on nominal speed
	/// when the synchro is enabled
	///
	/// \param elapsedTime
	///
	void timeCounter(PhTime elapsedTime);

protected:
	///
	/// @brief Close event, if accepted by the user, will close the media panel
	/// @param event
	///
	virtual void closeEvent(QCloseEvent *event);

	///
	/// @brief Open all supported strip file
	///
	/// @param filePath The file path
	///
	bool openDocument(const QString &filePath);

	virtual void  onExternalChange(const QString &path);

	///
	/// @brief Custom event filter
	///
	/// @param sender The event sender
	/// @param event The event
	/// @return True if handled, false otherwise
	///
	bool eventFilter(QObject *sender, QEvent *event);

	///
	/// @brief Give the ui->menuOpen_recent item to PhDocumentWindow
	///
	/// PhDocumentWindow will fill the submenu item with the
	/// last document opened when calling setCurrentDocument().
	/// The max number of item is the PhDocumentWindowSettings::maxRecentDocument().
	///
	/// @return A reference to the menu item
	///
	QMenu *recentDocumentMenu();

	///
	/// @brief Give the ui->actionFullscreen to PhWindow
	///
	/// PhWindow will make it checkable and check and uncheck it
	/// each time the user toggle between fullscreen and normal.
	/// The action must be connected in JokerWindow constructor.
	/// @todo connect the action with PhWindow
	/// @return A reference to the action
	///
	QAction *fullScreenAction();

	///
	/// \brief Show the control panel on application activation
	///
	void onApplicationActivate();

	///
	/// \brief Hide the control panel on application deactivation
	///
	void onApplicationDeactivate();

	///
	/// @brief Setup the synchronisation protocol
	///
	/// Close all the protocol if opened and setup the one specified
	/// by the settings.
	/// If it failed to open a protocol, go to the NoSync mode (and store
	/// it in the settings).
	/// The PhMediaPanel slider is enabled only in the NoSync mode.
	///
	void setupSyncProtocol();

	///
	/// @brief Check if the current document has been modified.
	///
	bool isDocumentModified();

private slots:
	// Custom slots

	void showMediaPanel();

	void hideMediaPanel();

	void onPaint(int width, int height);

	void onVideoSync();

	void setCurrentTime(PhTime time);

	void setCurrentRate(PhRate rate);

	void onTimecodeTypeChanged(PhTimeCodeType tcType);

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

	void on_actionProperties_triggered();

	void on_actionTest_mode_triggered();

	void on_actionTimecode_triggered();

	void on_actionNext_element_triggered();

	void on_actionPrevious_element_triggered();

	void on_actionSave_triggered();

	void on_actionSave_as_triggered();

	void on_actionSelect_character_triggered();

	void on_actionForce_16_9_ratio_triggered(bool checked);

	void on_actionInvert_colors_toggled(bool checked);

	void on_actionDisplay_feet_triggered(bool checked);

	void on_actionSet_first_foot_timecode_triggered();

	void on_actionSet_distance_between_two_feet_triggered();

	void on_actionNew_triggered();

	void on_actionClose_video_triggered();

	void on_actionSend_feedback_triggered();

	void on_actionDeinterlace_video_triggered(bool checked);

	void on_actionHide_the_rythmo_triggered(bool checked);

	void on_actionPrevious_loop_triggered();

	void on_actionNext_loop_triggered();

	void on_actionDisplay_the_cuts_toggled(bool checked);

	void on_actionDisplay_the_vertical_scale_triggered(bool checked);

	void on_actionDisplay_the_control_panel_triggered(bool checked);

	void on_actionDisplay_the_information_panel_triggered(bool checked);

	void on_actionHide_selected_peoples_triggered(bool checked);

	void on_actionUse_native_video_size_triggered(bool checked);

	void on_actionSet_TC_in_triggered();

	void on_actionSet_TC_out_triggered();

	void on_actionLoop_triggered(bool checked);

private:
	PhTimeCodeType timeCodeType();
	PhTime currentTime();
	PhRate currentRate();

	Ui::JokerWindow *ui;
	JokerSettings *_settings;
	PhGraphicStrip _strip;
	PhStripDoc *_doc;
#ifdef USE_VIDEO
	PhVideoEngine _videoEngine;
#endif
	PhSynchronizer _synchronizer;
#ifdef USE_SONY
	PhSonySlaveController _sonySlave;
#endif
#ifdef USE_LTC
	PhLtcReader _ltcReader;
#endif
#ifdef USE_MIDI
	PhMidiTimeCodeReader _mtcReader;
	PhMidiTimeCodeWriter _mtcWriter;
#endif

	PhFloatingMediaPanel _mediaPanel;

	PropertyDialog _propertyDialog;

	bool _firstDoc;
	bool _resizingStrip;

	PhGraphicImage _videoLogo;

	QTime _lastVideoSyncElapsed;
};

#endif // MAINWINDOW_H
