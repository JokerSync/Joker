#include <stdio.h>

#include <QApplication>

#include <PhTools/PhDebug.h>

#include "ClockTest.h"
#include "SettingsTest.h"
#include "TimeCodeTest.h"
#include "DebugTest.h"
#include "StripDocTest.h"
#include "LockableSpinBoxTest.h"
#include "TimeCodeEditTest.h"
#include "WindowTest.h"
#include "SonyControllerTest.h"
#include "GraphicTest.h"
#include "GraphicStripTest.h"
#include "GraphicTextTest.h"
#include "VideoTest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PHDEBUG << "AutoTest";

	bool testAll = (argc < 2); // if no argument, test all
	bool testClock = testAll;
	bool testSettings = testAll;
	bool testTC = testAll;
	bool testDebug = testAll;
	bool testDoc = testAll;
	bool testLockableSpinBox = testAll;
	bool testTCEdit = testAll;
	bool testWindow = testAll;
	bool testSony = testAll;
	bool testGraphic = testAll;
	bool testGraphicText = testAll;
	bool testGraphicStrip = testAll;
	bool testVideo = testAll;

	int result = 0;

	QStringList testArgList;

	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "all") == 0) {
			testClock = testSettings = testTC = testDebug = testDoc = testLockableSpinBox = testTCEdit =
			                                                                                    testWindow = testSony = testGraphic = testGraphicText = testGraphicStrip = true;
		}
		else if(strcmp(argv[i], "clock") == 0)
			testClock = true;
		else if(strcmp(argv[i], "settings") == 0)
			testSettings = true;
		else if(strcmp(argv[i], "tc") == 0)
			testTC = true;
		else if(strcmp(argv[i], "debug") == 0)
			testDebug = true;
		else if(strcmp(argv[i], "doc") == 0)
			testDoc = true;
		else if(strcmp(argv[i], "lockspin") == 0)
			testLockableSpinBox = true;
		else if(strcmp(argv[i], "tcedit") == 0)
			testTCEdit = true;
		else if(strcmp(argv[i], "window") == 0)
			testWindow = true;
		else if(strcmp(argv[i], "ui") == 0) {
			testTCEdit = testLockableSpinBox = testWindow = true;
		}
		else if(strcmp(argv[i], "sony") == 0)
			testSony = true;
		else if(strcmp(argv[i], "graphic") == 0)
			testGraphic = true;
		else if(strcasecmp(argv[i], "graphictext") == 0)
			testGraphicText = true;
		else if(strcasecmp(argv[i], "graphicstrip") == 0)
			testGraphicStrip = true;
		else if(strcasecmp(argv[i], "video") == 0)
			testVideo = true;
		else
			testArgList.append(argv[i]);
	}

	if(testClock) {
		// Testing PhClock
		ClockTest clockTest;
		result += QTest::qExec(&clockTest, testArgList);
	}

	if(testSettings) {
		// Testing PhSettings
		SettingsTest settingsTest;
		result += QTest::qExec(&settingsTest, testArgList);
	}

	if(testTC) {
		// Testing PhTimeCode
		TimeCodeTest tcTest;
		result += QTest::qExec(&tcTest, testArgList);
	}

	if(testDebug) {
		DebugTest debugTest;
		result += QTest::qExec(&debugTest, testArgList);
	}
	if(testDoc) {
		// Testing PhStripDoc
		StripDocTest docTest;
		result += QTest::qExec(&docTest, testArgList);
	}

	if(testSony) {
		// Testing PhSonyController
		SonyControllerTest sonyTest;
		result += QTest::qExec(&sonyTest, testArgList);
	}

	if(testLockableSpinBox) {
		LockableSpinBoxTest spinBoxTest;
		result += QTest::qExec(&spinBoxTest, testArgList);
	}

	if(testTCEdit) {
		// Testing PhTimeCodeEdit
		TimeCodeEditTest tcEditTest;
		result += QTest::qExec(&tcEditTest, testArgList);
	}

	if(testWindow) {
		WindowTest windowTest;
		result += QTest::qExec(&windowTest, testArgList);
	}

	if(testGraphic) {
		GraphicTest graphicTest;
		result += QTest::qExec(&graphicTest, testArgList);
	}

	if(testGraphicText) {
		GraphicTextTest viewTest;
		result += QTest::qExec(&viewTest, testArgList);
	}
	if(testVideo) {
		VideoTest videoTest;
		result += QTest::qExec(&videoTest);
	}

	if(testGraphicStrip) {
		GraphicStripTest viewTest;
		result += QTest::qExec(&viewTest, testArgList);
	}

	QThread::msleep(500);

	if(qgetenv("TRAVIS") == "true") {

		if(result == 0) {

			PHDEBUG << "                            ************";
			PHDEBUG << "                         *****************";
			PHDEBUG << "                       ********************";
			PHDEBUG << "                        ********************";
			PHDEBUG << "                         ********";
			PHDEBUG << "                          ********                 *********";
			PHDEBUG << "                           ***********              *********";
			PHDEBUG << "              *********      ****       *            *********";
			PHDEBUG << "            *            ***               *          *********";
			PHDEBUG << "          *                       **         *          ********";
			PHDEBUG << "         *                         **         *       ***********";
			PHDEBUG << "        *        *****              **      *********************";
			PHDEBUG << "       *        *******                     *********************";
			PHDEBUG << "       *         *****                         *****************";
			PHDEBUG << "       *                                        *";
			PHDEBUG << "        *     *               *                 *";
			PHDEBUG << "         *     *             *                  *";
			PHDEBUG << "           *    *           *                  *";
			PHDEBUG << "             *    *       *                   *";
			PHDEBUG << "                *   *****                   *";
			PHDEBUG << "       ***               *     ************";
			PHDEBUG << "      *   *            *      *";
			PHDEBUG << "      *   *          *********          ***";
			PHDEBUG << "  ****    **********        *          *   *";
			PHDEBUG << " *               *         *           *   *";
			PHDEBUG << "*              *           *************    ****";
			PHDEBUG << "*      *******                                  *";
			PHDEBUG << "*      *    *                                    *";
			PHDEBUG << "  *****    *              *****************      *";
			PHDEBUG << "          *               *               *      *";
			PHDEBUG << "         *      ****      *                *****";
			PHDEBUG << "         *    *      **   *";
			PHDEBUG << "         *   *         ** *";
			PHDEBUG << "      *** *  *            *";
			PHDEBUG << "     *    *   *            *    *";
			PHDEBUG << "      *    *   *           *   * *";
			PHDEBUG << "       *     *  *          *  *  *";
			PHDEBUG << "         *     * *         *  *  *";
			PHDEBUG << "          *       *        *  *  *";
			PHDEBUG << "            *       *    *****   *";
			PHDEBUG << "               *      * * *****  *";
		}
		else {
			PHDEBUG << "                                     _";
			PHDEBUG << "                                     gs        ,8+";
			PHDEBUG << "                                    d88        888.";
			PHDEBUG << "                                    888        888b";
			PHDEBUG << "                                   8888        8888";
			PHDEBUG << "                                   8888        8888";
			PHDEBUG << "                                   8888        8888.";
			PHDEBUG << "                                  ,8888        8888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88889";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  88888        88888";
			PHDEBUG << "                                  ~8888        8888~";
			PHDEBUG << "                                   8888Y+y.'fsY8888";
			PHDEBUG << "                                   Y8888+lif'f+8888";
			PHDEBUG << "                                   '8886~~~~~~~7888";
			PHDEBUG << "                                   88`          V88";
			PHDEBUG << "                                 ,88       _    d88";
			PHDEBUG << "                                 88   __+  ]]   ~ Yi";
			PHDEBUG << "                               d88`   !8!  ]]]    !8.";
			PHDEBUG << "                   _cooooooooood888i   889  ]]]]    88";
			PHDEBUG << "                i88~~~~` ~Y8~~~Y~808   808   ]]]    88";
			PHDEBUG << "              g88f`       '~`    808   808   ]]]    888";
			PHDEBUG << "             d88                 !8!   !8!    ]]    88!";
			PHDEBUG << "            88!                                     d8";
			PHDEBUG << "           ,88                                     ,8!";
			PHDEBUG << "           888     ,d88888.                       ,d8";
			PHDEBUG << "           888   ,888888888]                     g88";
			PHDEBUG << "           '88.  !888888888]                  ,g88f`";
			PHDEBUG << "            !88.                             ,888";
			PHDEBUG << "             '88nsnnnnnnnnnnnnnn___________nnn88~";
			PHDEBUG << "              '~~~~~~~~~~~~~~~~~88888i  g8~`";
			PHDEBUG << "                                88  88` T8`";
			PHDEBUG << "                                88     i8f";
			PHDEBUG << "                                88    ,88";
			PHDEBUG << "                                88    888";
			PHDEBUG << "                                888b____d88";
			PHDEBUG << "                                88888888888";
			PHDEBUG << "                               ~88~~~~~~Y8~";
			PHDEBUG << "                              ,88`      88";
			PHDEBUG << "                              d8        88";
			PHDEBUG << "                              88        '88";
			PHDEBUG << "                             88          88";
			PHDEBUG << "                             88          !8b";
			PHDEBUG << "                             Y8       /\\  ~88";
			PHDEBUG << "                             !8.      88   ~88s.      o";
			PHDEBUG << "                              8b      88   88!88b.    #o_";
			PHDEBUG << "                              88.     88   '888888b    88o";
			PHDEBUG << "                              8V8     88    88~~V88b   d88s";
			PHDEBUG << "                   ,____d8____8.8!    88    88   !88   d888";
			PHDEBUG << "                _o88f~~~  o8888888888g88    88!   8b.  88088";
			PHDEBUG << "               g88~,_i  ,8888f~~~~~~~V88    8b__gg888888~ 889";
			PHDEBUG << "              ,88  88   88  ,d!   ,.        8888_.!88d88_8889      ,";
			PHDEBUG << "_/\\===+/\\~~T/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\";
			PHDEBUG << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
		}
	}

	return result;
}
