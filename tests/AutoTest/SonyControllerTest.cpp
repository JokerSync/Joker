/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QtMath>

#include "SonyControllerTest.h"

#include "PhSync/PhSonyController.h"

#include "PhTools/PhDebug.h"

void SonyControllerTest::testComputeRate()
{
	QVERIFY(qAbs(PhSonyController::computeRate(0)) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(32) - 0.1) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(64) - 1.0) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(79) - 2.94) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(118) - 48.69) < 0.01);
}
