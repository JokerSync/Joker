#ifndef PHSONYSETTINGS_H
#define PHSONYSETTINGS_H

#include <QString>

/**
 * @brief The settings for a PhSonyController
 */
class PhSonySettings {
public:
	/**
	 * @brief Shall the PhSonyController sync on a CTS up front or down front
	 * @return True for up front, false for down front
	 */
	virtual bool videoSyncUp() = 0;
	/**
	 * @brief Sony device id most significant byte
	 * @return A 8 bit value
	 */
	virtual unsigned char sonyDevice1() = 0;
	/**
	 * @brief Sony device id less significant byte
	 * @return A 8 bit value
	 */
	virtual unsigned char sonyDevice2() = 0;

	/**
	 * @brief Sony device fast rate
	 * @return A float value
	 */
	virtual float sonyFastRate() = 0;

	/**
	 * @brief The slave port name suffix
	 * @return A string
	 */
	virtual QString sonySlavePortSuffix() = 0;

	/**
	 * @brief The timecode type used for the communication on the slave side
	 * @return A timecode type value.
	 */
	virtual int sonySlaveCommunicationTimeCodeType() = 0;

	/**
	 * @brief The slave video sync timecode type
	 * @return A timecode type value.
	 */
	virtual int sonySlaveVideoSyncTimeCodeType() = 0;

	/**
	 * @brief The master port name suffix
	 * @return A string
	 */
	virtual QString sonyMasterPortSuffix() = 0;

	/**
	 * @brief The timecode type used for the communication on the master side
	 * @return A timecode type value.
	 */
	virtual int sonyMasterCommunicationTimeCodeType() = 0;

	/**
	 * @brief The master video sync timecode type
	 * @return A timecode type value.
	 */
	virtual int sonyMasterVideoSyncTimeCodeType() = 0;

};

#endif // PHSONYSETTINGS_H
