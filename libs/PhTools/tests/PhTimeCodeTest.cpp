#include "PhTimeCodeTest.h"
#include "../PhTimeCode.h"

PhTimeCodeTest::PhTimeCodeTest(QObject *parent) :
	QObject(parent)
{
}

void PhTimeCodeTest::testStringFromFrameForTC2398()
{
	PhTimeCodeType type = PhTimeCodeType2398;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:00"));
	frame = 1;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:01"));
	frame = 23;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:23"));
	frame = 24;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:00"));
	frame = 47;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:23"));
	frame = 48;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:02:00"));
	frame = 1439;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:59:23"));
	frame = 1440;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:00:00"));
	frame = 2879;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:59:23"));
	frame = 2880;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:00:00"));
	frame = 4319;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:59:23"));
	frame = 4320;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:03:00:00"));
	frame = 14399;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:09:59:23"));
	frame = 14400;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:00"));
	frame = 14401;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:01"));
	frame = 15839;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:59:23"));
	frame = 15840;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:11:00:00"));
}

void PhTimeCodeTest::testStringFromFrameForTC24() {
	PhTimeCodeType type = PhTimeCodeType24;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:00"));
	frame = 1;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:01"));
	frame = 23;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:23"));
	frame = 24;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:00"));
	frame = 47;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:23"));
	frame = 48;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:02:00"));
	frame = 1439;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:59:23"));
	frame = 1440;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:00:00"));
	frame = 2879;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:59:23"));
	frame = 2880;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:00:00"));
	frame = 4319;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:59:23"));
	frame = 4320;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:03:00:00"));
	frame = 14399;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:09:59:23"));
	frame = 14400;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:00"));
	frame = 14401;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:01"));
	frame = 15839;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:59:23"));
	frame = 15840;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:11:00:00"));
}

void PhTimeCodeTest::testStringFromFrameForTC25() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:00"));
	frame = 1;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:01"));
	frame = 24;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:24"));
	frame = 25;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:00"));
	frame = 49;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:24"));
	frame = 50;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:02:00"));
	frame = 1499;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:59:24"));
	frame = 1500;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:00:00"));
	frame = 2999;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:59:24"));
	frame = 3000;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:00:00"));
	frame = 4499;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:59:24"));
	frame = 4500;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:03:00:00"));
	frame = 14999;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:09:59:24"));
	frame = 15000;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:00"));
	frame = 15001;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:01"));
	frame = 16499;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:59:24"));
	frame = 16500;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:11:00:00"));
}

void PhTimeCodeTest::testStringFromFrameForTC2997() {
	PhTimeCodeType type = PhTimeCodeType2997;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:00"));
	frame = 1;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:01"));
	frame = 2;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:02"));
	frame = 29;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:00:29"));
	frame = 30;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:00"));
	frame = 59;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:01:29"));
	frame = 60;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:02:00"));
	frame = 1799;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:00:59:29"));
	frame = 1800;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:00:02"));
	frame = 3597;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:01:59:29"));
	frame = 3598;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:00:02"));
	frame = 5395;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:02:59:29"));
	frame = 5396;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:03:00:02"));
	frame = 7193;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:03:59:29"));
	frame = 7194;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:04:00:02"));
	frame = 17981;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:09:59:29"));
	frame = 17982;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:00"));
	frame = 17983;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:00:01"));
	frame = 19781;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:10:59:29"));
	frame = 19782;
	QCOMPARE(PhTimeCode::stringFromFrame(frame, type), QString("00:11:00:02"));
}

void PhTimeCodeTest::testFrameFromStringForTC2398()
{
	PhTimeCodeType type = PhTimeCodeType2398;

	// Check various value of string
	QString string = "00:00:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 0);
	string = "00:00:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1);
	string = "00:00:00:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 23);
	string = "00:00:01:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 24);
	string = "00:00:01:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 47);
	string = "00:00:02:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 48);
	string = "00:00:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1439);
	string = "00:01:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1440);
	string = "00:01:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 2879);
	string = "00:02:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 2880);
	string = "00:02:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4319);
	string = "00:03:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4320);
	string = "00:09:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14399);
	string = "00:10:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14400);
	string = "00:10:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14401);
	string = "00:10:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 15839);
	string = "00:11:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 15840);
}

void PhTimeCodeTest::testFrameFromStringForTC24() {
	PhTimeCodeType type = PhTimeCodeType24;

	// Check various value of string
	QString string = "00:00:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 0);
	string = "00:00:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1);
	string = "00:00:00:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 23);
	string = "00:00:01:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 24);
	string = "00:00:01:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 47);
	string = "00:00:02:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 48);
	string = "00:00:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1439);
	string = "00:01:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1440);
	string = "00:01:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 2879);
	string = "00:02:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 2880);
	string = "00:02:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4319);
	string = "00:03:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4320);
	string = "00:09:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14399);
	string = "00:10:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14400);
	string = "00:10:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14401);
	string = "00:10:59:23";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 15839);
	string = "00:11:00:00";

}

void PhTimeCodeTest::testFrameFromStringForTC25() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Check various value of string
	QString string = "00:00:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 0);
	string = "00:00:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1);
	string = "00:00:00:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 24);
	string = "00:00:01:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 25);
	string = "00:00:01:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 49);
	string = "00:00:02:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 50);
	string = "00:00:59:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1499);
	string = "00:01:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1500);
	string = "00:01:59:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 2999);
	string = "00:02:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 3000);
	string = "00:02:59:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4499);
	string = "00:03:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 4500);
	string = "00:09:59:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 14999);
	string = "00:10:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 15000);
	string = "00:10:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 15001);
	string = "00:10:59:24";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 16499);
	string = "00:11:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 16500);
}

void PhTimeCodeTest::testFrameFromStringForTC2997() {
	PhTimeCodeType type = PhTimeCodeType2997;

	// Check various value of string
	QString string = "00:00:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 0);
	string = "00:00:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1);
	string = "00:00:00:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 29);
	string = "00:00:01:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 30);
	string = "00:00:01:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 59);
	string = "00:00:02:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 60);
	string = "00:00:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1799);
	string = "00:01:00:02";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 1800);
	string = "00:01:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 3597);
	string = "00:02:00:02";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 3598);
	string = "00:02:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 5395);
	string = "00:03:00:02";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 5396);
	string = "00:03:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 7193);
	string = "00:04:00:02";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 7194);
	string = "00:09:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 17981);
	string = "00:10:00:00";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 17982);
	string = "00:10:00:01";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 17983);
	string = "00:10:59:29";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 19781);
	string = "00:11:00:02";
	QCOMPARE((int)PhTimeCode::frameFromString(string, type), 19782);
}

void PhTimeCodeTest::testBcdFromFrameForTC2398() {
	PhTimeCodeType type = PhTimeCodeType2398;
	PhFrame frame = 0;

	// Check various value of frame
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000000);
	frame = 1;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000001);
	frame = 23;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000023);
	frame = 24;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000100);
	frame = 47;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000123);
	frame = 48;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000200);
	frame = 1439;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00005923);
	frame = 1440;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00010000);
	frame = 2879;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00015923);
	frame = 2880;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00020000);
	frame = 4319;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00025923);
	frame = 4320;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00030000);
	frame = 14399;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00095923);
	frame = 14400;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100000);
	frame = 14401;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100001);
	frame = 15839;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00105923);
	frame = 15840;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00110000);
}

void PhTimeCodeTest::testBcdFromFrameForTC24() {
	PhTimeCodeType type = PhTimeCodeType24;
	PhFrame frame = 0;

	// Check various value of frame
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000000);
	frame = 1;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000001);
	frame = 23;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000023);
	frame = 24;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000100);
	frame = 47;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000123);
	frame = 48;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000200);
	frame = 1439;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00005923);
	frame = 1440;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00010000);
	frame = 2879;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00015923);
	frame = 2880;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00020000);
	frame = 4319;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00025923);
	frame = 4320;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00030000);
	frame = 14399;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00095923);
	frame = 14400;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100000);
	frame = 14401;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100001);
	frame = 15839;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00105923);
	frame = 15840;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00110000);
}

void PhTimeCodeTest::testBcdFromFrameForTC25() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000000);
	frame = 1;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000001);
	frame = 24;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000024);
	frame = 25;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000100);
	frame = 49;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000124);
	frame = 50;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000200);
	frame = 1499;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00005924);
	frame = 1500;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00010000);
	frame = 2999;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00015924);
	frame = 3000;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00020000);
	frame = 4499;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00025924);
	frame = 4500;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00030000);
	frame = 14999;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00095924);
	frame = 15000;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100000);
	frame = 15001;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100001);
	frame = 16499;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00105924);
	frame = 16500;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00110000);
}

void PhTimeCodeTest::testBcdFromFrameForTC2997() {
	PhTimeCodeType type = PhTimeCodeType2997;

	// Check various value of frame
	PhFrame frame = 0;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x000000);
	frame = 1;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000001);
	frame = 2;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000002);
	frame = 29;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000029);
	frame = 30;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000100);
	frame = 59;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000129);
	frame = 60;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00000200);
	frame = 1799;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00005929);
	frame = 1800;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00010002);
	frame = 3597;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00015929);
	frame = 3598;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00020002);
	frame = 5395;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00025929);
	frame = 5396;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00030002);
	frame = 7193;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00035929);
	frame = 7194;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00040002);
	frame = 17981;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00095929);
	frame = 17982;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100000);
	frame = 17983;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00100001);
	frame = 19781;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00105929);
	frame = 19782;
	QCOMPARE((int)PhTimeCode::bcdFromFrame(frame, type), 0x00110002);
}

void PhTimeCodeTest::testFrameFromBcdForTC2398() {
	PhTimeCodeType type = PhTimeCodeType2398;

	// Check various value of binary coded decimal (bcd)
	unsigned int bcd = 0x00000000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 0);
	bcd = 0x00000001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1);
	bcd = 0x00000023;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 23);
	bcd = 0x00000100;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 24);
	bcd = 0x00000123;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 47);
	bcd = 0x00000200;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 48);
	bcd = 0x00005923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1439);
	bcd = 0x00010000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1440);
	bcd = 0x00015923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 2879);
	bcd = 0x00020000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 2880);
	bcd = 0x00025923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4319);
	bcd = 0x00030000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4320);
	bcd = 0x00095923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14399);
	bcd = 0x00100000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14400);
	bcd = 0x00100001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14401);
	bcd = 0x00105923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15839);
	bcd = 0x00110000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15840);
}

void PhTimeCodeTest::testFrameFromBcdForTC24() {
	PhTimeCodeType type = PhTimeCodeType24;

	// Check various value of binary coded decimal (bcd)
	unsigned int bcd = 0x00000000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 0);
	bcd = 0x00000001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1);
	bcd = 0x00000023;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 23);
	bcd = 0x00000100;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 24);
	bcd = 0x00000123;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 47);
	bcd = 0x00000200;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 48);
	bcd = 0x00005923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1439);
	bcd = 0x00010000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1440);
	bcd = 0x00015923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 2879);
	bcd = 0x00020000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 2880);
	bcd = 0x00025923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4319);
	bcd = 0x00030000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4320);
	bcd = 0x00095923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14399);
	bcd = 0x00100000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14400);
	bcd = 0x00100001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14401);
	bcd = 0x00105923;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15839);
	bcd = 0x00110000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15840);
}

void PhTimeCodeTest::testFrameFromBcdForTC25() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Check various value of binary coded decimal (bcd)
	unsigned int bcd = 0x00000000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 0);
	bcd = 0x00000001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1);
	bcd = 0x00000024;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 24);
	bcd = 0x00000100;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 25);
	bcd = 0x00000124;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 49);
	bcd = 0x00000200;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 50);
	bcd = 0x00005924;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1499);
	bcd = 0x00010000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1500);
	bcd = 0x00015924;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 2999);
	bcd = 0x00020000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 3000);
	bcd = 0x00025924;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4499);
	bcd = 0x00030000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 4500);
	bcd = 0x00095924;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 14999);
	bcd = 0x00100000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15000);
	bcd = 0x00100001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 15001);
	bcd = 0x00105924;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 16499);
	bcd = 0x00110000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 16500);
}

void PhTimeCodeTest::testFrameFromBcdForTC2997() {
	PhTimeCodeType type = PhTimeCodeType2997;

	// Check various value of binary coded decimal (bcd)
	unsigned int bcd = 0x00000000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 0);
	bcd = 0x00000001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1);
	bcd = 0x00000029;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 29);
	bcd = 0x00000100;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 30);
	bcd = 0x00000129;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 59);
	bcd = 0x00000200;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 60);
	bcd = 0x00005929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1799);
	bcd = 0x00010002;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 1800);
	bcd = 0x00015929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 3597);
	bcd = 0x00020002;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 3598);
	bcd = 0x00025929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 5395);
	bcd = 0x00030002;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 5396);
	bcd = 0x00035929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 7193);
	bcd = 0x00040002;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 7194);
	bcd = 0x00095929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 17981);
	bcd = 0x00100000;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 17982);
	bcd = 0x00100001;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 17983);
	bcd = 0x00105929;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 19781);
	bcd = 0x00110002;
	QCOMPARE((int)PhTimeCode::frameFromBcd(bcd, type), 19782);
}


void PhTimeCodeTest::testStringFromFrameWithExtremeValue() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Test frame values around 24h
	QCOMPARE(PhTimeCode::stringFromFrame(2159999, type), QString("23:59:59:24"));
	QCOMPARE(PhTimeCode::stringFromFrame(2160000, type), QString("24:00:00:00"));
	QCOMPARE(PhTimeCode::stringFromFrame(2160001, type), QString("24:00:00:01"));
	QCOMPARE(PhTimeCode::stringFromFrame(2176499, type), QString("24:10:59:24"));

	// Test frame values around -24h
	QCOMPARE(PhTimeCode::stringFromFrame(-1, type), QString("-00:00:00:01"));
	QCOMPARE(PhTimeCode::stringFromFrame(-2159999, type), QString("-23:59:59:24"));
	QCOMPARE(PhTimeCode::stringFromFrame(-2160000, type), QString("-24:00:00:00"));
	QCOMPARE(PhTimeCode::stringFromFrame(-2160001, type), QString("-24:00:00:01"));
	QCOMPARE(PhTimeCode::stringFromFrame(-2176499, type), QString("-24:10:59:24"));
}

void PhTimeCodeTest::testTCWithSpecialString() {
	PhTimeCodeType type = PhTimeCodeType25;

	// Test bad value for hh, mm, ss and ff
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:23:34:30", type), type), QString("12:23:34:00"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:23:34:ff", type), type), QString("12:23:34:00"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:23:60:19", type), type), QString("12:23:00:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:23:ss:19", type), type), QString("12:23:00:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:60:34:19", type), type), QString("12:00:34:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:mm:34:19", type), type), QString("12:00:34:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("hh:23:34:19", type), type), QString("00:23:34:19"));

	// Test extreme value for hh
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("24:23:34:19", type), type), QString("24:23:34:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("-1:23:34:19", type), type), QString("-01:23:34:19"));

	// Test bad digit count : if there is not enough digit, frame are considered first, then seconds, minutes and hours
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("19"            , type), type), QString("00:00:00:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("34:19"         , type), type), QString("00:00:34:19"));
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("23:34:19"      , type), type), QString("00:23:34:19"));

	// Test bad digit count : if there is to much digit, only the first four are taken in account
	QCOMPARE(PhTimeCode::stringFromFrame(PhTimeCode::frameFromString("12:23:34:19:12", type), type), QString("12:23:34:19"));
}

