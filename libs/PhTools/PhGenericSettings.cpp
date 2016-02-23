#include "PhGenericSettings.h"
#include "PhDebug.h"

PhGenericSettings::PhGenericSettings(bool clear) : _settings(PH_ORG_NAME, PH_APP_NAME)
{
	QSettings::setDefaultFormat(QSettings::NativeFormat);
	PHDEBUG << "Settings file:" << _settings.fileName();
	if(clear)
		this->clear();
}

void PhGenericSettings::clear()
{
	_settings.clear();
}

void PhGenericSettings::setIntValue(QString name, int value)
{
	_settings.setValue(name, value);
}

int PhGenericSettings::intValue(QString name, int defaultValue)
{
	return _settings.value(name, defaultValue).toInt();
}

void PhGenericSettings::setLongLongValue(QString name, qlonglong value)
{
	_settings.setValue(name, value);
}

qlonglong PhGenericSettings::longLongValue(QString name, qlonglong defaultValue)
{
	return _settings.value(name, defaultValue).toLongLong();
}

int PhGenericSettings::intValueWithAlias(QString name, QString alias)
{
	/// If the regular value is 0, return the alias value
	int result = intValue(name);
	if(result == 0)
		result = intValue(alias);
	return result;
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

void PhGenericSettings::setStringList(QString name, QStringList list)
{
	_settings.remove(name);
	_settings.beginWriteArray(name);
	for(int i = 0; i < list.size(); i++) {
		_settings.setArrayIndex(i);
		_settings.setValue("listItem", list.at(i));
	}
	_settings.endArray();
}

QStringList PhGenericSettings::stringList(QString name, QStringList defaultValue)
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

void PhGenericSettings::setByteArray(QString name, QByteArray array)
{
	_settings.setValue(name, array);
}

QByteArray PhGenericSettings::byteArray(QString name)
{
	return _settings.value(name).toByteArray();
}

void PhGenericSettings::setHash(QString name, QString key, QVariant value)
{
	QHash<QString, QVariant> hash = _settings.value(name).toHash();
	hash[key] = value;
	_settings.setValue(name, hash);
}

QVariant PhGenericSettings::hash(QString name, QString key)
{
	return _settings.value(name).toHash()[key];
}
