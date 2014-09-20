/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "bandit/bandit.h"

#include "PhTools/PhDebug.h"

#include "PhSync/PhTimeCode.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("timecode_test", []() {
		describe("2398fps", [](){
			PhTimeCodeType type = PhTimeCodeType2398;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				PhFrame frame = 0;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:00"));
				frame = 1;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:01"));
				frame = 23;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:23"));
				frame = 24;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:00"));
				frame = 47;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:23"));
				frame = 48;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:02:00"));
				frame = 1439;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:59:23"));
				frame = 1440;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:00:00"));
				frame = 2879;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:59:23"));
				frame = 2880;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:00:00"));
				frame = 4319;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:59:23"));
				frame = 4320;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:03:00:00"));
				frame = 14399;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:09:59:23"));
				frame = 14400;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:00"));
				frame = 14401;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:01"));
				frame = 15839;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:59:23"));
				frame = 15840;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				;
				AssertThat((int)PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));
				;
				AssertThat((int)PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:23", type), Equals(23));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:00", type), Equals(24));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:23", type), Equals(47));

				AssertThat((int)PhTimeCode::frameFromString("00:00:02:00", type), Equals(48));

				AssertThat((int)PhTimeCode::frameFromString("00:00:59:23", type), Equals(1439));

				AssertThat((int)PhTimeCode::frameFromString("00:01:00:00", type), Equals(1440));

				AssertThat((int)PhTimeCode::frameFromString("00:01:59:23", type), Equals(2879));

				AssertThat((int)PhTimeCode::frameFromString("00:02:00:00", type), Equals(2880));

				AssertThat((int)PhTimeCode::frameFromString("00:02:59:23", type), Equals(4319));

				AssertThat((int)PhTimeCode::frameFromString("00:03:00:00", type), Equals(4320));

				AssertThat((int)PhTimeCode::frameFromString("00:09:59:23", type), Equals(14399));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:00", type), Equals(14400));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:01", type), Equals(14401));

				AssertThat((int)PhTimeCode::frameFromString("00:10:59:23", type), Equals(15839));

				AssertThat((int)PhTimeCode::frameFromString("00:11:00:00", type), Equals(15840));
			});

			it("get_bcd_from_frame", [&](){
				PhFrame frame = 0;

				// Check various value of frame
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000000));
				frame = 1;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000001));
				frame = 23;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000023));
				frame = 24;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000100));
				frame = 47;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000123));
				frame = 48;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000200));
				frame = 1439;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00005923));
				frame = 1440;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00010000));
				frame = 2879;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00015923));
				frame = 2880;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00020000));
				frame = 4319;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00025923));
				frame = 4320;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00030000));
				frame = 14399;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00095923));
				frame = 14400;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100000));
				frame = 14401;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100001));
				frame = 15839;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00105923));
				frame = 15840;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00110000));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				unsigned int bcd = 0x00000000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(0));
				bcd = 0x00000001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1));
				bcd = 0x00000023;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(23));
				bcd = 0x00000100;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(24));
				bcd = 0x00000123;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(47));
				bcd = 0x00000200;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(48));
				bcd = 0x00005923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1439));
				bcd = 0x00010000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1440));
				bcd = 0x00015923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(2879));
				bcd = 0x00020000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(2880));
				bcd = 0x00025923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4319));
				bcd = 0x00030000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4320));
				bcd = 0x00095923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14399));
				bcd = 0x00100000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14400));
				bcd = 0x00100001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14401));
				bcd = 0x00105923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15839));
				bcd = 0x00110000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15840));
			});

		});

		describe("24fps", [](){
			PhTimeCodeType type = PhTimeCodeType24;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				PhFrame frame = 0;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:00"));
				frame = 1;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:01"));
				frame = 23;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:23"));
				frame = 24;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:00"));
				frame = 47;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:23"));
				frame = 48;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:02:00"));
				frame = 1439;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:59:23"));
				frame = 1440;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:00:00"));
				frame = 2879;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:59:23"));
				frame = 2880;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:00:00"));
				frame = 4319;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:59:23"));
				frame = 4320;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:03:00:00"));
				frame = 14399;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:09:59:23"));
				frame = 14400;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:00"));
				frame = 14401;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:01"));
				frame = 15839;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:59:23"));
				frame = 15840;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:23", type), Equals(23));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:00", type), Equals(24));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:23", type), Equals(47));

				AssertThat((int)PhTimeCode::frameFromString("00:00:02:00", type), Equals(48));

				AssertThat((int)PhTimeCode::frameFromString("00:00:59:23", type), Equals(1439));

				AssertThat((int)PhTimeCode::frameFromString("00:01:00:00", type), Equals(1440));

				AssertThat((int)PhTimeCode::frameFromString("00:01:59:23", type), Equals(2879));

				AssertThat((int)PhTimeCode::frameFromString("00:02:00:00", type), Equals(2880));

				AssertThat((int)PhTimeCode::frameFromString("00:02:59:23", type), Equals(4319));

				AssertThat((int)PhTimeCode::frameFromString("00:03:00:00", type), Equals(4320));

				AssertThat((int)PhTimeCode::frameFromString("00:09:59:23", type), Equals(14399));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:00", type), Equals(14400));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:01", type), Equals(14401));

				AssertThat((int)PhTimeCode::frameFromString("00:10:59:23", type), Equals(15839));
			});

			it("get_bcd_from_frame", [&](){
				PhFrame frame = 0;

				// Check various value of frame
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000000));
				frame = 1;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000001));
				frame = 23;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000023));
				frame = 24;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000100));
				frame = 47;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000123));
				frame = 48;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000200));
				frame = 1439;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00005923));
				frame = 1440;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00010000));
				frame = 2879;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00015923));
				frame = 2880;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00020000));
				frame = 4319;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00025923));
				frame = 4320;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00030000));
				frame = 14399;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00095923));
				frame = 14400;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100000));
				frame = 14401;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100001));
				frame = 15839;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00105923));
				frame = 15840;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00110000));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				unsigned int bcd = 0x00000000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(0));
				bcd = 0x00000001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1));
				bcd = 0x00000023;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(23));
				bcd = 0x00000100;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(24));
				bcd = 0x00000123;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(47));
				bcd = 0x00000200;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(48));
				bcd = 0x00005923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1439));
				bcd = 0x00010000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1440));
				bcd = 0x00015923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(2879));
				bcd = 0x00020000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(2880));
				bcd = 0x00025923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4319));
				bcd = 0x00030000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4320));
				bcd = 0x00095923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14399));
				bcd = 0x00100000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14400));
				bcd = 0x00100001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14401));
				bcd = 0x00105923;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15839));
				bcd = 0x00110000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15840));
			});


		});

		describe("25fps", [](){
			PhTimeCodeType type = PhTimeCodeType25;
			it("get_string_from_frame", [&](){
				// Check various value of frame
				PhFrame frame = 0;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:00"));
				frame = 1;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:01"));
				frame = 24;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:24"));
				frame = 25;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:00"));
				frame = 49;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:24"));
				frame = 50;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:02:00"));
				frame = 1499;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:59:24"));
				frame = 1500;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:00:00"));
				frame = 2999;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:59:24"));
				frame = 3000;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:00:00"));
				frame = 4499;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:59:24"));
				frame = 4500;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:03:00:00"));
				frame = 14999;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:09:59:24"));
				frame = 15000;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:00"));
				frame = 15001;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:01"));
				frame = 16499;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:59:24"));
				frame = 16500;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:11:00:00"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string
				AssertThat((int)PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:24", type), Equals(24));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:00", type), Equals(25));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:24", type), Equals(49));

				AssertThat((int)PhTimeCode::frameFromString("00:00:02:00", type), Equals(50));

				AssertThat((int)PhTimeCode::frameFromString("00:00:59:24", type), Equals(1499));

				AssertThat((int)PhTimeCode::frameFromString("00:01:00:00", type), Equals(1500));

				AssertThat((int)PhTimeCode::frameFromString("00:01:59:24", type), Equals(2999));

				AssertThat((int)PhTimeCode::frameFromString("00:02:00:00", type), Equals(3000));

				AssertThat((int)PhTimeCode::frameFromString("00:02:59:24", type), Equals(4499));

				AssertThat((int)PhTimeCode::frameFromString("00:03:00:00", type), Equals(4500));

				AssertThat((int)PhTimeCode::frameFromString("00:09:59:24", type), Equals(14999));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:00", type), Equals(15000));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:01", type), Equals(15001));

				AssertThat((int)PhTimeCode::frameFromString("00:10:59:24", type), Equals(16499));

				AssertThat((int)PhTimeCode::frameFromString("00:11:00:00", type), Equals(16500));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				PhFrame frame = 0;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000000));
				frame = 1;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000001));
				frame = 24;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000024));
				frame = 25;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000100));
				frame = 49;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000124));
				frame = 50;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000200));
				frame = 1499;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00005924));
				frame = 1500;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00010000));
				frame = 2999;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00015924));
				frame = 3000;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00020000));
				frame = 4499;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00025924));
				frame = 4500;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00030000));
				frame = 14999;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00095924));
				frame = 15000;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100000));
				frame = 15001;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100001));
				frame = 16499;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00105924));
				frame = 16500;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00110000));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				unsigned int bcd = 0x00000000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(0));
				bcd = 0x00000001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1));
				bcd = 0x00000024;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(24));
				bcd = 0x00000100;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(25));
				bcd = 0x00000124;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(49));
				bcd = 0x00000200;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(50));
				bcd = 0x00005924;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1499));
				bcd = 0x00010000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1500));
				bcd = 0x00015924;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(2999));
				bcd = 0x00020000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(3000));
				bcd = 0x00025924;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4499));
				bcd = 0x00030000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(4500));
				bcd = 0x00095924;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(14999));
				bcd = 0x00100000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15000));
				bcd = 0x00100001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(15001));
				bcd = 0x00105924;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(16499));
				bcd = 0x00110000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(16500));
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
				PhFrame frame = 0;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:00"));
				frame = 1;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:01"));
				frame = 2;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:02"));
				frame = 29;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:00:29"));
				frame = 30;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:00"));
				frame = 59;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:01:29"));
				frame = 60;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:02:00"));
				frame = 1799;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:00:59:29"));
				frame = 1800;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:00:02"));
				frame = 3597;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:01:59:29"));
				frame = 3598;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:00:02"));
				frame = 5395;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:02:59:29"));
				frame = 5396;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:03:00:02"));
				frame = 7193;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:03:59:29"));
				frame = 7194;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:04:00:02"));
				frame = 17981;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:09:59:29"));
				frame = 17982;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:00"));
				frame = 17983;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:00:01"));
				frame = 19781;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:10:59:29"));
				frame = 19782;
				AssertThat(PhTimeCode::stringFromFrame(frame, type).toStdString(), Equals("00:11:00:02"));
			});

			it("get_frame_from_string", [&](){
				// Check various value of string

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:00", type), Equals(0));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:01", type), Equals(1));

				AssertThat((int)PhTimeCode::frameFromString("00:00:00:29", type), Equals(29));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:00", type), Equals(30));

				AssertThat((int)PhTimeCode::frameFromString("00:00:01:29", type), Equals(59));

				AssertThat((int)PhTimeCode::frameFromString("00:00:02:00", type), Equals(60));

				AssertThat((int)PhTimeCode::frameFromString("00:00:59:29", type), Equals(1799));

				AssertThat((int)PhTimeCode::frameFromString("00:01:00:02", type), Equals(1800));

				AssertThat((int)PhTimeCode::frameFromString("00:01:59:29", type), Equals(3597));

				AssertThat((int)PhTimeCode::frameFromString("00:02:00:02", type), Equals(3598));

				AssertThat((int)PhTimeCode::frameFromString("00:02:59:29", type), Equals(5395));

				AssertThat((int)PhTimeCode::frameFromString("00:03:00:02", type), Equals(5396));

				AssertThat((int)PhTimeCode::frameFromString("00:03:59:29", type), Equals(7193));

				AssertThat((int)PhTimeCode::frameFromString("00:04:00:02", type), Equals(7194));

				AssertThat((int)PhTimeCode::frameFromString("00:09:59:29", type), Equals(17981));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:00", type), Equals(17982));

				AssertThat((int)PhTimeCode::frameFromString("00:10:00:01", type), Equals(17983));

				AssertThat((int)PhTimeCode::frameFromString("00:10:59:29", type), Equals(19781));

				AssertThat((int)PhTimeCode::frameFromString("00:11:00:02", type), Equals(19782));
			});

			it("get_bcd_from_frame", [&](){
				// Check various value of frame
				PhFrame frame = 0;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x000000));
				frame = 1;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000001));
				frame = 2;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000002));
				frame = 29;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000029));
				frame = 30;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000100));
				frame = 59;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000129));
				frame = 60;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00000200));
				frame = 1799;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00005929));
				frame = 1800;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00010002));
				frame = 3597;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00015929));
				frame = 3598;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00020002));
				frame = 5395;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00025929));
				frame = 5396;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00030002));
				frame = 7193;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00035929));
				frame = 7194;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00040002));
				frame = 17981;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00095929));
				frame = 17982;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100000));
				frame = 17983;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00100001));
				frame = 19781;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00105929));
				frame = 19782;
				AssertThat((int)PhTimeCode::bcdFromFrame(frame, type), Equals(0x00110002));
			});

			it("get_frame_from_bcd", [&](){
				// Check various value of binary coded decimal (bcd)
				unsigned int bcd = 0x00000000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(0));
				bcd = 0x00000001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1));
				bcd = 0x00000029;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(29));
				bcd = 0x00000100;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(30));
				bcd = 0x00000129;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(59));
				bcd = 0x00000200;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(60));
				bcd = 0x00005929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1799));
				bcd = 0x00010002;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(1800));
				bcd = 0x00015929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(3597));
				bcd = 0x00020002;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(3598));
				bcd = 0x00025929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(5395));
				bcd = 0x00030002;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(5396));
				bcd = 0x00035929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(7193));
				bcd = 0x00040002;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(7194));
				bcd = 0x00095929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(17981));
				bcd = 0x00100000;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(17982));
				bcd = 0x00100001;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(17983));
				bcd = 0x00105929;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(19781));
				bcd = 0x00110002;
				AssertThat((int)PhTimeCode::frameFromBcd(bcd, type), Equals(19782));
			});
		});
	});
});
