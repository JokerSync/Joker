#include "PhGenericSettings.h"

PhGenericSettings::PhGenericSettings() : _settings(ORG_NAME, APP_NAME)
{
	QSettings::setDefaultFormat(QSettings::NativeFormat);
}

void PhGenericSettings::setIntValue(QString name, int value)
{
	_settings.setValue(name, value);
}

int PhGenericSettings::intValue(QString name, int defaultValue)
{
	return _settings.value(name, defaultValue).toInt();
}

void PhGenericSettings::setBoolValue(QString name, bool value)
{
	_settings.setValue(name, value);
}

bool PhGenericSettings::boolValue(QString name, bool defaultValue)
{
	return _settings.value(name, defaultValue).toBool();
}

void PhGenericSettings::setFloatValue(QString name, float value)
{
	_settings.setValue(name, value);
}

float PhGenericSettings::floatValue(QString name, float defaultValue)
{
	return _settings.value(name, defaultValue).toFloat();
}

void PhGenericSettings::setStringValue(QString name, QString value)
{
	_settings.setValue(name, value);
}

QString PhGenericSettings::stringValue(QString name, QString defaultValue)
{
	return _settings.value(name, defaultValue).toString();
}
