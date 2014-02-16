/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTIMECODETEST_H
#define PHTIMECODETEST_H

#include <QtTest>

/**
 * @brief Test the PhTimeCode class.
 */
class PhTimeCodeTest : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhTimeCodeTest constructor.
	 * @param parent The object owner.
	 */
	explicit PhTimeCodeTest(QObject *parent = 0);

private slots:
	/** @brief Test stringFromFrame for 23.98 timecode. */
	void testStringFromFrameForTC2398();

	/** @brief Test stringFromFrame for 24 timecode. */
	void testStringFromFrameForTC24();

	/** @brief Test stringFromFrame for 25 timecode. */
	void testStringFromFrameForTC25();

	/** @brief Test stringFromFrame for 29.97 timecode. */
	void testStringFromFrameForTC2997();

	/** @brief Test frameFromString for 23.98 timecode. */
	void testFrameFromStringForTC2398();

	/** @brief Test frameFromString for 24 timecode. */
	void testFrameFromStringForTC24();

	/** @brief Test frameFromString for 25 timecode. */
	void testFrameFromStringForTC25();

	/** @brief Test frameFromString for 29.97 timecode. */
	void testFrameFromStringForTC2997();

	/** @brief Test bcdFromFrame for 23.98 timecode. */
	void testBcdFromFrameForTC2398();

	/** @brief Test bcdFromFrame for 24 timecode. */
	void testBcdFromFrameForTC24();

	/** @brief Test bcdFromFrame for 25 timecode. */
	void testBcdFromFrameForTC25();

	/** @brief Test bcdFromFrame for 29.97 timecode. */
	void testBcdFromFrameForTC2997();

	/** @brief Test frameFromBcd for 23.98 timecode. */
	void testFrameFromBcdForTC2398();

	/** @brief Test frameFromBcd for 24 timecode. */
	void testFrameFromBcdForTC24();

	/** @brief Test frameFromBcd for 25 timecode. */
	void testFrameFromBcdForTC25();

	/** @brief Test frameFromBcd for 29.97 timecode. */
	void testFrameFromBcdForTC2997();

	/** @brief Test stringFromFrame with value out of the common scope */
	void testStringFromFrameWithExtremeValue();

	/** @brief Test stringFromFrame with bad formated strings */
	void testTCWithSpecialString();
};

#endif // PHTIMECODETEST_H
