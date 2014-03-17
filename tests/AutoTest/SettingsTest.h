#ifndef PHSETTINGSTEST_H
#define PHSETTINGSTEST_H

#include <QObject>
#include <QTest>

#include "PhTools/PhGenericSettings.h"

class SettingsTest : public QObject
{
	Q_OBJECT

private slots:
	void testClear();
	void testIntSettings();
	void testUnsignedCharSettings();
	void testBoolSettings();
	void testFloatSettings();
	void testStringSettings();
	void testStringListSettings();
	void testByteArraySettings();
};

#endif // PHSETTINGSTEST_H
