/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhTestTools.h"
#include "PhSync/PhTimeCode.h"

#include "StripDocTest.h"

void StripDocTest::initTestCase()
{
	PhDebug::enable();
}

void StripDocTest::importDetXHeaderTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.generator(), QString("Cappella v0.12.5, 1"));
	QCOMPARE(doc.title(), QString("Title test"));
	QCOMPARE(doc.translatedTitle(), QString("Translated title"));
	QCOMPARE(doc.episode(), QString("episode ref"));
	QCOMPARE(doc.season(), QString("season ref"));
	QCOMPARE(doc.videoFilePath(), QString("/Path/to/video.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("01:00:00:00"));
	QCOMPARE(doc.authorName(), QString("Pierre Dupont"));
	QCOMPARE(t2s(doc.lastTime(), PhTimeCodeType25), QString("01:00:16:00"));

	QList<QString> metaKey = doc.metaKeys();

	QCOMPARE(metaKey.count(), 6);
	QVERIFY(metaKey.contains("Producteur"));
	QVERIFY(metaKey.contains("Année de production"));
	QVERIFY(metaKey.contains("Distributeur"));
	QVERIFY(metaKey.contains("Réalisateur"));
	QVERIFY(metaKey.contains("Diffuseur"));
	QVERIFY(metaKey.contains("Pays d'origine"));

	QCOMPARE(doc.metaInformation("Producteur"), QString("Producer"));
	QCOMPARE(doc.metaInformation("Année de production"), QString("2014"));
	QCOMPARE(doc.metaInformation("Distributeur"), QString("Distributor"));
	QCOMPARE(doc.metaInformation("Réalisateur"), QString("Director"));
	QCOMPARE(doc.metaInformation("Diffuseur"), QString("Publisher"));
	QCOMPARE(doc.metaInformation("Pays d'origine"), QString("Country"));
	PHDEBUG << doc.title();
}

void StripDocTest::importDetXNoFile()
{
	PhStripDoc doc;

	QVERIFY(!doc.importDetXFile("does_not_exist.detx"));
}

void StripDocTest::importDetXPeopleTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.peoples().count(), 3);
	PhPeople *jeanne = doc.peopleByName("Jeanne");
	QVERIFY(jeanne);
	QCOMPARE(jeanne->name(), QString("Jeanne"));
	QCOMPARE(jeanne->color(), QString("#00BB00"));

	PhPeople *sue = doc.peopleByName("Sue");
	QVERIFY(sue);
	QCOMPARE(sue->name(), QString("Sue"));
	QCOMPARE(sue->color(), QString("#BB0000"));

	PhPeople *paul = doc.peopleByName("Paul");
	QVERIFY(paul);
	QCOMPARE(paul->name(), QString("Paul"));
	QCOMPARE(paul->color(), QString("#0000BB"));
	QVERIFY(doc.peopleByName("Johnny") == NULL);
}

void StripDocTest::importDetXLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.loops().count() == 2);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType25), QString("01:01:00:00"));

}

void StripDocTest::importDetXCutTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.cuts().count() == 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:01:00"));
}

void StripDocTest::importDetXTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.texts().count() == 6);

	QCOMPARE(doc.texts()[0]->content(), QString("Simple sentence"));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("01:00:04:00"));
	QCOMPARE(doc.texts()[0]->people(), doc.peopleByName("Jeanne"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.25f), PHNQ(QString::number(doc.texts()[0]->y())));


	QCOMPARE(doc.texts()[1]->content(), QString("Composed "));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), QString("01:00:06:00"));

	QCOMPARE(doc.texts()[2]->content(), QString("sentence"));
	QCOMPARE(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), QString("01:00:07:00"));

	QCOMPARE(doc.texts()[3]->content(), QString("Simple off sentence"));
	QCOMPARE(t2s(doc.texts()[3]->timeIn(), PhTimeCodeType25), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.texts()[3]->timeOut(), PhTimeCodeType25), QString("01:00:14:00"));

	QCOMPARE(doc.texts()[4]->content(), QString("Composed sentence with off"));
	QCOMPARE(t2s(doc.texts()[4]->timeIn(), PhTimeCodeType25), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.texts()[4]->timeOut(), PhTimeCodeType25), QString("01:00:17:00"));

	QCOMPARE(doc.texts()[5]->content(), QString("Sentence with out not linked"));
	QCOMPARE(t2s(doc.texts()[5]->timeIn(), PhTimeCodeType25), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.texts()[5]->timeOut(), PhTimeCodeType25), QString("01:00:31:04"));
}

void StripDocTest::importDetXDetectTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.detects().count(), 6);

	QCOMPARE(doc.detects()[0]->people(), doc.peopleByName("Jeanne"));
	QCOMPARE(doc.detects()[0]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), QString("01:00:04:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[0]->y(), 0.25f), PHNQ(QString::number(doc.detects()[0]->y())));

	QCOMPARE(doc.detects()[1]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[1]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[1]->timeIn(), PhTimeCodeType25), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.detects()[1]->timeOut(), PhTimeCodeType25), QString("01:00:07:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[1]->y(), 0.5f), PHNQ(QString::number(doc.detects()[1]->y())));


	QCOMPARE(doc.detects()[2]->people(), doc.peopleByName("Paul"));
	QCOMPARE(doc.detects()[2]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[2]->timeIn(), PhTimeCodeType25), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.detects()[2]->timeOut(), PhTimeCodeType25), QString("01:00:14:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[2]->y(), 0.25f), PHNQ(QString::number(doc.detects()[2]->y())));


	QCOMPARE(doc.detects()[3]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[3]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[3]->timeIn(), PhTimeCodeType25), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.detects()[3]->timeOut(), PhTimeCodeType25), QString("01:00:17:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[3]->y(), 0.5f), PHNQ(QString::number(doc.detects()[3]->y())));

	QCOMPARE(doc.detects()[4]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[4]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[4]->timeIn(), PhTimeCodeType25), QString("01:00:20:00"));
	QCOMPARE(t2s(doc.detects()[4]->timeOut(), PhTimeCodeType25), QString("01:00:22:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[4]->y(), 0.5f), PHNQ(QString::number(doc.detects()[4]->y())));


	QCOMPARE(doc.detects()[5]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[5]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[5]->timeIn(), PhTimeCodeType25), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.detects()[5]->timeOut(), PhTimeCodeType25), QString("01:00:31:04"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[5]->y(), 0.5f), PHNQ(QString::number(doc.detects()[5]->y())));

}

void StripDocTest::importMosTest01()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test01.mos"));

	QCOMPARE(doc.authorName(), QString("auteur"));

	QCOMPARE(doc.videoFilePath(), QString(""));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:00:00:00"));

	QCOMPARE(doc.title(), QString("Titre VO"));
	QCOMPARE(doc.season(), QString("saison"));
	QCOMPARE(doc.episode(), QString("episode"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 1);
	PhPeople * people = doc.peopleByName("Nom personnage");
	QVERIFY(people != NULL);
	QCOMPARE(people->name(), QString("Nom personnage"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 0);

	// Test loop
	QCOMPARE(doc.loops().count(), 0);

	// Test texts
	QCOMPARE(doc.texts().count(), 3);
	QCOMPARE(doc.texts()[0]->people(), people);
	QCOMPARE(doc.texts()[0]->content(), QString("Ceci "));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("00:00:00:20"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("00:00:01:12"));

	QCOMPARE(doc.texts()[1]->content(), QString("est un"));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), QString("00:00:01:12"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), QString("00:00:01:16"));

	QCOMPARE(doc.texts()[2]->content(), QString(" test."));
	QCOMPARE(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), QString("00:00:01:16"));
	QCOMPARE(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), QString("00:00:02:03"));

	// Detect test
	//#warning TODO more test on detect
	QCOMPARE(doc.detects().count(), 4);
}

void StripDocTest::importMosTest02()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test02.mos"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:58:24:00"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 2);

	PhPeople * pierre = doc.peopleByName("Pierre");
	QVERIFY(pierre != NULL);
	QCOMPARE(pierre->name(), QString("Pierre"));

	PhPeople * marie = doc.peopleByName("Marie");
	QVERIFY(marie != NULL);
	QCOMPARE(marie->name(), QString("Marie"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));

	// Test loops
	QCOMPARE(doc.loops().count(), 1);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));
	QCOMPARE(doc.loops()[0]->label(), QString("1"));

	// Test texts
	QCOMPARE(doc.texts().count(), 2);
	QCOMPARE(doc.texts()[0]->people(), pierre);
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.0), PHNQ(QString::number(doc.texts()[0]->y())));
	QCOMPARE(doc.texts()[0]->content(), QString("Bonjour, Marie."));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("01:00:02:00"));

	QCOMPARE(doc.texts()[1]->people(), marie);
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->y(), 0.6), PHNQ(QString::number(doc.texts()[1]->y())));
	QCOMPARE(doc.texts()[1]->content(), QString("Bonjour, Pierre."));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), QString("01:00:04:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), QString("01:00:06:00"));

	// Detect test
	QCOMPARE(doc.detects().count(), 0);
}

void StripDocTest::importMosTest03()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test03.mos"));

	QCOMPARE(doc.videoFilePath(), QString("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType24);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType24), QString("05:59:50:00"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 2);

	PhPeople * noName = doc.peopleByName("");
	QVERIFY(noName != NULL);
	QCOMPARE(noName->name(), QString(""));

	PhPeople * denis = doc.peopleByName("Denis");
	QVERIFY(denis != NULL);
	QCOMPARE(denis->name(), QString("Denis"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 29);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType24), QString("05:59:50:00"));
	QCOMPARE(t2s(doc.cuts()[1]->timeIn(), PhTimeCodeType24), QString("05:59:58:00"));
	QCOMPARE(t2s(doc.cuts()[2]->timeIn(), PhTimeCodeType24), QString("06:00:00:00"));
	QCOMPARE(t2s(doc.cuts()[3]->timeIn(), PhTimeCodeType24), QString("06:00:05:22"));
	QCOMPARE(t2s(doc.cuts()[4]->timeIn(), PhTimeCodeType24), QString("06:00:08:05"));

	// Test loops
	QCOMPARE(doc.loops().count(), 2);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType24), QString("06:00:01:00"));
	QCOMPARE(doc.loops()[0]->label(), QString("30"));
	QCOMPARE(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType24), QString("06:01:15:00"));
	QCOMPARE(doc.loops()[1]->label(), QString("off"));

	// Test texts
	QCOMPARE(doc.texts(false).count(), 206);
	QCOMPARE(doc.texts(true).count(), 0);

	QCOMPARE(doc.texts()[0]->people(), noName);
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), QString("05:59:49:05"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), QString("05:59:49:20"));

	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 2.0f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.2f), PHNQ(QString::number(doc.texts()[0]->height())));

	QCOMPARE(doc.texts()[0]->content(), QString("Départ bob 6"));
	QCOMPARE(doc.texts()[1]->content(), QString("(X)"));
	QCOMPARE(doc.texts()[2]->content(), QString("05.59.50.00"));
	QCOMPARE(doc.texts()[3]->content(), QString("1000"));
	QCOMPARE(doc.texts()[4]->content(), QString("(X)"));
	QCOMPARE(doc.texts()[5]->content(), QString("P.I"));

	QCOMPARE(doc.texts()[9]->people(), denis);
	QCOMPARE(t2s(doc.texts()[9]->timeIn(), PhTimeCodeType24), QString("06:00:07:23"));
	QCOMPARE(t2s(doc.texts()[9]->timeOut(), PhTimeCodeType24), QString("06:00:08:03"));
	QCOMPARE(t2s(doc.texts()[10]->timeIn(), PhTimeCodeType24), QString("06:00:08:03"));
	QCOMPARE(t2s(doc.texts()[10]->timeOut(), PhTimeCodeType24), QString("06:00:08:07"));

	QCOMPARE(doc.texts()[9]->content(), QString("Tu "));
	QCOMPARE(doc.texts()[10]->content(), QString("tra"));
	QCOMPARE(doc.texts()[11]->content(), QString("vaillais "));
	QCOMPARE(doc.texts()[12]->content(), QString("pas "));
	QCOMPARE(doc.texts()[13]->content(), QString("ce "));
	QCOMPARE(doc.texts()[14]->content(), QString("soir !"));

	QCOMPARE(t2s(doc.texts()[14]->timeOut(), PhTimeCodeType24), QString("06:00:09:06"));

	// Detect test
	QCOMPARE(doc.detects().count(), 24);
	//#warning TODO more test on detect
}

void StripDocTest::importMosTest04()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test04.mos"));

	// Test video info
	QCOMPARE(doc.videoFilePath(), QString("D:\\Ressources\\Mosaic\\Utilisateurs\\Yves\\Bold 5704\\5704.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:59:39:24"));

	// Test loops
	QCOMPARE(doc.loops().count(), 27);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:13"));
	QCOMPARE(doc.loops()[0]->label(), QString("1"));
	QCOMPARE(doc.loops()[1]->label(), QString("2"));
	QCOMPARE(doc.loops()[2]->label(), QString("3"));
	QCOMPARE(doc.loops()[3]->label(), QString("off"));
	QCOMPARE(doc.loops()[4]->label(), QString("4"));
	QCOMPARE(t2s(doc.loops()[5]->timeIn(), PhTimeCodeType25), QString("01:02:56:02"));
	QCOMPARE(doc.loops()[5]->label(), QString("5"));
	QCOMPARE(t2s(doc.loops()[25]->timeIn(), PhTimeCodeType25), QString("01:18:14:11"));
	QCOMPARE(doc.loops()[25]->label(), QString("25"));
	QCOMPARE(t2s(doc.loops()[26]->timeIn(), PhTimeCodeType25), QString("01:19:01:01"));
	QCOMPARE(doc.loops()[26]->label(), QString("off"));

	// Test texts
	QCOMPARE(doc.texts().count(), 1118);
	QCOMPARE(doc.texts(true).count(), 4329);
	QCOMPARE(doc.detects().count(), 4552);

	QCOMPARE(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), QString("01:00:01:13"));
	QCOMPARE(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), QString("01:00:02:20"));
	QCOMPARE(doc.detects()[0]->type(), PhStripDetect::Off);
}

void StripDocTest::importMosTest24fps()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("mos24.mos"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Gilles\\Desktop\\Sonic_EP_01_mix_VA.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType24);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType24), QString("00:59:58:00"));

	QCOMPARE(doc.texts().count(), 1);

	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), QString("01:00:01:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), QString("01:00:03:00"));
}

void StripDocTest::importMosTest25fps()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("mos25.mos"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Gilles\\Desktop\\Get Blake\\Get Blake 115\\GBL_EP115_Online_Master_VA_h264_TCI.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("09:59:58:00"));
}

void StripDocTest::importDrbTest01()
{
	PhStripDoc doc;

	QVERIFY(doc.openStripFile("drb01.drb"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\SYNCHRO USER\\Downloads\\TheManWithTheGoldenArm_25fps_high\\TheManWithTheGoldenArm_25fps_high.mov"));

	QCOMPARE(doc.loops().count(), 0);

	QCOMPARE(doc.peoples().count(), 2);
	QCOMPARE(doc.peoples()[0]->name(), QString("Personnage 1"));
	QCOMPARE(doc.peoples()[1]->name(), QString("Personnage 2"));

	QCOMPARE(doc.texts().count(), 1);
	QCOMPARE(doc.texts()[0]->people()->name(), QString("Personnage 2"));

	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.36f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[0]->height())));

#warning /// @todo should be the same than syn6
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("00:02:12:10"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("00:02:44:07"));
}

void StripDocTest::importDrbTest02()
{
	PhStripDoc doc;

	QVERIFY(doc.openStripFile("drb02.drb"));

	QCOMPARE(doc.videoFilePath(), QString("D:\\NED 201.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:58:04:20"));

	QCOMPARE(doc.loops().count(), 21);

	for(int i = 0; i < 21; i++)
		QCOMPARE(doc.loops().at(i)->label(), QString::number(i + 1));

	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:39:02"));
	QCOMPARE(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType25), QString("01:02:14:23"));

	QCOMPARE(doc.peoples().count(), 28);

	QCOMPARE(doc.peoples()[0]->name(), QString("Intervenant 1"));
	QCOMPARE(doc.peoples()[1]->name(), QString("ned"));
	QCOMPARE(doc.peoples()[2]->name(), QString("moze"));
	QCOMPARE(doc.peoples()[3]->name(), QString("suzie"));

	QCOMPARE(doc.texts().count(), 546);
	QCOMPARE(doc.texts()[0]->people()->name(), QString("ned"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.213333338f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.28666667f), PHNQ(QString::number(doc.texts()[0]->height())));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:13"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("01:00:01:05"));
}

void StripDocTest::importDrbTest03()
{
	PhStripDoc doc;

	QVERIFY(doc.openStripFile("drb03.drb"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:58:24:00"));

	QCOMPARE(doc.loops().count(), 1);

	QCOMPARE(doc.loops().at(0)->label(), QString("1"));

	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));

	QCOMPARE(doc.peoples().count(), 2);

	QCOMPARE(doc.peoples()[0]->name(), QString("Pierre"));
	QCOMPARE(doc.peoples()[1]->name(), QString("Marie"));

	QCOMPARE(doc.texts().count(), 2);

	QCOMPARE(doc.texts()[0]->people()->name(), QString("Pierre"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.066666667f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[0]->height())));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:01:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("01:00:03:00"));

	QCOMPARE(doc.texts()[1]->people()->name(), QString("Marie"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->y(), 0.7f), PHNQ(QString::number(doc.texts()[1]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[1]->height())));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), QString("01:00:04:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), QString("01:00:06:00"));

	QCOMPARE(doc.cuts().count(), 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:05:00"));
}

void StripDocTest::importSyn6Test01()
{
	PhStripDoc doc;

	QVERIFY(doc.openStripFile("test01.syn6"));

	QCOMPARE(doc.peoples().count(), 2);
	QCOMPARE(doc.peoples()[0]->name(), QString("Personnage 1"));
	QCOMPARE(doc.peoples()[1]->name(), QString("Personnage 2"));

	QCOMPARE(doc.texts().count(), 1);
	QCOMPARE(doc.texts()[0]->people()->name(), QString("Personnage 1"));

	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.36f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[0]->height())));

	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("00:00:03:11"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("00:00:06:14"));
}

void StripDocTest::importSyn6Test02()
{
	PhStripDoc doc;

	QVERIFY(doc.openStripFile("test02.syn6"));

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:58:24:00"));

	QCOMPARE(doc.loops().count(), 1);

	QCOMPARE(doc.loops().at(0)->label(), QString("1"));

	QCOMPARE(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), QString("01:00:00:00"));

	QCOMPARE(doc.peoples().count(), 2);

	QCOMPARE(doc.peoples()[0]->name(), QString("Pierre"));
	QCOMPARE(doc.peoples()[1]->name(), QString("Marie"));

	QCOMPARE(doc.texts().count(), 2);

	QCOMPARE(doc.texts()[0]->people()->name(), QString("Pierre"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.066666667f), PHNQ(QString::number(doc.texts()[0]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[0]->height())));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:01:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), QString("01:00:03:00"));

	QCOMPARE(doc.texts()[1]->people()->name(), QString("Marie"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->y(), 0.7f), PHNQ(QString::number(doc.texts()[1]->y())));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->height(), 0.22666667f), PHNQ(QString::number(doc.texts()[1]->height())));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), QString("01:00:04:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), QString("01:00:06:00"));

	QCOMPARE(doc.cuts().count(), 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), QString("01:00:05:00"));
}

void StripDocTest::openStripFileTest()
{
	PhStripDoc doc;

	// DetX test
	QVERIFY(doc.openStripFile("test01.detx"));
	QCOMPARE(t2s(doc.lastTime(), PhTimeCodeType25), QString("01:00:16:00"));
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("01:00:00:00"));

	// Mos test
	QVERIFY(doc.openStripFile("test03.mos"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType24);

	// Strip file
	QVERIFY(doc.openStripFile("test.strip"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("00:59:00:00"));
	QCOMPARE(t2s(doc.lastTime(), PhTimeCodeType25), QString("01:02:03:04"));

	// Test regular joker file linked to detx
	QVERIFY(doc.openStripFile("test01.joker"));
	QCOMPARE(doc.filePath(), QString("test01.detx"));
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("01:01:00:00"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(t2s(doc.lastTime(), PhTimeCodeType25), QString("01:30:00:00"));

	QCOMPARE(doc.title(), QString("Title test"));
	QCOMPARE(doc.generator(), QString("Cappella v0.12.5, 1"));

	// Test regular joker file linked to mos
	QVERIFY(doc.openStripFile("test02.joker"));
	QCOMPARE(doc.filePath(), QString("test03.mos"));
	QCOMPARE(doc.videoFilePath(), QString("test02.mov"));
	QCOMPARE(doc.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc.videoTimeIn(), PhTimeCodeType25), QString("01:03:00:00"));
	QCOMPARE(doc.forceRatio169(), false);
	QCOMPARE(t2s(doc.lastTime(), PhTimeCodeType25), QString("01:20:00:00"));

	QCOMPARE(doc.title(), QString("Au fil d'Ariane"));
	QCOMPARE(doc.generator(), QString("Mosaic"));

	// Test bad files
	QVERIFY(QFile::exists("bad_tag.joker"));
	QVERIFY(!doc.openStripFile("bad_tag.joker"));

	QVERIFY(QFile::exists("empty.joker"));
	QVERIFY(!doc.openStripFile("empty.joker"));

	// Test accepted files
	QVERIFY(QFile::exists("empty_root.joker"));
	QVERIFY(doc.openStripFile("empty_root.joker"));

	QVERIFY(QFile::exists("empty_meta.joker"));
	QVERIFY(doc.openStripFile("empty_meta.joker"));

#warning /// @todo Test video frame rate
}

void StripDocTest::openSaveTest01()
{
	PhStripDoc doc1;

	QVERIFY(doc1.openStripFile("test01.joker"));

	QVERIFY(doc1.saveStripFile("save01.joker", s2t("01:01:01:01", PhTimeCodeType25)));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save01.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(doc2.videoTimeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(doc2.videoTimeIn(), PhTimeCodeType25), t2s(doc1.videoTimeIn(), PhTimeCodeType25));
	QCOMPARE(doc2.forceRatio169(), true);
	QCOMPARE(doc2.videoDeinterlace(), false);
	QCOMPARE(t2s(doc2.lastTime(), PhTimeCodeType25), QString("01:01:01:01"));

	QCOMPARE(doc2.title(), doc1.title());
	QCOMPARE(doc2.generator(), doc1.generator());
}

void StripDocTest::openSaveTest02()
{
	PhStripDoc doc1;

	QVERIFY(doc1.openStripFile("test02.joker"));

	QVERIFY(doc1.saveStripFile("save02.joker", s2t("02:02:02:02", PhTimeCodeType25)));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save02.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(t2s(doc2.videoTimeIn(), PhTimeCodeType25), t2s(doc1.videoTimeIn(), PhTimeCodeType25));
	QCOMPARE(doc2.forceRatio169(), false);
	QCOMPARE(doc2.videoDeinterlace(), true);
	QCOMPARE(t2s(doc2.lastTime(), PhTimeCodeType25), QString("02:02:02:02"));

	QCOMPARE(doc2.title(), doc1.title());
	QCOMPARE(doc2.generator(), doc1.generator());
}

void StripDocTest::getPeopleByNameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.peopleByName("Jeanne")->name(), QString("Jeanne"));
	QCOMPARE(doc.peopleByName("Sue")->name(), QString("Sue"));
	QCOMPARE(doc.peopleByName("Paul")->name(), QString("Paul"));
	QVERIFY(doc.peopleByName("Bob") == NULL);
}

void StripDocTest::getPreviousElementTimeTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QCOMPARE(t2s(doc.previousElementTime(s2t("23:00:00:00", tcType)), tcType), QString("01:01:00:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:01:00:00", tcType)), tcType), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:00:30:00", tcType)), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:00:15:00", tcType)), tcType), QString("01:00:12:00"));
	QCOMPARE(doc.previousElementTime(s2t("01:00:00:00", tcType)), PHTIMEMIN);
}

void StripDocTest::getNextElementTimeTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QCOMPARE(t2s(doc.nextElementTime(s2t("00:00:00:00", tcType)), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:00:00", tcType)), tcType), QString("01:00:01:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:01:00", tcType)), tcType), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:02:00", tcType)), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:17:00", tcType)), tcType), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:30:00", tcType)), tcType), QString("01:01:00:00"));
	QCOMPARE(doc.nextElementTime(s2t("01:01:00:00", tcType)), PHTIMEMAX);

}

void StripDocTest::getNextTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QCOMPARE(t2s(doc.nextText(s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:02:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:12:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextTextTestByPeople()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	PhPeople* sue = doc.peopleByName("Sue");

	QCOMPARE(t2s(doc.nextText(sue, s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(sue, s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextTextTestByPeopleList()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QList<PhPeople*> peopleList;
	peopleList.append(doc.peopleByName("Sue"));
	peopleList.append(doc.peopleByName("Paul"));

	QCOMPARE(t2s(doc.nextText(peopleList, s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:12:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(peopleList, s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QCOMPARE(t2s(doc.nextLoop(s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.nextLoop(s2t("01:00:00:00", tcType))->timeIn(), tcType), QString("01:01:00:00"));
	QVERIFY(doc.nextLoop(s2t("01:01:00:00", tcType)) == NULL);
}

void StripDocTest::getPreviousLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = PhTimeCodeType25;

	QVERIFY(doc.previousLoop(s2t("01:00:00:00", tcType)) == NULL);
	QCOMPARE(t2s(doc.previousLoop(s2t("01:01:00:00", tcType))->timeIn(), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.previousLoop(s2t("23:00:00:00", tcType))->timeIn(), tcType), QString("01:01:00:00"));
}

void StripDocTest::addObjectTest()
{
	PhStripDoc doc;
	doc.addPeople(new PhPeople("A people"));

	doc.addObject(new PhStripText(0, doc.peoples().last(), 10000, 1, "Hello", 0.25f));
	QVERIFY(doc.texts().count() == 1);
	doc.addObject(new PhStripCut(5400, PhStripCut::CrossFade));
	QVERIFY(doc.cuts().count() == 1);
	doc.addObject(new PhStripDetect(PhStripDetect::Aperture, 10000, doc.peoples().last(), 11000, 1));
	QVERIFY(doc.detects().count() == 1);

	doc.addObject(new PhStripLoop(22000, "3"));
	QVERIFY(doc.loops().count() == 1);

}

void StripDocTest::addPeopleTest()
{
	PhStripDoc doc;
	doc.addPeople(new PhPeople("A people"));
	QVERIFY(doc.peoples().count() == 1);
	doc.addPeople(new PhPeople("A second people"));
	QVERIFY(doc.peoples().count() == 2);

}

#warning /// @todo Move to PhTest
QString StripDocTest::t2s(PhTime time, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(time, tcType);
}

PhTime StripDocTest::s2t(QString string, PhTimeCodeType tcType)
{
	return PhTimeCode::timeFromString(string, tcType);
}

void StripDocTest::importDetXNoTitleTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("notitle.detx"));
	QVERIFY(doc.title() == "notitle");

}
