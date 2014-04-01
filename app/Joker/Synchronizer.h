/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

/**
 * @brief Provide a synchronisation system between the strip, the video and the external sync signal
 */
class Synchronizer : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief The different kind of synchronization enumeration used by Joker
	 */
	enum SyncType {
		NoSync = 0,
		Sony = 1,
#if USE_LTC
		LTC = 2,
#endif
	};

	Synchronizer();

	/**
	 * @brief Set the strip clock
	 * @param clock The strip clock
	 */
	void setStripClock(PhClock *clock);

	/**
	 * @brief Get the strip clock
	 * @return The strip clock
	 */
	PhClock * stripClock() {
		return _stripClock;
	}

	/**
	 * @brief Set the videoClock
	 * @param clock The video clock
	 */
	void setVideoClock(PhClock *clock);

	/**
	 * @brief Get the video clock
	 * @return The video clock
	 */
	PhClock * videoClock() {
		return _videoClock;
	}

	/**
	 * @brief Set the synchronization clock
	 * @param clock The synchronization clock
	 * @param type The desired PhTimeCodeType
	 */
	void setSyncClock(PhClock *clock, SyncType type);

	/**
	 * @brief Get the Synchronization clock
	 * @return The Synchronization clock.
	 */
	PhClock * syncClock() {
		return _syncClock;
	}

signals:
	void correction(PhTime delta);

private slots:
	void onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onStripRateChanged(PhRate rate);
	void onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onVideoRateChanged(PhRate rate);
	void onVideoTCTypeChanged(PhTimeCodeType tcType);
	void onSyncFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onSyncRateChanged(PhRate rate);
private:
	int _syncType;
	PhClock * _stripClock;
	PhClock * _videoClock;
	PhClock * _syncClock;
	bool _settingStripFrame;
	bool _settingVideoFrame;
	bool _settingSonyFrame;
	bool _settingStripRate;
	bool _settingVideoRate;
	bool _settingSonyRate;
};

#endif // SYNCHRONIZER_H
