#ifndef PHSETTINGSTEST_H
#define PHSETTINGSTEST_H

#include <QObject>
#include <QTest>

#include "../PhGenericSettings.h"

class PhSettingsTest : public QObject, protected PhGenericSettings
{
	Q_OBJECT
public:
	explicit PhSettingsTest(QObject *parent = 0);

	PH_SETTING_INT(IntTest1, intTest1)
	PH_SETTING_INT(IntTest2, intTest2)
	PH_SETTING_INT2(IntTest3, intTest3, 4)

	PH_SETTING_BOOL(BoolTest1, boolTest1)
	PH_SETTING_BOOL(BoolTest2, boolTest2)
	PH_SETTING_BOOL2(BoolTest3, boolTest3, true)

	PH_SETTING_FLOAT(FloatTest1, floatTest1)
	PH_SETTING_FLOAT(FloatTest2, floatTest2)
	PH_SETTING_FLOAT2(FloatTest3, floatTest3, 3.14f)

	PH_SETTING_STRING(StringTest1, stringTest1)
	PH_SETTING_STRING(StringTest2, stringTest2)
	PH_SETTING_STRING2(StringTest3, stringTest3, "stringTest default value")

	PH_SETTING_STRING(StringTest4, stringTest4)
	PH_SETTING_STRINGLIST(StringListTest, stringListTest)

private slots:
	void testIntSettings();
	void testBoolSettings();
	void testFloatSettings();
	void testStringSettings();
	void testStringListSettings();
};

#endif // PHSETTINGSTEST_H
