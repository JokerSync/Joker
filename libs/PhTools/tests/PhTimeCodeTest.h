#ifndef PHTIMECODETEST_H
#define PHTIMECODETEST_H

#include <QtTest>

class PhTimeCodeTest : public QObject
{
	Q_OBJECT
public:
	explicit PhTimeCodeTest(QObject *parent = 0);

private slots:
	/** Test stringFromFrame for 23.98 timecode. */
	void testStringFromFrameForTC2398();

	/** Test stringFromFrame for 24 timecode. */
	void testStringFromFrameForTC24();

	/**
	 Test stringFromFrame for 25 timecode.
	 */
	void testStringFromFrameForTC25();

	/**
	 Test stringFromFrame for 29.97 timecode.
	 */
	void testStringFromFrameForTC2997();

	/**
	 Test frameFromString for 23.98 timecode.
	 */
	void testFrameFromStringForTC2398();

	/**
	 Test frameFromString for 24 timecode.
	 */
	void testFrameFromStringForTC24();

	/**
	 Test frameFromString for 25 timecode.
	 */
	void testFrameFromStringForTC25();

	/**
	 Test frameFromString for 29.97 timecode.
	 */
	void testFrameFromStringForTC2997();

	/**
	 Test bcdFromFrame for 23.98 timecode.
	 */
	void testBcdFromFrameForTC2398();

	/**
	 Test bcdFromFrame for 24 timecode.
	 */
	void testBcdFromFrameForTC24();

	/**
	 Test bcdFromFrame for 25 timecode.
	 */
	void testBcdFromFrameForTC25();

	/**
	 Test bcdFromFrame for 29.97 timecode.
	 */
	void testBcdFromFrameForTC2997();

	/**
	 Test frameFromBcd for 23.98 timecode.
	 */
	void testFrameFromBcdForTC2398();

	/**
	 Test frameFromBcd for 24 timecode.
	 */
	void testFrameFromBcdForTC24();

	/**
	 Test frameFromBcd for 25 timecode.
	 */
	void testFrameFromBcdForTC25();

	/**
	 Test frameFromBcd for 29.97 timecode.
	 */
	void testFrameFromBcdForTC2997();

	/** Test stringFromFrame with value out of the common scope */
	void testStringFromFrameWithExtremeValue();

	/** Test stringFromFrame with bad formated strings */
	void testTCWithSpecialString();
};

#endif // PHTIMECODETEST_H
