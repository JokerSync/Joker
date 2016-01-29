/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhSync/PhTimeCode.h"

#include "CommonSpec.h"

#include "PhSpec.h"

using namespace bandit;

go_bandit([](){
	describe("timecode_test", []() {
		before_each([](){
			PhDebug::disable();
		});

		it("get_average_fps", []() {
			AssertThat(PhTimeCode::getAverageFps(PhTimeCodeType2398), Equals(23.98f));
			AssertThat(PhTimeCode::getAverageFps(PhTimeCodeType24), Equals(24.00f));
			AssertThat(PhTimeCode::getAverageFps(PhTimeCodeType25), Equals(25.00f));
			AssertThat(PhTimeCode::getAverageFps(PhTimeCodeType2997), Equals(29.97f));
			AssertThat(PhTimeCode::getAverageFps(PhTimeCodeType30), Equals(30.0f));
		});

		it("compute_timecode_type_from_float_value", []() {
			AssertThat(PhTimeCode::computeTimeCodeType(0.0f), Equals(PhTimeCodeType25));

			AssertThat(PhTimeCode::computeTimeCodeType(1.0f), Equals(PhTimeCodeType2398));
			AssertThat(PhTimeCode::computeTimeCodeType(23.98f), Equals(PhTimeCodeType2398));
			AssertThat(PhTimeCode::computeTimeCodeType(23.9999f), Equals(PhTimeCodeType2398));

			AssertThat(PhTimeCode::computeTimeCodeType(24.0f), Equals(PhTimeCodeType24));
			AssertThat(PhTimeCode::computeTimeCodeType(24.49f), Equals(PhTimeCodeType24));

			AssertThat(PhTimeCode::computeTimeCodeType(24.5f), Equals(PhTimeCodeType25));
			AssertThat(PhTimeCode::computeTimeCodeType(25.0f), Equals(PhTimeCodeType25));
			AssertThat(PhTimeCode::computeTimeCodeType(25.9999f), Equals(PhTimeCodeType25));

			AssertThat(PhTimeCode::computeTimeCodeType(26.0f), Equals(PhTimeCodeType2997));
			AssertThat(PhTimeCode::computeTimeCodeType(29.97f), Equals(PhTimeCodeType2997));
			AssertThat(PhTimeCode::computeTimeCodeType(29.99f), Equals(PhTimeCodeType2997));

			AssertThat(PhTimeCode::computeTimeCodeType(30.0f), Equals(PhTimeCodeType30));
			AssertThat(PhTimeCode::computeTimeCodeType(30.99f), Equals(PhTimeCodeType30));

			AssertThat(PhTimeCode::computeTimeCodeType(31.0f), Equals(PhTimeCodeType25));
		});

		it("get_time_per_frame", []() {
			AssertThat(PhTimeCode::timePerFrame(PhTimeCodeType2398), Equals(1001));
			AssertThat(PhTimeCode::timePerFrame(PhTimeCodeType24), Equals(1000));
			AssertThat(PhTimeCode::timePerFrame(PhTimeCodeType25), Equals(960));
			AssertThat(PhTimeCode::timePerFrame(PhTimeCodeType2997), Equals(801));
			AssertThat(PhTimeCode::timePerFrame(PhTimeCodeType30), Equals(800));
		});

		describe("2398fps", [](){
			PhTimeCodeType type = PhTimeCodeType2398;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::stringFromFrame(0, type).toStdString(), Equals("00:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(1, type).toStdString(), Equals("00:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(23, type).toStdString(), Equals("00:00:00:23"));
				AssertThat(PhTimeCode::stringFromFrame(24, type).toStdString(), Equals("00:00:01:00"));
				AssertThat(PhTimeCode::stringFromFrame(47, type).toStdString(), Equals("00:00:01:23"));
				AssertThat(PhTimeCode::stringFromFrame(48, type).toStdString(), Equals("00:00:02:00"));
				AssertThat(PhTimeCode::stringFromFrame(1439, type).toStdString(), Equals("00:00:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(1440, type).toStdString(), Equals("00:01:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(2879, type).toStdString(), Equals("00:01:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(2880, type).toStdString(), Equals("00:02:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(4319, type).toStdString(), Equals("00:02:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(4320, type).toStdString(), Equals("00:03:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(14399, type).toStdString(), Equals("00:09:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(14400, type).toStdString(), Equals("00:10:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(14401, type).toStdString(), Equals("00:10:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(15839, type).toStdString(), Equals("00:10:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(15840, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));
				AssertThat(PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));
				AssertThat(PhTimeCode::frameFromString("00:00:00:23", type), Equals(23));
				AssertThat(PhTimeCode::frameFromString("00:00:01:00", type), Equals(24));
				AssertThat(PhTimeCode::frameFromString("00:00:01:23", type), Equals(47));
				AssertThat(PhTimeCode::frameFromString("00:00:02:00", type), Equals(48));
				AssertThat(PhTimeCode::frameFromString("00:00:59:23", type), Equals(1439));
				AssertThat(PhTimeCode::frameFromString("00:01:00:00", type), Equals(1440));
				AssertThat(PhTimeCode::frameFromString("00:01:59:23", type), Equals(2879));
				AssertThat(PhTimeCode::frameFromString("00:02:00:00", type), Equals(2880));
				AssertThat(PhTimeCode::frameFromString("00:02:59:23", type), Equals(4319));
				AssertThat(PhTimeCode::frameFromString("00:03:00:00", type), Equals(4320));
				AssertThat(PhTimeCode::frameFromString("00:09:59:23", type), Equals(14399));
				AssertThat(PhTimeCode::frameFromString("00:10:00:00", type), Equals(14400));
				AssertThat(PhTimeCode::frameFromString("00:10:00:01", type), Equals(14401));
				AssertThat(PhTimeCode::frameFromString("00:10:59:23", type), Equals(15839));
				AssertThat(PhTimeCode::frameFromString("00:11:00:00", type), Equals(15840));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::bcdFromFrame(0, type), Equals(0x00000000u));
				AssertThat(PhTimeCode::bcdFromFrame(1, type), Equals(0x00000001u));
				AssertThat(PhTimeCode::bcdFromFrame(23, type), Equals(0x00000023u));
				AssertThat(PhTimeCode::bcdFromFrame(24, type), Equals(0x00000100u));
				AssertThat(PhTimeCode::bcdFromFrame(47, type), Equals(0x00000123u));
				AssertThat(PhTimeCode::bcdFromFrame(48, type), Equals(0x00000200u));
				AssertThat(PhTimeCode::bcdFromFrame(1439, type), Equals(0x00005923u));
				AssertThat(PhTimeCode::bcdFromFrame(1440, type), Equals(0x00010000u));
				AssertThat(PhTimeCode::bcdFromFrame(2879, type), Equals(0x00015923u));
				AssertThat(PhTimeCode::bcdFromFrame(2880, type), Equals(0x00020000u));
				AssertThat(PhTimeCode::bcdFromFrame(4319, type), Equals(0x00025923u));
				AssertThat(PhTimeCode::bcdFromFrame(4320, type), Equals(0x00030000u));
				AssertThat(PhTimeCode::bcdFromFrame(14399, type), Equals(0x00095923u));
				AssertThat(PhTimeCode::bcdFromFrame(14400, type), Equals(0x00100000u));
				AssertThat(PhTimeCode::bcdFromFrame(14401, type), Equals(0x00100001u));
				AssertThat(PhTimeCode::bcdFromFrame(15839, type), Equals(0x00105923u));
				AssertThat(PhTimeCode::bcdFromFrame(15840, type), Equals(0x00110000u));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				AssertThat(PhTimeCode::frameFromBcd(0x00000000, type), Equals(0));
				AssertThat(PhTimeCode::frameFromBcd(0x00000001, type), Equals(1));
				AssertThat(PhTimeCode::frameFromBcd(0x00000023, type), Equals(23));
				AssertThat(PhTimeCode::frameFromBcd(0x00000100, type), Equals(24));
				AssertThat(PhTimeCode::frameFromBcd(0x00000123, type), Equals(47));
				AssertThat(PhTimeCode::frameFromBcd(0x00000200, type), Equals(48));
				AssertThat(PhTimeCode::frameFromBcd(0x00005923, type), Equals(1439));
				AssertThat(PhTimeCode::frameFromBcd(0x00010000, type), Equals(1440));
				AssertThat(PhTimeCode::frameFromBcd(0x00015923, type), Equals(2879));
				AssertThat(PhTimeCode::frameFromBcd(0x00020000, type), Equals(2880));
				AssertThat(PhTimeCode::frameFromBcd(0x00025923, type), Equals(4319));
				AssertThat(PhTimeCode::frameFromBcd(0x00030000, type), Equals(4320));
				AssertThat(PhTimeCode::frameFromBcd(0x00095923, type), Equals(14399));
				AssertThat(PhTimeCode::frameFromBcd(0x00100000, type), Equals(14400));
				AssertThat(PhTimeCode::frameFromBcd(0x00100001, type), Equals(14401));
				AssertThat(PhTimeCode::frameFromBcd(0x00105923, type), Equals(15839));
				AssertThat(PhTimeCode::frameFromBcd(0x00110000, type), Equals(15840));
			});

		});

		describe("24fps", [](){
			PhTimeCodeType type = PhTimeCodeType24;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::stringFromFrame(0, type).toStdString(), Equals("00:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(1, type).toStdString(), Equals("00:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(23, type).toStdString(), Equals("00:00:00:23"));
				AssertThat(PhTimeCode::stringFromFrame(24, type).toStdString(), Equals("00:00:01:00"));
				AssertThat(PhTimeCode::stringFromFrame(47, type).toStdString(), Equals("00:00:01:23"));
				AssertThat(PhTimeCode::stringFromFrame(48, type).toStdString(), Equals("00:00:02:00"));
				AssertThat(PhTimeCode::stringFromFrame(1439, type).toStdString(), Equals("00:00:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(1440, type).toStdString(), Equals("00:01:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(2879, type).toStdString(), Equals("00:01:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(2880, type).toStdString(), Equals("00:02:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(4319, type).toStdString(), Equals("00:02:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(4320, type).toStdString(), Equals("00:03:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(14399, type).toStdString(), Equals("00:09:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(14400, type).toStdString(), Equals("00:10:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(14401, type).toStdString(), Equals("00:10:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(15839, type).toStdString(), Equals("00:10:59:23"));
				AssertThat(PhTimeCode::stringFromFrame(15840, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));
				AssertThat(PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));
				AssertThat(PhTimeCode::frameFromString("00:00:00:23", type), Equals(23));
				AssertThat(PhTimeCode::frameFromString("00:00:01:00", type), Equals(24));
				AssertThat(PhTimeCode::frameFromString("00:00:01:23", type), Equals(47));
				AssertThat(PhTimeCode::frameFromString("00:00:02:00", type), Equals(48));
				AssertThat(PhTimeCode::frameFromString("00:00:59:23", type), Equals(1439));
				AssertThat(PhTimeCode::frameFromString("00:01:00:00", type), Equals(1440));
				AssertThat(PhTimeCode::frameFromString("00:01:59:23", type), Equals(2879));
				AssertThat(PhTimeCode::frameFromString("00:02:00:00", type), Equals(2880));
				AssertThat(PhTimeCode::frameFromString("00:02:59:23", type), Equals(4319));
				AssertThat(PhTimeCode::frameFromString("00:03:00:00", type), Equals(4320));
				AssertThat(PhTimeCode::frameFromString("00:09:59:23", type), Equals(14399));
				AssertThat(PhTimeCode::frameFromString("00:10:00:00", type), Equals(14400));
				AssertThat(PhTimeCode::frameFromString("00:10:00:01", type), Equals(14401));
				AssertThat(PhTimeCode::frameFromString("00:10:59:23", type), Equals(15839));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::bcdFromFrame(0, type), Equals(0x00000000u));
				AssertThat(PhTimeCode::bcdFromFrame(1, type), Equals(0x00000001u));
				AssertThat(PhTimeCode::bcdFromFrame(23, type), Equals(0x00000023u));
				AssertThat(PhTimeCode::bcdFromFrame(24, type), Equals(0x00000100u));
				AssertThat(PhTimeCode::bcdFromFrame(47, type), Equals(0x00000123u));
				AssertThat(PhTimeCode::bcdFromFrame(48, type), Equals(0x00000200u));
				AssertThat(PhTimeCode::bcdFromFrame(1439, type), Equals(0x00005923u));
				AssertThat(PhTimeCode::bcdFromFrame(1440, type), Equals(0x00010000u));
				AssertThat(PhTimeCode::bcdFromFrame(2879, type), Equals(0x00015923u));
				AssertThat(PhTimeCode::bcdFromFrame(2880, type), Equals(0x00020000u));
				AssertThat(PhTimeCode::bcdFromFrame(4319, type), Equals(0x00025923u));
				AssertThat(PhTimeCode::bcdFromFrame(4320, type), Equals(0x00030000u));
				AssertThat(PhTimeCode::bcdFromFrame(14399, type), Equals(0x00095923u));
				AssertThat(PhTimeCode::bcdFromFrame(14400, type), Equals(0x00100000u));
				AssertThat(PhTimeCode::bcdFromFrame(14401, type), Equals(0x00100001u));
				AssertThat(PhTimeCode::bcdFromFrame(15839, type), Equals(0x00105923u));
				AssertThat(PhTimeCode::bcdFromFrame(15840, type), Equals(0x00110000u));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				AssertThat(PhTimeCode::frameFromBcd(0x00000000, type), Equals(0));
				AssertThat(PhTimeCode::frameFromBcd(0x00000001, type), Equals(1));
				AssertThat(PhTimeCode::frameFromBcd(0x00000023, type), Equals(23));
				AssertThat(PhTimeCode::frameFromBcd(0x00000100, type), Equals(24));
				AssertThat(PhTimeCode::frameFromBcd(0x00000123, type), Equals(47));
				AssertThat(PhTimeCode::frameFromBcd(0x00000200, type), Equals(48));
				AssertThat(PhTimeCode::frameFromBcd(0x00005923, type), Equals(1439));
				AssertThat(PhTimeCode::frameFromBcd(0x00010000, type), Equals(1440));
				AssertThat(PhTimeCode::frameFromBcd(0x00015923, type), Equals(2879));
				AssertThat(PhTimeCode::frameFromBcd(0x00020000, type), Equals(2880));
				AssertThat(PhTimeCode::frameFromBcd(0x00025923, type), Equals(4319));
				AssertThat(PhTimeCode::frameFromBcd(0x00030000, type), Equals(4320));
				AssertThat(PhTimeCode::frameFromBcd(0x00095923, type), Equals(14399));
				AssertThat(PhTimeCode::frameFromBcd(0x00100000, type), Equals(14400));
				AssertThat(PhTimeCode::frameFromBcd(0x00100001, type), Equals(14401));
				AssertThat(PhTimeCode::frameFromBcd(0x00105923, type), Equals(15839));
				AssertThat(PhTimeCode::frameFromBcd(0x00110000, type), Equals(15840));
			});
		});

		describe("25fps", [](){
			PhTimeCodeType type = PhTimeCodeType25;

			it("get_string_from_time", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::stringFromTime(0, type).toStdString(), Equals("00:00:00:00"));
				AssertThat(PhTimeCode::stringFromTime(960, type).toStdString(), Equals("00:00:00:01"));
				AssertThat(PhTimeCode::stringFromTime(23040, type).toStdString(), Equals("00:00:00:24"));
				AssertThat(PhTimeCode::stringFromTime(24000, type).toStdString(), Equals("00:00:01:00"));
				AssertThat(PhTimeCode::stringFromTime(47040, type).toStdString(), Equals("00:00:01:24"));
				AssertThat(PhTimeCode::stringFromTime(48000, type).toStdString(), Equals("00:00:02:00"));
				AssertThat(PhTimeCode::stringFromTime(1439040, type).toStdString(), Equals("00:00:59:24"));
				AssertThat(PhTimeCode::stringFromTime(1440000, type).toStdString(), Equals("00:01:00:00"));
				AssertThat(PhTimeCode::stringFromTime(2879040, type).toStdString(), Equals("00:01:59:24"));
				AssertThat(PhTimeCode::stringFromTime(2880000, type).toStdString(), Equals("00:02:00:00"));
				AssertThat(PhTimeCode::stringFromTime(4319040, type).toStdString(), Equals("00:02:59:24"));
				AssertThat(PhTimeCode::stringFromTime(4320000, type).toStdString(), Equals("00:03:00:00"));
				AssertThat(PhTimeCode::stringFromTime(14399040, type).toStdString(), Equals("00:09:59:24"));
				AssertThat(PhTimeCode::stringFromTime(14400000, type).toStdString(), Equals("00:10:00:00"));
				AssertThat(PhTimeCode::stringFromTime(14400960, type).toStdString(), Equals("00:10:00:01"));
				AssertThat(PhTimeCode::stringFromTime(15839040, type).toStdString(), Equals("00:10:59:24"));
				AssertThat(PhTimeCode::stringFromTime(15840000, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_string_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::stringFromFrame(0, type).toStdString(), Equals("00:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(1, type).toStdString(), Equals("00:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(24, type).toStdString(), Equals("00:00:00:24"));
				AssertThat(PhTimeCode::stringFromFrame(25, type).toStdString(), Equals("00:00:01:00"));
				AssertThat(PhTimeCode::stringFromFrame(49, type).toStdString(), Equals("00:00:01:24"));
				AssertThat(PhTimeCode::stringFromFrame(50, type).toStdString(), Equals("00:00:02:00"));
				AssertThat(PhTimeCode::stringFromFrame(1499, type).toStdString(), Equals("00:00:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(1500, type).toStdString(), Equals("00:01:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(2999, type).toStdString(), Equals("00:01:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(3000, type).toStdString(), Equals("00:02:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(4499, type).toStdString(), Equals("00:02:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(4500, type).toStdString(), Equals("00:03:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(14999, type).toStdString(), Equals("00:09:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(15000, type).toStdString(), Equals("00:10:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(15001, type).toStdString(), Equals("00:10:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(16499, type).toStdString(), Equals("00:10:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(16500, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_time_from_string", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::timeFromString("00:00:00:00", type), Equals(0));
				AssertThat(PhTimeCode::timeFromString("00:00:00:01", type), Equals(960));
				AssertThat(PhTimeCode::timeFromString("00:00:00:24", type), Equals(23040));
				AssertThat(PhTimeCode::timeFromString("00:00:01:00", type), Equals(24000));
				AssertThat(PhTimeCode::timeFromString("00:00:01:24", type), Equals(47040));
				AssertThat(PhTimeCode::timeFromString("00:00:02:00", type), Equals(48000));
				AssertThat(PhTimeCode::timeFromString("00:00:59:24", type), Equals(1439040));
				AssertThat(PhTimeCode::timeFromString("00:01:00:00", type), Equals(1440000));
				AssertThat(PhTimeCode::timeFromString("00:01:59:24", type), Equals(2879040));
				AssertThat(PhTimeCode::timeFromString("00:02:00:00", type), Equals(2880000));
				AssertThat(PhTimeCode::timeFromString("00:02:59:24", type), Equals(4319040));
				AssertThat(PhTimeCode::timeFromString("00:03:00:00", type), Equals(4320000));
				AssertThat(PhTimeCode::timeFromString("00:09:59:24", type), Equals(14399040));
				AssertThat(PhTimeCode::timeFromString("00:10:00:00", type), Equals(14400000));
				AssertThat(PhTimeCode::timeFromString("00:10:00:01", type), Equals(14400960));
				AssertThat(PhTimeCode::timeFromString("00:10:59:24", type), Equals(15839040));
				AssertThat(PhTimeCode::timeFromString("00:11:00:00", type), Equals(15840000));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));
				AssertThat(PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));
				AssertThat(PhTimeCode::frameFromString("00:00:00:24", type), Equals(24));
				AssertThat(PhTimeCode::frameFromString("00:00:01:00", type), Equals(25));
				AssertThat(PhTimeCode::frameFromString("00:00:01:24", type), Equals(49));
				AssertThat(PhTimeCode::frameFromString("00:00:02:00", type), Equals(50));
				AssertThat(PhTimeCode::frameFromString("00:00:59:24", type), Equals(1499));
				AssertThat(PhTimeCode::frameFromString("00:01:00:00", type), Equals(1500));
				AssertThat(PhTimeCode::frameFromString("00:01:59:24", type), Equals(2999));
				AssertThat(PhTimeCode::frameFromString("00:02:00:00", type), Equals(3000));
				AssertThat(PhTimeCode::frameFromString("00:02:59:24", type), Equals(4499));
				AssertThat(PhTimeCode::frameFromString("00:03:00:00", type), Equals(4500));
				AssertThat(PhTimeCode::frameFromString("00:09:59:24", type), Equals(14999));
				AssertThat(PhTimeCode::frameFromString("00:10:00:00", type), Equals(15000));
				AssertThat(PhTimeCode::frameFromString("00:10:00:01", type), Equals(15001));
				AssertThat(PhTimeCode::frameFromString("00:10:59:24", type), Equals(16499));
				AssertThat(PhTimeCode::frameFromString("00:11:00:00", type), Equals(16500));
			});

			it("get_bcd_from_time", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::bcdFromTime(0, type), Equals(0x00000000u));
				AssertThat(PhTimeCode::bcdFromTime(960, type), Equals(0x00000001u));
				AssertThat(PhTimeCode::bcdFromTime(23040, type), Equals(0x00000024u));
				AssertThat(PhTimeCode::bcdFromTime(24000, type), Equals(0x00000100u));
				AssertThat(PhTimeCode::bcdFromTime(47040, type), Equals(0x00000124u));
				AssertThat(PhTimeCode::bcdFromTime(48000, type), Equals(0x00000200u));
				AssertThat(PhTimeCode::bcdFromTime(1439040, type), Equals(0x00005924u));
				AssertThat(PhTimeCode::bcdFromTime(1440000, type), Equals(0x00010000u));
				AssertThat(PhTimeCode::bcdFromTime(2879040, type), Equals(0x00015924u));
				AssertThat(PhTimeCode::bcdFromTime(2880000, type), Equals(0x00020000u));
				AssertThat(PhTimeCode::bcdFromTime(4319040, type), Equals(0x00025924u));
				AssertThat(PhTimeCode::bcdFromTime(4320000, type), Equals(0x00030000u));
				AssertThat(PhTimeCode::bcdFromTime(14399040, type), Equals(0x00095924u));
				AssertThat(PhTimeCode::bcdFromTime(14400000, type), Equals(0x00100000u));
				AssertThat(PhTimeCode::bcdFromTime(14400960, type), Equals(0x00100001u));
				AssertThat(PhTimeCode::bcdFromTime(15839040, type), Equals(0x00105924u));
				AssertThat(PhTimeCode::bcdFromTime(15840000, type), Equals(0x00110000u));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::bcdFromFrame(0, type), Equals((int)0x00000000u));
				AssertThat(PhTimeCode::bcdFromFrame(1, type), Equals(0x00000001u));
				AssertThat(PhTimeCode::bcdFromFrame(24, type), Equals(0x00000024u));
				AssertThat(PhTimeCode::bcdFromFrame(25, type), Equals(0x00000100u));
				AssertThat(PhTimeCode::bcdFromFrame(49, type), Equals(0x00000124u));
				AssertThat(PhTimeCode::bcdFromFrame(50, type), Equals(0x00000200u));
				AssertThat(PhTimeCode::bcdFromFrame(1499, type), Equals(0x00005924u));
				AssertThat(PhTimeCode::bcdFromFrame(1500, type), Equals(0x00010000u));
				AssertThat(PhTimeCode::bcdFromFrame(2999, type), Equals(0x00015924u));
				AssertThat(PhTimeCode::bcdFromFrame(3000, type), Equals(0x00020000u));
				AssertThat(PhTimeCode::bcdFromFrame(4499, type), Equals(0x00025924u));
				AssertThat(PhTimeCode::bcdFromFrame(4500, type), Equals(0x00030000u));
				AssertThat(PhTimeCode::bcdFromFrame(14999, type), Equals(0x00095924u));
				AssertThat(PhTimeCode::bcdFromFrame(15000, type), Equals(0x00100000u));
				AssertThat(PhTimeCode::bcdFromFrame(15001, type), Equals(0x00100001u));
				AssertThat(PhTimeCode::bcdFromFrame(16499, type), Equals(0x00105924u));
				AssertThat(PhTimeCode::bcdFromFrame(16500, type), Equals(0x00110000u));
			});

			it("get_time_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				AssertThat(PhTimeCode::timeFromBcd(0x00000000, type), Equals(0));
				AssertThat(PhTimeCode::timeFromBcd(0x00000001, type), Equals(960));
				AssertThat(PhTimeCode::timeFromBcd(0x00000024, type), Equals(23040));
				AssertThat(PhTimeCode::timeFromBcd(0x00000100, type), Equals(24000));
				AssertThat(PhTimeCode::timeFromBcd(0x00000124, type), Equals(47040));
				AssertThat(PhTimeCode::timeFromBcd(0x00000200, type), Equals(48000));
				AssertThat(PhTimeCode::timeFromBcd(0x00005924, type), Equals(1439040));
				AssertThat(PhTimeCode::timeFromBcd(0x00010000, type), Equals(1440000));
				AssertThat(PhTimeCode::timeFromBcd(0x00015924, type), Equals(2879040));
				AssertThat(PhTimeCode::timeFromBcd(0x00020000, type), Equals(2880000));
				AssertThat(PhTimeCode::timeFromBcd(0x00025924, type), Equals(4319040));
				AssertThat(PhTimeCode::timeFromBcd(0x00030000, type), Equals(4320000));
				AssertThat(PhTimeCode::timeFromBcd(0x00095924, type), Equals(14399040));
				AssertThat(PhTimeCode::timeFromBcd(0x00100000, type), Equals(14400000));
				AssertThat(PhTimeCode::timeFromBcd(0x00100001, type), Equals(14400960));
				AssertThat(PhTimeCode::timeFromBcd(0x00105924, type), Equals(15839040));
				AssertThat(PhTimeCode::timeFromBcd(0x00110000, type), Equals(15840000));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				AssertThat(PhTimeCode::frameFromBcd(0x00000000, type), Equals(0));
				AssertThat(PhTimeCode::frameFromBcd(0x00000001, type), Equals(1));
				AssertThat(PhTimeCode::frameFromBcd(0x00000024, type), Equals(24));
				AssertThat(PhTimeCode::frameFromBcd(0x00000100, type), Equals(25));
				AssertThat(PhTimeCode::frameFromBcd(0x00000124, type), Equals(49));
				AssertThat(PhTimeCode::frameFromBcd(0x00000200, type), Equals(50));
				AssertThat(PhTimeCode::frameFromBcd(0x00005924, type), Equals(1499));
				AssertThat(PhTimeCode::frameFromBcd(0x00010000, type), Equals(1500));
				AssertThat(PhTimeCode::frameFromBcd(0x00015924, type), Equals(2999));
				AssertThat(PhTimeCode::frameFromBcd(0x00020000, type), Equals(3000));
				AssertThat(PhTimeCode::frameFromBcd(0x00025924, type), Equals(4499));
				AssertThat(PhTimeCode::frameFromBcd(0x00030000, type), Equals(4500));
				AssertThat(PhTimeCode::frameFromBcd(0x00095924, type), Equals(14999));
				AssertThat(PhTimeCode::frameFromBcd(0x00100000, type), Equals(15000));
				AssertThat(PhTimeCode::frameFromBcd(0x00100001, type), Equals(15001));
				AssertThat(PhTimeCode::frameFromBcd(0x00105924, type), Equals(16499));
				AssertThat(PhTimeCode::frameFromBcd(0x00110000, type), Equals(16500));
			});

			it("get_time_from_hhmmssf_1", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 0, 0,  type), Equals(0));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 0, 1, type), Equals(960));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 0, 24, type), Equals(23040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 1, 0,  type), Equals(24000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 1, 24, type), Equals(47040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 2, 0,  type), Equals(48000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 0, 59, 24, type), Equals(1439040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 1, 0, 0,  type), Equals(1440000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 1, 59, 24, type), Equals(2879040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 2, 0, 0,  type), Equals(2880000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 2, 59, 24, type), Equals(4319040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 3, 0, 0,  type), Equals(4320000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 9, 59, 24, type), Equals(14399040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 10, 0, 0,  type), Equals(14400000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 10, 0, 1, type), Equals(14400960));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 10, 59, 24, type), Equals(15839040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(0, 11, 0, 0,  type), Equals(15840000));
			});

			it("get_time_from_hhmmssf_2", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  0,  0},  type), Equals(0));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  0,  1},  type), Equals(960));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  0,  24}, type), Equals(23040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  1,  0},  type), Equals(24000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  1,  24}, type), Equals(47040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  2,  0},  type), Equals(48000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 0,  59, 24}, type), Equals(1439040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 1,  0,  0},  type), Equals(1440000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 1,  59, 24}, type), Equals(2879040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 2,  0,  0},  type), Equals(2880000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 2,  59, 24}, type), Equals(4319040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 3,  0,  0},  type), Equals(4320000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 9,  59, 24}, type), Equals(14399040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 10, 0,  0},  type), Equals(14400000));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 10, 0,  1},  type), Equals(14400960));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 10, 59, 24}, type), Equals(15839040));
				AssertThat(PhTimeCode::timeFromHhMmSsFf(new unsigned int[4]{0, 11, 0,  0},  type), Equals(15840000));
			});

			it("get_frame_from_hhmmssf", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 0, 0,  type), Equals(0));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 0, 01, type), Equals(1));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 0, 24, type), Equals(24));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 1, 0,  type), Equals(25));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 1, 24, type), Equals(49));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 2, 0,  type), Equals(50));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 0, 59, 24, type), Equals(1499));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 1, 0, 0,  type), Equals(1500));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 1, 59, 24, type), Equals(2999));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 2, 0, 0,  type), Equals(3000));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 2, 59, 24, type), Equals(4499));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 3, 0, 0,  type), Equals(4500));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 9, 59, 24, type), Equals(14999));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 10, 0, 0,  type), Equals(15000));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 10, 0, 1, type), Equals(15001));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 10, 59, 24, type), Equals(16499));
				AssertThat(PhTimeCode::frameFromHhMmSsFf(0, 11, 0, 0,  type), Equals(16500));
			});

			it("get_string_from_frame_with_extreme_value", [&](){
				// Test frame values around 24h
				AssertThat(PhTimeCode::stringFromFrame(2159999, type).toStdString(), Equals("23:59:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(2160000, type).toStdString(), Equals("24:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(2160001, type).toStdString(), Equals("24:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(2176499, type).toStdString(), Equals("24:10:59:24"));

				// Test frame values around -24h
				AssertThat(PhTimeCode::stringFromFrame(-1, type).toStdString(), Equals("-00:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(-2159999, type).toStdString(), Equals("-23:59:59:24"));
				AssertThat(PhTimeCode::stringFromFrame(-2160000, type).toStdString(), Equals("-24:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(-2160001, type).toStdString(), Equals("-24:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(-2176499, type).toStdString(), Equals("-24:10:59:24"));
			});

			it("get_frame_from_string_special", [&](){
				// Test bad value for hh, mm, ss and ff
				AssertThat(f2s(PhTimeCode::frameFromString("12:23:34:30", type), type), Equals("12:23:34:00"));
				AssertThat(f2s(PhTimeCode::frameFromString("12:23:34:ff", type), type), Equals("12:23:34:00"));
				AssertThat(f2s(PhTimeCode::frameFromString("12:23:60:19", type), type), Equals("12:23:00:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("12:23:ss:19", type), type), Equals("12:23:00:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("12:60:34:19", type), type), Equals("12:00:34:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("12:mm:34:19", type), type), Equals("12:00:34:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("hh:23:34:19", type), type), Equals("00:23:34:19"));

				// Test extreme value for hh
				AssertThat(f2s(PhTimeCode::frameFromString("24:23:34:19", type), type), Equals("24:23:34:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("-1:23:34:19", type), type), Equals("-01:23:34:19"));

				// Test bad digit count : if there is not enough digit, frame are considered first, then seconds, minutes and hours
				AssertThat(f2s(PhTimeCode::frameFromString("19", type), type), Equals("00:00:00:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("34:19", type), type), Equals("00:00:34:19"));
				AssertThat(f2s(PhTimeCode::frameFromString("23:34:19", type), type), Equals("00:23:34:19"));

				// Test bad digit count : if there is to much digit, only the first four are taken in account
				AssertThat(f2s(PhTimeCode::frameFromString("12:23:34:19:12", type), type), Equals("12:23:34:19"));
			});

		});

		describe("2997fps", [](){
			PhTimeCodeType type = PhTimeCodeType2997;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::stringFromFrame(0, type).toStdString(), Equals("00:00:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(1, type).toStdString(), Equals("00:00:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(2, type).toStdString(), Equals("00:00:00:02"));
				AssertThat(PhTimeCode::stringFromFrame(29, type).toStdString(), Equals("00:00:00:29"));
				AssertThat(PhTimeCode::stringFromFrame(30, type).toStdString(), Equals("00:00:01:00"));
				AssertThat(PhTimeCode::stringFromFrame(59, type).toStdString(), Equals("00:00:01:29"));
				AssertThat(PhTimeCode::stringFromFrame(60, type).toStdString(), Equals("00:00:02:00"));
				AssertThat(PhTimeCode::stringFromFrame(1799, type).toStdString(), Equals("00:00:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(1800, type).toStdString(), Equals("00:01:00:02"));
				AssertThat(PhTimeCode::stringFromFrame(3597, type).toStdString(), Equals("00:01:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(3598, type).toStdString(), Equals("00:02:00:02"));
				AssertThat(PhTimeCode::stringFromFrame(5395, type).toStdString(), Equals("00:02:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(5396, type).toStdString(), Equals("00:03:00:02"));
				AssertThat(PhTimeCode::stringFromFrame(7193, type).toStdString(), Equals("00:03:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(7194, type).toStdString(), Equals("00:04:00:02"));
				AssertThat(PhTimeCode::stringFromFrame(17981, type).toStdString(), Equals("00:09:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(17982, type).toStdString(), Equals("00:10:00:00"));
				AssertThat(PhTimeCode::stringFromFrame(17983, type).toStdString(), Equals("00:10:00:01"));
				AssertThat(PhTimeCode::stringFromFrame(19781, type).toStdString(), Equals("00:10:59:29"));
				AssertThat(PhTimeCode::stringFromFrame(19782, type).toStdString(), Equals("00:11:00:02"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				AssertThat(PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));
				AssertThat(PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));
				AssertThat(PhTimeCode::frameFromString("00:00:00:29", type), Equals(29));
				AssertThat(PhTimeCode::frameFromString("00:00:01:00", type), Equals(30));
				AssertThat(PhTimeCode::frameFromString("00:00:01:29", type), Equals(59));
				AssertThat(PhTimeCode::frameFromString("00:00:02:00", type), Equals(60));
				AssertThat(PhTimeCode::frameFromString("00:00:59:29", type), Equals(1799));
				AssertThat(PhTimeCode::frameFromString("00:01:00:02", type), Equals(1800));
				AssertThat(PhTimeCode::frameFromString("00:01:59:29", type), Equals(3597));
				AssertThat(PhTimeCode::frameFromString("00:02:00:02", type), Equals(3598));
				AssertThat(PhTimeCode::frameFromString("00:02:59:29", type), Equals(5395));
				AssertThat(PhTimeCode::frameFromString("00:03:00:02", type), Equals(5396));
				AssertThat(PhTimeCode::frameFromString("00:03:59:29", type), Equals(7193));
				AssertThat(PhTimeCode::frameFromString("00:04:00:02", type), Equals(7194));
				AssertThat(PhTimeCode::frameFromString("00:09:59:29", type), Equals(17981));
				AssertThat(PhTimeCode::frameFromString("00:10:00:00", type), Equals(17982));
				AssertThat(PhTimeCode::frameFromString("00:10:00:01", type), Equals(17983));
				AssertThat(PhTimeCode::frameFromString("00:10:59:29", type), Equals(19781));
				AssertThat(PhTimeCode::frameFromString("00:11:00:02", type), Equals(19782));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				AssertThat(PhTimeCode::bcdFromFrame(0, type), Equals(0x000000u));
				AssertThat(PhTimeCode::bcdFromFrame(1, type), Equals(0x00000001u));
				AssertThat(PhTimeCode::bcdFromFrame(2, type), Equals(0x00000002u));
				AssertThat(PhTimeCode::bcdFromFrame(29, type), Equals(0x00000029u));
				AssertThat(PhTimeCode::bcdFromFrame(30, type), Equals(0x00000100u));
				AssertThat(PhTimeCode::bcdFromFrame(59, type), Equals(0x00000129u));
				AssertThat(PhTimeCode::bcdFromFrame(60, type), Equals(0x00000200u));
				AssertThat(PhTimeCode::bcdFromFrame(1799, type), Equals(0x00005929u));
				AssertThat(PhTimeCode::bcdFromFrame(1800, type), Equals(0x00010002u));
				AssertThat(PhTimeCode::bcdFromFrame(3597, type), Equals(0x00015929u));
				AssertThat(PhTimeCode::bcdFromFrame(3598, type), Equals(0x00020002u));
				AssertThat(PhTimeCode::bcdFromFrame(5395, type), Equals(0x00025929u));
				AssertThat(PhTimeCode::bcdFromFrame(5396, type), Equals(0x00030002u));
				AssertThat(PhTimeCode::bcdFromFrame(7193, type), Equals(0x00035929u));
				AssertThat(PhTimeCode::bcdFromFrame(7194, type), Equals(0x00040002u));
				AssertThat(PhTimeCode::bcdFromFrame(17981, type), Equals(0x00095929u));
				AssertThat(PhTimeCode::bcdFromFrame(17982, type), Equals(0x00100000u));
				AssertThat(PhTimeCode::bcdFromFrame(17983, type), Equals(0x00100001u));
				AssertThat(PhTimeCode::bcdFromFrame(19781, type), Equals(0x00105929u));
				AssertThat(PhTimeCode::bcdFromFrame(19782, type), Equals(0x00110002u));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				AssertThat(PhTimeCode::frameFromBcd(0x00000000, type), Equals(0));
				AssertThat(PhTimeCode::frameFromBcd(0x00000001, type), Equals(1));
				AssertThat(PhTimeCode::frameFromBcd(0x00000029, type), Equals(29));
				AssertThat(PhTimeCode::frameFromBcd(0x00000100, type), Equals(30));
				AssertThat(PhTimeCode::frameFromBcd(0x00000129, type), Equals(59));
				AssertThat(PhTimeCode::frameFromBcd(0x00000200, type), Equals(60));
				AssertThat(PhTimeCode::frameFromBcd(0x00005929, type), Equals(1799));
				AssertThat(PhTimeCode::frameFromBcd(0x00010002, type), Equals(1800));
				AssertThat(PhTimeCode::frameFromBcd(0x00015929, type), Equals(3597));
				AssertThat(PhTimeCode::frameFromBcd(0x00020002, type), Equals(3598));
				AssertThat(PhTimeCode::frameFromBcd(0x00025929, type), Equals(5395));
				AssertThat(PhTimeCode::frameFromBcd(0x00030002, type), Equals(5396));
				AssertThat(PhTimeCode::frameFromBcd(0x00035929, type), Equals(7193));
				AssertThat(PhTimeCode::frameFromBcd(0x00040002, type), Equals(7194));
				AssertThat(PhTimeCode::frameFromBcd(0x00095929, type), Equals(17981));
				AssertThat(PhTimeCode::frameFromBcd(0x00100000, type), Equals(17982));
				AssertThat(PhTimeCode::frameFromBcd(0x00100001, type), Equals(17983));
				AssertThat(PhTimeCode::frameFromBcd(0x00105929, type), Equals(19781));
				AssertThat(PhTimeCode::frameFromBcd(0x00110002, type), Equals(19782));
			});
		});
	});
});
