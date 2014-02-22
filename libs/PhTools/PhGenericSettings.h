#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include <QSettings>
#include <QStringList>

#define PH_SETTING_INT(setter, getter) \
	void set ## setter(int getter) { setIntValue(#getter, getter); } \
	int getter() {return intValue(#getter); }

#define PH_SETTING_INT2(setter, getter, defaultValue) \
	void set ## setter(int s) { setIntValue(#getter, s); } \
	int getter() {return intValue(#getter, defaultValue); }

#define PH_SETTING_BOOL(setter, getter) \
	void set ## setter(bool getter) { setBoolValue(#getter, getter); } \
	bool getter() {return boolValue(#getter); }

#define PH_SETTING_BOOL2(setter, getter, defaultValue) \
	void set ## setter(bool s) { setBoolValue(#getter, s); } \
	bool getter() {return boolValue(#getter, defaultValue); }

#define PH_SETTING_FLOAT(setter, getter) \
	void set ## setter(float getter) { setFloatValue(#getter, getter); } \
	float getter() {return floatValue(#getter); }

#define PH_SETTING_FLOAT2(setter, getter, defaultValue) \
	void set ## setter(float s) { setFloatValue(#getter, s); } \
	float getter() {return floatValue(#getter, defaultValue); }

#define PH_SETTING_STRING(setter, getter) \
	void set ## setter(QString getter) { setStringValue(#getter, getter); } \
	QString getter() {return stringValue(#getter); }

#define PH_SETTING_STRING2(setter, getter, defaultValue) \
	void set ## setter(QString s) { setStringValue(#getter, s); } \
	QString getter() {return stringValue(#getter, defaultValue); }

#define PH_SETTING_STRINGLIST(setter, getter) \
	void set ## setter(QStringList list) { setStringList(#getter, list); } \
	QStringList getter() {return stringList(#getter); }

class PhGenericSettings
{
public:
	PhGenericSettings();

protected:
	void setIntValue(QString name, int value);
	int intValue(QString name, int defaultValue = 0);

	void setBoolValue(QString name, bool value);
	bool boolValue(QString name, bool defaultValue = false);

	void setFloatValue(QString name, float value);
	float floatValue(QString name, float defaultValue = 0);

	void setStringValue(QString name, QString value);
	QString stringValue(QString name, QString defaultValue = "");

	void setStringList(QString name, QStringList list);
	QStringList stringList(QString name);

//private:
	QSettings _settings;
};

#endif // PHGENERICSETTINGS_H
