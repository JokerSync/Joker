#include "JokerSettings.h"
#include "PhTools\PhGenericSettings.h"
#include "PhTools\PhDebug.h"

JokerSettings::JokerSettings(bool clear) :
	_settings(ORG_NAME, APP_NAME)
{
	QSettings::setDefaultFormat(QSettings::NativeFormat);
	PHDEBUG << "Settings file:" << _settings.fileName();
	if(clear)
		this->clear();
}

void JokerSettings::clear()
{
	_settings.clear();
}

void JokerSettings::setIntValue(QString name, int value)
{
	if (intValue(name) != value) {
		_settings.setValue(name, value);
		emit changed();
	}
}

int JokerSettings::intValue(QString name, int defaultValue)
{
	return _settings.value(name, defaultValue).toInt();
}

void JokerSettings::setLongLongValue(QString name, qlonglong value)
{
	if (longLongValue(name) != value) {
		_settings.setValue(name, value);
		emit changed();
	}
}

qlonglong JokerSettings::longLongValue(QString name, qlonglong defaultValue)
{
	return _settings.value(name, defaultValue).toLongLong();
}

int JokerSettings::intValueWithAlias(QString name, QString alias)
{
	/// If the regular value is 0, return the alias value
	int result = intValue(name);
	if(result == 0)
		result = intValue(alias);
	return result;
}

void JokerSettings::setBoolValue(QString name, bool value)
{
	if (boolValue(name) != value) {
		_settings.setValue(name, value);
		emit changed();
	}
}

bool JokerSettings::boolValue(QString name, bool defaultValue)
{
	return _settings.value(name, defaultValue).toBool();
}

void JokerSettings::setFloatValue(QString name, float value)
{
	if (floatValue(name) != value) {
		_settings.setValue(name, value);
		emit changed();
	}
}

float JokerSettings::floatValue(QString name, float defaultValue)
{
	return _settings.value(name, defaultValue).toFloat();
}

void JokerSettings::setStringValue(QString name, QString value)
{
	if (stringValue(name) != value) {
		_settings.setValue(name, value);
		emit changed();
	}
}

QString JokerSettings::stringValue(QString name, QString defaultValue)
{
	return _settings.value(name, defaultValue).toString();
}

void JokerSettings::setStringList(QString name, QStringList list)
{
	_settings.remove(name);
	_settings.beginWriteArray(name);
	for(int i = 0; i < list.size(); i++) {
		_settings.setArrayIndex(i);
		_settings.setValue("listItem", list.at(i));
	}
	_settings.endArray();

	emit changed();
}

QStringList JokerSettings::stringList(QString name, QStringList defaultValue)
{
	QStringList list;
	int size = _settings.beginReadArray(name);
	if(size == 0)
		list = defaultValue;
	else {
		for(int i = 0; i < size; i++) {
			_settings.setArrayIndex(i);
			list.append(_settings.value("listItem").toString());
		}
	}
	_settings.endArray();

	return list;
}

void JokerSettings::setByteArray(QString name, QByteArray array)
{
	if (byteArray(name) != array) {
		_settings.setValue(name, array);
		emit changed();
	}
}

QByteArray JokerSettings::byteArray(QString name)
{
	return _settings.value(name).toByteArray();
}
