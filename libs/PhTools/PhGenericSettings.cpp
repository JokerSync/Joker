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

	// clear caches
	_intValues.clear();
	_longLongValues.clear();
	_floatValues.clear();
	_boolValues.clear();
	_stringValues.clear();
	_byteArrayValues.clear();
	_stringListValues.clear();
	_hashValues.clear();
}

bool PhGenericSettings::setIntValue(QString name, int value)
{
	if (intValue(name) != value) {
		_intValues[name] = value;
		_settings.setValue(name, value);
		return true;
	}
	return false;
}

int PhGenericSettings::intValue(QString name, int defaultValue)
{
	if (!_intValues.contains(name)) {
		_intValues[name] = _settings.value(name, defaultValue).toInt();
	}

	return _intValues[name];
}

bool PhGenericSettings::setLongLongValue(QString name, qlonglong value)
{
	if (longLongValue(name) != value) {
		_longLongValues[name] = value;
		_settings.setValue(name, value);
		return true;
	}
	return false;
}

qlonglong PhGenericSettings::longLongValue(QString name, qlonglong defaultValue)
{
	if (!_longLongValues.contains(name)) {
		_longLongValues[name] = _settings.value(name, defaultValue).toLongLong();
	}

	return _longLongValues[name];
}

int PhGenericSettings::intValueWithAlias(QString name, QString alias)
{
	/// If the regular value is 0, return the alias value
	int result = intValue(name);
	if(result == 0)
		result = intValue(alias);
	return result;
}

bool PhGenericSettings::setBoolValue(QString name, bool value)
{
	if (boolValue(name) != value) {
		_boolValues[name] = value;
		_settings.setValue(name, value);
		return true;
	}
	return false;
}

bool PhGenericSettings::boolValue(QString name, bool defaultValue)
{
	if (!_boolValues.contains(name)) {
		_boolValues[name] = _settings.value(name, defaultValue).toBool();
	}

	return _boolValues[name];
}

bool PhGenericSettings::setFloatValue(QString name, float value)
{
	if (floatValue(name) != value) {
		_floatValues[name] = value;
		_settings.setValue(name, value);
		return true;
	}
	return false;
}

float PhGenericSettings::floatValue(QString name, float defaultValue)
{
	if (!_floatValues.contains(name)) {
		_floatValues[name] = _settings.value(name, defaultValue).toFloat();
	}

	return _floatValues[name];
}

bool PhGenericSettings::setStringValue(QString name, QString value)
{
	if (stringValue(name) != value) {
		_stringValues[name] = value;
		_settings.setValue(name, value);
		return true;
	}
	return false;
}

QString PhGenericSettings::stringValue(QString name, QString defaultValue)
{
	if (!_stringValues.contains(name)) {
		_stringValues[name] = _settings.value(name, defaultValue).toString();
	}

	return _stringValues[name];
}

bool PhGenericSettings::setStringList(QString name, QStringList list)
{
	_stringListValues[name] = list;

	_settings.remove(name);
	_settings.beginWriteArray(name);
	for(int i = 0; i < list.size(); i++) {
		_settings.setArrayIndex(i);
		_settings.setValue("listItem", list.at(i));
	}
	_settings.endArray();

	// always return that a change occured. Actually comparing values is overkill.
	return true;
}

QStringList PhGenericSettings::stringList(QString name, QStringList defaultValue)
{
	if (!_stringListValues.contains(name)) {
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

		_stringListValues[name] = list;
	}

	return _stringListValues[name];
}

bool PhGenericSettings::setByteArray(QString name, QByteArray array)
{
	if (byteArray(name) != array) {
		_byteArrayValues[name] = array;
		_settings.setValue(name, array);
		return true;
	}
	return false;
}

QByteArray PhGenericSettings::byteArray(QString name)
{
	if (!_byteArrayValues.contains(name)) {
		_byteArrayValues[name] = _settings.value(name).toByteArray();
	}

	return _byteArrayValues[name];
}

QHash<QString, QVariant> PhGenericSettings::fullHash(QString name)
{
	if (!_hashValues.contains(name)) {
		_hashValues[name] = _settings.value(name).toHash();
	}

	return _hashValues[name];
}

bool PhGenericSettings::setHash(QString name, QString key, QVariant value)
{
	QHash<QString, QVariant> hash = fullHash(name);
	if (hash[key] != value) {
		hash[key] = value;
		_hashValues[name] = hash;
		_settings.setValue(name, hash);
		return true;
	}
	return false;
}

QVariant PhGenericSettings::hash(QString name, QString key)
{
	return fullHash(name)[key];
}
