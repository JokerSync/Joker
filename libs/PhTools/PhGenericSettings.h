#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include <QSettings>

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

private:
	QSettings _settings;
};

#endif // PHGENERICSETTINGS_H
