/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSYNCHRONIZER_H
#define PHSYNCHRONIZER_H

#include <QObject>

#include "PhSync/PhClock.h"

/**
 * @brief Provide a synchronisation system between the strip, the video and the external sync signal
 */
class PhSynchronizer : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief The different kind of synchronization enumeration used by Joker
	 */
	enum SyncType {
		NoSync = 0,
		Sony = 1,
		LTC = 2,
		MTC = 3,
	};

	PhSynchronizer();

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

private slots:
	void onStripTimeChanged(PhTime time);
	void onStripRateChanged(PhRate rate);
	void onVideoTimeChanged(PhTime time);
	void onVideoRateChanged(PhRate rate);
	void onSyncTimeChanged(PhTime time);
	void onSyncRateChanged(PhRate rate);
private:
	int _syncType;
	PhClock * _stripClock;
	PhClock * _videoClock;
	PhClock * _syncClock;
	bool _settingStripTime;
	bool _settingVideoTime;
	bool _settingSonyTime;
	bool _settingStripRate;
	bool _settingVideoRate;
	bool _settingSonyRate;
};

#endif // PHSYNCHRONIZER_H
