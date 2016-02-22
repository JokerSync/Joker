#ifndef SETTINGSSPECSETTINGS_H
#define SETTINGSSPECSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

/**
 * @brief Settings for unit tests
 */
class SettingsSpecSettings : public PhGenericSettings
{
public:
	PH_SETTING_INT(setIntTest1, intTest1)
	PH_SETTING_INT(setIntTest2, intTest2)
	PH_SETTING_INT2(setIntTest3, intTest3, 4)
	PH_SETTING_INT3(setIntTest4, intTest4, intTest1)

	PH_SETTING_UCHAR(setUnsignedCharTest1, unsignedCharTest1)
	PH_SETTING_UCHAR(setUnsignedCharTest2, unsignedCharTest2)
	PH_SETTING_UCHAR2(setUnsignedCharTest3, unsignedCharTest3, 0xF0)

	PH_SETTING_BOOL(setBoolTest1, boolTest1)
	PH_SETTING_BOOL(setBoolTest2, boolTest2)
	PH_SETTING_BOOL2(setBoolTest3, boolTest3, true)

	PH_SETTING_FLOAT(setFloatTest1, floatTest1)
	PH_SETTING_FLOAT(setFloatTest2, floatTest2)
	PH_SETTING_FLOAT2(setFloatTest3, floatTest3, 3.14f)

	PH_SETTING_STRING(setStringTest1, stringTest1)
	PH_SETTING_STRING(setStringTest2, stringTest2)
	PH_SETTING_STRING2(setStringTest3, stringTest3, "stringTest default value")

	PH_SETTING_STRING(setStringTest4, stringTest4)
	PH_SETTING_STRINGLIST(setStringListTest1, stringListTest1)
	PH_SETTING_STRINGLIST(setStringListTest2, stringListTest2)
	PH_SETTING_STRINGLIST2(setStringListTest3, stringListTest3, QStringList({"a", "b", "c"}))

	PH_SETTING_BYTEARRAY(setByteArrayTest1, byteArrayTest1)
	PH_SETTING_BYTEARRAY(setByteArrayTest2, byteArrayTest2)

	PH_SETTING_HASH(setHashTest1, hashTest1)
};

#endif // SETTINGSSPECSETTINGS_H
