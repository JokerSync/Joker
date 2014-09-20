/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhStrip/PhStripDoc.h"

#include "CommonSpec.h"

using namespace igloo;

Describe(import) {
	Describe(detx) {
		PhStripDoc doc;

		void SetUp() {
			Assert::That(doc.importDetXFile("test01.detx"), IsTrue(), __FILE__, __LINE__);
		}

		It(import_text) {
			Assert::That(doc.texts().count(), Equals(6), __FILE__, __LINE__);

			Assert::That(doc.texts()[0]->content().toStdString(), Equals("Simple sentence"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"), __FILE__, __LINE__);
			Assert::That(doc.texts()[0]->people(), Equals(doc.peopleByName("Jeanne")), __FILE__, __LINE__);
			Assert::That(doc.texts()[0]->y(), Equals(0.25f), __FILE__, __LINE__);

			Assert::That(doc.texts()[1]->content().toStdString(), Equals("Composed "), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"), __FILE__, __LINE__);

			Assert::That(doc.texts()[2]->content().toStdString(), Equals("sentence"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:06:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"), __FILE__, __LINE__);

			Assert::That(doc.texts()[3]->content().toStdString(), Equals("Simple off sentence"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"), __FILE__, __LINE__);

			Assert::That(doc.texts()[4]->content().toStdString(), Equals("Composed sentence with off"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"), __FILE__, __LINE__);

			Assert::That(doc.texts()[5]->content().toStdString(), Equals("Sentence with out not linked"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[5]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.texts()[5]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"), __FILE__, __LINE__);
		}

		It(import_detect) {
			Assert::That(doc.detects().count(), Equals(6), __FILE__, __LINE__);

			Assert::That(doc.detects()[0]->people(), Equals(doc.peopleByName("Jeanne")), __FILE__, __LINE__);
			Assert::That(doc.detects()[0]->type(), Equals(PhStripDetect::On), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"), __FILE__, __LINE__);
			Assert::That(doc.detects()[0]->y(), Equals(0.25f), __FILE__, __LINE__);

			Assert::That(doc.detects()[1]->people(), Equals(doc.peopleByName("Sue")), __FILE__, __LINE__);
			Assert::That(doc.detects()[1]->type(), Equals(PhStripDetect::On), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"), __FILE__, __LINE__);
			Assert::That(doc.detects()[1]->y(), Equals(0.5f), __FILE__, __LINE__);


			Assert::That(doc.detects()[2]->people(), Equals(doc.peopleByName("Paul")), __FILE__, __LINE__);
			Assert::That(doc.detects()[2]->type(), Equals(PhStripDetect::Off), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"), __FILE__, __LINE__);
			Assert::That(doc.detects()[2]->y(), Equals(0.25f), __FILE__, __LINE__);


			Assert::That(doc.detects()[3]->people(), Equals(doc.peopleByName("Sue")), __FILE__, __LINE__);
			Assert::That(doc.detects()[3]->type(), Equals(PhStripDetect::Off), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"), __FILE__, __LINE__);
			Assert::That(doc.detects()[3]->y(), Equals(0.5f), __FILE__, __LINE__);

			Assert::That(doc.detects()[4]->people(), Equals(doc.peopleByName("Sue")), __FILE__, __LINE__);
			Assert::That(doc.detects()[4]->type(), Equals(PhStripDetect::Off), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:20:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:22:00"), __FILE__, __LINE__);
			Assert::That(doc.detects()[4]->y(), Equals(0.5f), __FILE__, __LINE__);


			Assert::That(doc.detects()[5]->people(), Equals(doc.peopleByName("Sue")), __FILE__, __LINE__);
			Assert::That(doc.detects()[5]->type(), Equals(PhStripDetect::On), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[5]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"), __FILE__, __LINE__);
			Assert::That(t2s(doc.detects()[5]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"), __FILE__, __LINE__);
			Assert::That(doc.detects()[5]->y(), Equals(0.5f), __FILE__, __LINE__);
		}
	};

	Describe(mos) {
		PhStripDoc doc;

		It(import_test01) {
			Assert::That(doc.importMosFile("test01.mos"));

			Assert::That(doc.authorName().toStdString(), Equals("auteur"));

			Assert::That(doc.videoFilePath().toStdString(), Equals(""));
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:00:00:00"));

			Assert::That(doc.title().toStdString(), Equals("Titre VO"));
			Assert::That(doc.season().toStdString(), Equals("saison"));
			Assert::That(doc.episode().toStdString(), Equals("episode"));

			// Test peoples
			Assert::That(doc.peoples().count(), Equals(1));
			PhPeople * people = doc.peopleByName("Nom personnage");
			Assert::That(people != NULL);
			Assert::That(people->name().toStdString(), Equals("Nom personnage"));

			// Test cuts
			Assert::That(doc.cuts().count(), Equals(0));

			// Test loop
			Assert::That(doc.loops().count(), Equals(0));

			// Test texts
			Assert::That(doc.texts().count(), Equals(3));
			Assert::That(doc.texts()[0]->people(), Equals(people));
			Assert::That(doc.texts()[0]->content().toStdString(), Equals("Ceci "));
			Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:00:20"));
			Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:01:12"));

			Assert::That(doc.texts()[1]->content().toStdString(), Equals("est un"));
			Assert::That(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("00:00:01:12"));
			Assert::That(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("00:00:01:16"));

			Assert::That(doc.texts()[2]->content().toStdString(), Equals(" test."));
			Assert::That(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("00:00:01:16"));
			Assert::That(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("00:00:02:03"));

			// Detect test
			//#warning TODO more test on detect
			Assert::That(doc.detects().count(), Equals(4));
		}

		It(import_test02) {
			Assert::That(doc.importMosFile("test02.mos"));

			Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

			// Test peoples
			Assert::That(doc.peoples().count(), Equals(2));

			PhPeople * pierre = doc.peopleByName("Pierre");
			Assert::That(pierre != NULL);
			Assert::That(pierre->name().toStdString(), Equals("Pierre"));

			PhPeople * marie = doc.peopleByName("Marie");
			Assert::That(marie != NULL);
			Assert::That(marie->name().toStdString(), Equals("Marie"));

			// Test cuts
			Assert::That(doc.cuts().count(), Equals(1));
			Assert::That(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

			// Test loops
			Assert::That(doc.loops().count(), Equals(1));
			Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
			Assert::That(doc.loops()[0]->label().toStdString(), Equals("1"));

			// Test texts
			Assert::That(doc.texts().count(), Equals(2));
			Assert::That(doc.texts()[0]->people(), Equals(pierre));
			Assert::That(doc.texts()[0]->y(), Equals(0.0f));
			Assert::That(doc.texts()[0]->content().toStdString(), Equals("Bonjour, Marie."));
			Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
			Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:02:00"));

			Assert::That(doc.texts()[1]->people(), Equals(marie));
			Assert::That(doc.texts()[1]->y(), Equals(0.6f));
			Assert::That(doc.texts()[1]->content().toStdString(), Equals("Bonjour, Pierre."));
			Assert::That(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
			Assert::That(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

			// Detect test
			Assert::That(doc.detects().count(), Equals(0));
		}

		It(import_test03) {
			Assert::That(doc.importMosFile("test03.mos"));

			Assert::That(doc.videoFilePath().toStdString(), Equals("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("05:59:50:00"));

			// Test peoples
			Assert::That(doc.peoples().count(), Equals(2));

			PhPeople * noName = doc.peopleByName("");
			Assert::That(noName != NULL);
			Assert::That(noName->name().toStdString(), Equals(""));

			PhPeople * denis = doc.peopleByName("Denis");
			Assert::That(denis != NULL);
			Assert::That(denis->name().toStdString(), Equals("Denis"));

			// Test cuts
			Assert::That(doc.cuts().count(), Equals(29));
			Assert::That(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType24), Equals("05:59:50:00"));
			Assert::That(t2s(doc.cuts()[1]->timeIn(), PhTimeCodeType24), Equals("05:59:58:00"));
			Assert::That(t2s(doc.cuts()[2]->timeIn(), PhTimeCodeType24), Equals("06:00:00:00"));
			Assert::That(t2s(doc.cuts()[3]->timeIn(), PhTimeCodeType24), Equals("06:00:05:22"));
			Assert::That(t2s(doc.cuts()[4]->timeIn(), PhTimeCodeType24), Equals("06:00:08:05"));

			// Test loops
			Assert::That(doc.loops().count(), Equals(2));
			Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType24), Equals("06:00:01:00"));
			Assert::That(doc.loops()[0]->label().toStdString(), Equals("30"));
			Assert::That(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType24), Equals("06:01:15:00"));
			Assert::That(doc.loops()[1]->label().toStdString(), Equals("off"));

			// Test texts
			Assert::That(doc.texts(false).count(), Equals(206));
			Assert::That(doc.texts(true).count(), Equals(0));

			Assert::That(doc.texts()[0]->people(), Equals(noName));
			Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("05:59:49:05"));
			Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("05:59:49:20"));

			Assert::That(doc.texts()[0]->y(), Equals(2.0f));
			Assert::That(doc.texts()[0]->height(), Equals(0.2f));

			Assert::That(doc.texts()[0]->content().toStdString(), Equals("Départ bob 6"));
			Assert::That(doc.texts()[1]->content().toStdString(), Equals("(X)"));
			Assert::That(doc.texts()[2]->content().toStdString(), Equals("05.59.50.00"));
			Assert::That(doc.texts()[3]->content().toStdString(), Equals("1000"));
			Assert::That(doc.texts()[4]->content().toStdString(), Equals("(X)"));
			Assert::That(doc.texts()[5]->content().toStdString(), Equals("P.I"));

			Assert::That(doc.texts()[9]->people(), Equals(denis));
			Assert::That(t2s(doc.texts()[9]->timeIn(), PhTimeCodeType24), Equals("06:00:07:23"));
			Assert::That(t2s(doc.texts()[9]->timeOut(), PhTimeCodeType24), Equals("06:00:08:03"));
			Assert::That(t2s(doc.texts()[10]->timeIn(), PhTimeCodeType24), Equals("06:00:08:03"));
			Assert::That(t2s(doc.texts()[10]->timeOut(), PhTimeCodeType24), Equals("06:00:08:07"));

			Assert::That(doc.texts()[9]->content().toStdString(), Equals("Tu "));
			Assert::That(doc.texts()[10]->content().toStdString(), Equals("tra"));
			Assert::That(doc.texts()[11]->content().toStdString(), Equals("vaillais "));
			Assert::That(doc.texts()[12]->content().toStdString(), Equals("pas "));
			Assert::That(doc.texts()[13]->content().toStdString(), Equals("ce "));
			Assert::That(doc.texts()[14]->content().toStdString(), Equals("soir !"));

			Assert::That(t2s(doc.texts()[14]->timeOut(), PhTimeCodeType24), Equals("06:00:09:06"));

			// Detect test
			Assert::That(doc.detects().count(), Equals(24));
			//#warning TODO more test on detect
		}

		It(import_test04) {
			Assert::That(doc.importMosFile("test04.mos"));

			// Test video info
			Assert::That(doc.videoFilePath().toStdString(), Equals("D:\\Ressources\\Mosaic\\Utilisateurs\\Yves\\Bold 5704\\5704.mov"));
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:39:24"));

			// Test loops
			Assert::That(doc.loops().count(), Equals(27));
			Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
			Assert::That(doc.loops()[0]->label().toStdString(), Equals("1"));
			Assert::That(doc.loops()[1]->label().toStdString(), Equals("2"));
			Assert::That(doc.loops()[2]->label().toStdString(), Equals("3"));
			Assert::That(doc.loops()[3]->label().toStdString(), Equals("off"));
			Assert::That(doc.loops()[4]->label().toStdString(), Equals("4"));
			Assert::That(t2s(doc.loops()[5]->timeIn(), PhTimeCodeType25), Equals("01:02:56:02"));
			Assert::That(doc.loops()[5]->label().toStdString(), Equals("5"));
			Assert::That(t2s(doc.loops()[25]->timeIn(), PhTimeCodeType25), Equals("01:18:14:11"));
			Assert::That(doc.loops()[25]->label().toStdString(), Equals("25"));
			Assert::That(t2s(doc.loops()[26]->timeIn(), PhTimeCodeType25), Equals("01:19:01:01"));
			Assert::That(doc.loops()[26]->label().toStdString(), Equals("off"));

			// Test texts
			Assert::That(doc.texts().count(), Equals(1118));
			Assert::That(doc.texts(true).count(), Equals(4329));
			Assert::That(doc.detects().count(), Equals(4552));

			Assert::That(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:13"));
			Assert::That(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:02:20"));
			Assert::That(doc.detects()[0]->type(), Equals(PhStripDetect::Off));
		}

		It(import_mos24) {
			Assert::That(doc.importMosFile("mos24.mos"));

			Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Gilles\\Desktop\\Sonic_EP_01_mix_VA.mov"));
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("00:59:58:00"));

			Assert::That(doc.texts().count(), Equals(1));

			Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("01:00:01:00"));
			Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("01:00:03:00"));
		}

		It(import_mos25) {
			Assert::That(doc.importMosFile("mos25.mos"));

			Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Gilles\\Desktop\\Get Blake\\Get Blake 115\\GBL_EP115_Online_Master_VA_h264_TCI.mov"));
			Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
			Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("09:59:58:00"));
		}
	};

	Describe(drb) {
		Describe(v5) {
			PhStripDoc doc;

			It(import_drb01) {
				Assert::That(doc.openStripFile("drb01.drb"));

				Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\SYNCHRO USER\\Downloads\\TheManWithTheGoldenArm_25fps_high\\TheManWithTheGoldenArm_25fps_high.mov"));

				Assert::That(doc.loops().count(), Equals(0));

				Assert::That(doc.peoples().count(), Equals(2));
				Assert::That(doc.peoples()[0]->name().toStdString(), Equals("Personnage 1"));
				Assert::That(doc.peoples()[1]->name().toStdString(), Equals("Personnage 2"));

				Assert::That(doc.texts().count(), Equals(1));
				Assert::That(doc.texts()[0]->people()->name().toStdString(), Equals("Personnage 2"));

				Assert::That(doc.texts()[0]->y(), Equals(0.36f));
				Assert::That(doc.texts()[0]->height(), EqualsWithDelta(0.22666667f, 0.00001f));

#warning /// @todo should be the same than syn6
				Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:02:12:10"));
				Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:02:44:07"));
			}

			It(import_drb02) {
				Assert::That(doc.openStripFile("drb02.drb"));

				Assert::That(doc.videoFilePath().toStdString(), Equals("D:\\NED 201.mov"));
				Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:04:20"));

				Assert::That(doc.loops().count(), Equals(21));

				for(int i = 0; i < 21; i++)
					Assert::That(doc.loops().at(i)->label().toStdString(), Equals(QString::number(i + 1).toStdString()));

				Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:39:02"));
				Assert::That(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType25), Equals("01:02:14:23"));

				Assert::That(doc.peoples().count(), Equals(28));

				Assert::That(doc.peoples()[0]->name().toStdString(), Equals("Intervenant 1"));
				Assert::That(doc.peoples()[1]->name().toStdString(), Equals("ned"));
				Assert::That(doc.peoples()[2]->name().toStdString(), Equals("moze"));
				Assert::That(doc.peoples()[3]->name().toStdString(), Equals("suzie"));

				Assert::That(doc.texts().count(), Equals(546));
				Assert::That(doc.texts()[0]->people()->name().toStdString(), Equals("ned"));
				Assert::That(doc.texts()[0]->y(), Equals(0.213333338f));
				Assert::That(doc.texts()[0]->height(), Equals(0.28666667f));
				Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
				Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:01:05"));
			}

			It(import_drb03) {
				Assert::That(doc.openStripFile("drb03.drb"));

				Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
				Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

				Assert::That(doc.loops().count(), Equals(1));

				Assert::That(doc.loops().at(0)->label().toStdString(), Equals("1"));

				Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

				Assert::That(doc.peoples().count(), Equals(2));

				Assert::That(doc.peoples()[0]->name().toStdString(), Equals("Pierre"));
				Assert::That(doc.peoples()[1]->name().toStdString(), Equals("Marie"));

				Assert::That(doc.texts().count(), Equals(2));

				Assert::That(doc.texts()[0]->people()->name().toStdString(), Equals("Pierre"));
				Assert::That(doc.texts()[0]->y(), Equals(0.066666667f));
				Assert::That(doc.texts()[0]->height(), Equals(0.22666667f));
				Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
				Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

				Assert::That(doc.texts()[1]->people()->name().toStdString(), Equals("Marie"));
				Assert::That(doc.texts()[1]->y(), Equals(0.7f));
				Assert::That(doc.texts()[1]->height(), Equals(0.22666667f));
				Assert::That(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
				Assert::That(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

				Assert::That(doc.cuts().count(), Equals(1));
				Assert::That(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
			}
		};

		Describe(v6) {
			PhStripDoc doc;
			It(import_test01) {
				Assert::That(doc.openStripFile("test01.syn6"));

				Assert::That(doc.peoples().count(), Equals(2));
				Assert::That(doc.peoples()[0]->name().toStdString(), Equals("Personnage 1"));
				Assert::That(doc.peoples()[1]->name().toStdString(), Equals("Personnage 2"));

				Assert::That(doc.texts().count(), Equals(1));
				Assert::That(doc.texts()[0]->people()->name().toStdString(), Equals("Personnage 1"));

				Assert::That(doc.texts()[0]->y(), Equals(0.36f));
				Assert::That(doc.texts()[0]->height(), Equals(0.22666667f));

				Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:03:11"));
				Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:06:14"));
			}

			It(import_test02) {
				Assert::That(doc.openStripFile("test02.syn6"));

				Assert::That(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
				Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

				Assert::That(doc.loops().count(), Equals(1));

				Assert::That(doc.loops().at(0)->label().toStdString(), Equals("1"));

				Assert::That(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

				Assert::That(doc.peoples().count(), Equals(2));

				Assert::That(doc.peoples()[0]->name().toStdString(), Equals("Pierre"));
				Assert::That(doc.peoples()[1]->name().toStdString(), Equals("Marie"));

				Assert::That(doc.texts().count(), Equals(2));

				Assert::That(doc.texts()[0]->people()->name().toStdString(), Equals("Pierre"));
				Assert::That(doc.texts()[0]->y(), Equals(0.066666667f));
				Assert::That(doc.texts()[0]->height(), Equals(0.22666667f));
				Assert::That(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
				Assert::That(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

				Assert::That(doc.texts()[1]->people()->name().toStdString(), Equals("Marie"));
				Assert::That(doc.texts()[1]->y(), Equals(0.7f));
				Assert::That(doc.texts()[1]->height(), Equals(0.22666667f));
				Assert::That(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
				Assert::That(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

				Assert::That(doc.cuts().count(), Equals(1));
				Assert::That(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
			}
		};
	};
	//void StripDocTest::openStripFileTest()
	//{
	//	PhStripDoc doc;

	//	// DetX test
	//	Assert::That(doc.openStripFile("test01.detx"));
	//	Assert::That(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:00:16:00"));
	//	Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

	//	// Mos test
	//	Assert::That(doc.openStripFile("test03.mos"));
	//	Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));

	//	// Strip file
	//	Assert::That(doc.openStripFile("test.strip"));
	//	Assert::That(doc.forceRatio169(), true);
	//	Assert::That(doc.videoFilePath().toStdString(), Equals("test01.mov"));
	//	Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:00:00"));
	//	Assert::That(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:02:03:04"));

	//	// Test regular joker file linked to detx
	//	Assert::That(doc.openStripFile("test01.joker"));
	//	Assert::That(doc.filePath(), Equals("test01.detx"));
	//	Assert::That(doc.videoFilePath().toStdString(), Equals("test01.mov"));
	//	Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
	//	Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
	//	Assert::That(doc.forceRatio169(), true);
	//	Assert::That(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:30:00:00"));

	//	Assert::That(doc.title(), Equals("Title test"));
	//	Assert::That(doc.generator(), Equals("Cappella v0.12.5, 1"));

	//	// Test regular joker file linked to mos
	//	Assert::That(doc.openStripFile("test02.joker"));
	//	Assert::That(doc.filePath(), Equals("test03.mos"));
	//	Assert::That(doc.videoFilePath().toStdString(), Equals("test02.mov"));
	//	Assert::That(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
	//	Assert::That(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:03:00:00"));
	//	Assert::That(doc.forceRatio169(), false);
	//	Assert::That(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:20:00:00"));

	//	Assert::That(doc.title(), Equals("Au fil d'Ariane"));
	//	Assert::That(doc.generator(), Equals("Mosaic"));

	//	// Test bad files
	//	Assert::That(QFile::exists("bad_tag.joker"));
	//	Assert::That(!doc.openStripFile("bad_tag.joker"));

	//	Assert::That(QFile::exists("empty.joker"));
	//	Assert::That(!doc.openStripFile("empty.joker"));

	//	// Test accepted files
	//	Assert::That(QFile::exists("empty_root.joker"));
	//	Assert::That(doc.openStripFile("empty_root.joker"));

	//	Assert::That(QFile::exists("empty_meta.joker"));
	//	Assert::That(doc.openStripFile("empty_meta.joker"));

	//#warning /// @todo Test video frame rate
	//}

	//void StripDocTest::openSaveTest01()
	//{
	//	PhStripDoc doc1;

	//	Assert::That(doc1.openStripFile("test01.joker"));

	//	Assert::That(doc1.saveStripFile("save01.joker", s2t("01:01:01:01", PhTimeCodeType25)));

	//	PhStripDoc doc2;

	//	Assert::That(doc2.openStripFile("save01.joker"));

	//	Assert::That(doc2.filePath(), doc1.filePath());
	//	Assert::That(doc2.videoFilePath(), doc1.videoFilePath());
	//	Assert::That(doc2.videoTimeCodeType(), Equals(PhTimeCodeType25));
	//	Assert::That(t2s(doc2.videoTimeIn(), PhTimeCodeType25), t2s(doc1.videoTimeIn(), PhTimeCodeType25));
	//	Assert::That(doc2.forceRatio169(), true);
	//	Assert::That(doc2.videoDeinterlace(), false);
	//	Assert::That(t2s(doc2.lastTime(), PhTimeCodeType25), Equals("01:01:01:01"));

	//	Assert::That(doc2.title(), doc1.title());
	//	Assert::That(doc2.generator(), doc1.generator());
	//}

	//void StripDocTest::openSaveTest02()
	//{
	//	PhStripDoc doc1;

	//	Assert::That(doc1.openStripFile("test02.joker"));

	//	Assert::That(doc1.saveStripFile("save02.joker", s2t("02:02:02:02", PhTimeCodeType25)));

	//	PhStripDoc doc2;

	//	Assert::That(doc2.openStripFile("save02.joker"));

	//	Assert::That(doc2.filePath(), doc1.filePath());
	//	Assert::That(doc2.videoFilePath(), doc1.videoFilePath());
	//	Assert::That(t2s(doc2.videoTimeIn(), PhTimeCodeType25), t2s(doc1.videoTimeIn(), PhTimeCodeType25));
	//	Assert::That(doc2.forceRatio169(), false);
	//	Assert::That(doc2.videoDeinterlace(), true);
	//	Assert::That(t2s(doc2.lastTime(), PhTimeCodeType25), Equals("02:02:02:02"));

	//	Assert::That(doc2.title(), doc1.title());
	//	Assert::That(doc2.generator(), doc1.generator());
	//}

	//void StripDocTest::getPeopleByNameTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));

	//	Assert::That(doc.peopleByName("Jeanne")->name().toStdString(), Equals("Jeanne"));
	//	Assert::That(doc.peopleByName("Sue")->name().toStdString(), Equals("Sue"));
	//	Assert::That(doc.peopleByName("Paul")->name().toStdString(), Equals("Paul"));
	//	Assert::That(doc.peopleByName("Bob") == NULL);
	//}

	//void StripDocTest::getPreviousElementTimeTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	Assert::That(t2s(doc.previousElementTime(s2t("23:00:00:00", tcType)), tcType), Equals("01:01:00:00"));
	//	Assert::That(t2s(doc.previousElementTime(s2t("01:01:00:00", tcType)), tcType), Equals("01:00:30:00"));
	//	Assert::That(t2s(doc.previousElementTime(s2t("01:00:30:00", tcType)), tcType), Equals("01:00:15:00"));
	//	Assert::That(t2s(doc.previousElementTime(s2t("01:00:15:00", tcType)), tcType), Equals("01:00:12:00"));
	//	Assert::That(doc.previousElementTime(s2t("01:00:00:00", tcType)), PHTIMEMIN);
	//}

	//void StripDocTest::getNextElementTimeTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	Assert::That(t2s(doc.nextElementTime(s2t("00:00:00:00", tcType)), tcType), Equals("01:00:00:00"));
	//	Assert::That(t2s(doc.nextElementTime(s2t("01:00:00:00", tcType)), tcType), Equals("01:00:01:00"));
	//	Assert::That(t2s(doc.nextElementTime(s2t("01:00:01:00", tcType)), tcType), Equals("01:00:02:00"));
	//	Assert::That(t2s(doc.nextElementTime(s2t("01:00:02:00", tcType)), tcType), Equals("01:00:05:00"));
	//	Assert::That(t2s(doc.nextElementTime(s2t("01:00:17:00", tcType)), tcType), Equals("01:00:30:00"));
	//	Assert::That(t2s(doc.nextElementTime(s2t("01:00:30:00", tcType)), tcType), Equals("01:01:00:00"));
	//	Assert::That(doc.nextElementTime(s2t("01:01:00:00", tcType)), PHTIMEMAX);

	//}

	//void StripDocTest::getNextTextTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	Assert::That(t2s(doc.nextText(s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:02:00"));
	//	Assert::That(t2s(doc.nextText(s2t("01:00:02:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
	//	Assert::That(t2s(doc.nextText(s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
	//	Assert::That(t2s(doc.nextText(s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:12:00"));
	//	Assert::That(t2s(doc.nextText(s2t("01:00:12:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
	//	Assert::That(t2s(doc.nextText(s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
	//	Assert::That(doc.nextText(s2t("01:00:30:00", tcType)) == NULL);
	//}

	//void StripDocTest::getNextTextTestByPeople()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	PhPeople* sue = doc.peopleByName("Sue");

	//	Assert::That(t2s(doc.nextText(sue, s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
	//	Assert::That(t2s(doc.nextText(sue, s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
	//	Assert::That(t2s(doc.nextText(sue, s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
	//	Assert::That(t2s(doc.nextText(sue, s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
	//	Assert::That(doc.nextText(sue, s2t("01:00:30:00", tcType)) == NULL);
	//}

	//void StripDocTest::getNextTextTestByPeopleList()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	QList<PhPeople*> peopleList;
	//	peopleList.append(doc.peopleByName("Sue"));
	//	peopleList.append(doc.peopleByName("Paul"));

	//	Assert::That(t2s(doc.nextText(peopleList, s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
	//	Assert::That(t2s(doc.nextText(peopleList, s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
	//	Assert::That(t2s(doc.nextText(peopleList, s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:12:00"));
	//	Assert::That(t2s(doc.nextText(peopleList, s2t("01:00:12:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
	//	Assert::That(t2s(doc.nextText(peopleList, s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
	//	Assert::That(doc.nextText(peopleList, s2t("01:00:30:00", tcType)) == NULL);
	//}

	//void StripDocTest::getNextLoopTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	Assert::That(t2s(doc.nextLoop(s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:00:00"));
	//	Assert::That(t2s(doc.nextLoop(s2t("01:00:00:00", tcType))->timeIn(), tcType), Equals("01:01:00:00"));
	//	Assert::That(doc.nextLoop(s2t("01:01:00:00", tcType)) == NULL);
	//}

	//void StripDocTest::getPreviousLoopTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("test01.detx"));
	//	PhTimeCodeType tcType = PhTimeCodeType25;

	//	Assert::That(doc.previousLoop(s2t("01:00:00:00", tcType)) == NULL);
	//	Assert::That(t2s(doc.previousLoop(s2t("01:01:00:00", tcType))->timeIn(), tcType), Equals("01:00:00:00"));
	//	Assert::That(t2s(doc.previousLoop(s2t("23:00:00:00", tcType))->timeIn(), tcType), Equals("01:01:00:00"));
	//}

	//void StripDocTest::addObjectTest()
	//{
	//	PhStripDoc doc;
	//	doc.addPeople(new PhPeople("A people"));

	//	doc.addObject(new PhStripText(0, doc.peoples().last(), 10000, 1, "Hello", 0.25f));
	//	Assert::That(doc.texts().count() == 1);
	//	doc.addObject(new PhStripCut(5400, PhStripCut::CrossFade));
	//	Assert::That(doc.cuts().count() == 1);
	//	doc.addObject(new PhStripDetect(PhStripDetect::Aperture, 10000, doc.peoples().last(), 11000, 1));
	//	Assert::That(doc.detects().count() == 1);

	//	doc.addObject(new PhStripLoop(22000, "3"));
	//	Assert::That(doc.loops().count() == 1);

	//}

	//void StripDocTest::addPeopleTest()
	//{
	//	PhStripDoc doc;
	//	doc.addPeople(new PhPeople("A people"));
	//	Assert::That(doc.peoples().count() == 1);
	//	doc.addPeople(new PhPeople("A second people"));
	//	Assert::That(doc.peoples().count() == 2);

	//}

	//#warning /// @todo Move to PhTest
	//Equals StripDocTest::t2s(PhTime time, PhTimeCodeType tcType)
	//{
	//	return PhTimeCode::stringFromTime(time, tcType);
	//}

	//PhTime StripDocTest::s2t(Equals string, PhTimeCodeType tcType)
	//{
	//	return PhTimeCode::timeFromString(string, tcType);
	//}

	//void StripDocTest::importDetXNoTitleTest()
	//{
	//	PhStripDoc doc;

	//	Assert::That(doc.importDetXFile("notitle.detx"));
	//	Assert::That(doc.title() == "notitle");

	//}
};

