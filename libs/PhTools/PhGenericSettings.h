#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

// This page was is a great resource that helped construct these useful macro:
// https://gcc.gnu.org/onlinedocs/cpp/Macros.html#Macros

/** Implement the integer setter and getter for a PhGenericSettings */
#define PH_SETTING_INT(setter, getter) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(int value) { if (setIntValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	int getter() {return intValue(#getter); }

/** Implement the integer setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_INT2(setter, getter, defaultValue) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(int value) { if (setIntValue(#getter, value)) { emit getter##Changed(); } } \
	void re ## setter() { if (setIntValue(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	int getter() {return intValue(#getter, defaultValue); }

/** Implement the integer setter, getter and alias for a PhGenericSettings */
#define PH_SETTING_INT3(setter, getter, alias) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(int value) { if (setIntValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	int getter() {return intValueWithAlias(#getter, #alias); }

/** Implement the unsigned char setter and getter for a PhGenericSettings */
#define PH_SETTING_UCHAR(setter, getter) \
Q_PROPERTY(unsigned char getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(unsigned char value) { if (setIntValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	unsigned char getter() {return intValue(#getter); }

/** Implement the unsigned char setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_UCHAR2(setter, getter, defaultValue) \
Q_PROPERTY(unsigned char getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(unsigned char value) { if (setIntValue(#getter, value)) { emit getter##Changed(); } } \
	void re ## setter() { if (setIntValue(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	unsigned char getter() {return intValue(#getter, defaultValue); }

/** Implement the bool setter and getter for a PhGenericSettings */
#define PH_SETTING_BOOL(setter, getter) \
Q_PROPERTY(bool getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(bool value) { if (setBoolValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	bool getter() {return boolValue(#getter); }

/** Implement the bool setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_BOOL2(setter, getter, defaultValue) \
Q_PROPERTY(bool getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
   void getter##Changed(); \
public slots: \
	void setter(bool value) { if (setBoolValue(#getter, value)) { emit getter##Changed(); } } \
	void re ## setter() { if (setBoolValue(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	bool getter() {return boolValue(#getter, defaultValue); } \

/** Implement the float setter and getter for a PhGenericSettings */
#define PH_SETTING_FLOAT(setter, getter) \
Q_PROPERTY(float getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(float value) { if (setFloatValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	float getter() {return floatValue(#getter); }

/** Implement the float setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_FLOAT2(setter, getter, defaultValue) \
Q_PROPERTY(float getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(float value) { if (setFloatValue(#getter, value)) { emit getter##Changed(); } } \
	void re ## setter() { if (setFloatValue(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	float getter() {return floatValue(#getter, defaultValue); }

/** Implement the string setter and getter for a PhGenericSettings */
#define PH_SETTING_STRING(setter, getter) \
Q_PROPERTY(QString getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QString value) { if (setStringValue(#getter, value)) { emit getter##Changed(); } } \
public: \
	QString getter() {return stringValue(#getter); }

/** Implement the string setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_STRING2(setter, getter, defaultValue) \
Q_PROPERTY(QString getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QString value) { if (setStringValue(#getter, value)) { emit getter##Changed(); } } \
	void re ## setter() { if (setStringValue(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	QString getter() {return stringValue(#getter, defaultValue); }

/** Implement the string list setter and getter for a PhGenericSettings */
#define PH_SETTING_STRINGLIST(setter, getter) \
Q_PROPERTY(QStringList getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QStringList list) { if (setStringList(#getter, list)) { emit getter##Changed(); } } \
public: \
	QStringList getter() {return stringList(#getter); }

/** Implement the string list setter and getter qnd default value for a PhGenericSettings */
#define PH_SETTING_STRINGLIST2(setter, getter, defaultValue) \
Q_PROPERTY(QStringList getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QStringList list) { if (setStringList(#getter, list)) { emit getter##Changed(); } } \
	void re ## setter() { if (setStringList(#getter, defaultValue)) { emit getter##Changed(); } } \
public: \
	QStringList getter() {return stringList(#getter, defaultValue); }

/** Implement the byte array setter and getter for a PhGenericSettings */
#define PH_SETTING_BYTEARRAY(setter, getter) \
Q_PROPERTY(QByteArray getter READ getter WRITE setter NOTIFY getter##Changed) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QByteArray array) { if (setByteArray(#getter, array)) { emit getter##Changed(); } } \
public: \
	QByteArray getter() {return byteArray(#getter); }

/** Implement the hash setter and getter for a PhGenericSettings */
//	Q_PROPERTY does not work here, there is no single-argument setter
#define PH_SETTING_HASH(setter, getter) \
Q_SIGNALS: \
	void getter##Changed(); \
public slots: \
	void setter(QString key, QVariant value) { if (setHash(#getter, key, value)) { emit getter##Changed(); } } \
public: \
	QVariant getter(QString key) {return hash(#getter, key); }

/**
 * @brief A generic implementation of the module settings
 *
 * PhGenericSettings and its macro provide an easyier way to managed
 * the application settings by supplying an overlay of the QSettings
 * behaviour.
 * The main interest is to centralize the default value of each settings
 * and to insure settings name unicity and homogeneity.
 */

class PhGenericSettings : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhGenericSettings constructor
	 * @param clear True if the settings shall be reset
	 */
	PhGenericSettings(bool clear = false);

	/**
	 * @brief Reset the settings to its default value.
	 */
	void clear();

protected:
	/**
	 * @brief Set an integer value
	 * @param name The settings name
	 * @param value The integer value
	 * @return Whether the value has changed
	 */
	bool setIntValue(QString name, int value);
	/**
	 * @brief Get an integer value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The integer value
	 */
	int intValue(QString name, int defaultValue = 0);
	/**
	 * @brief Set a long long value (64 bit)
	 * @param name The settings name
	 * @param value The long long value
	 * @return Whether the value has changed
	 */
	bool setLongLongValue(QString name, qlonglong value);
	/**
	 * @brief Get a long long value (64 bit)
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The long long value
	 */
	qlonglong longLongValue(QString name, qlonglong defaultValue = 0);
	/**
	 * @brief Get an integer value with alias
	 *
	 * @param name The settings name
	 * @param alias An alias
	 * @return The integer value
	 */
	int intValueWithAlias(QString name, QString alias);

	/**
	 * @brief Set a bool value
	 * @param name The settings name
	 * @param value The bool value
	 * @return Whether the value has changed
	 */
	bool setBoolValue(QString name, bool value);
	/**
	 * @brief Get a bool value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The bool value
	 */
	bool boolValue(QString name, bool defaultValue = false);

	/**
	 * @brief Set a float value
	 * @param name The settings name
	 * @param value The float value
	 * @return Whether the value has changed
	 */
	bool setFloatValue(QString name, float value);
	/**
	 * @brief Get a float value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The float value
	 */
	float floatValue(QString name, float defaultValue = 0);
	/**
	 * @brief Set a string value
	 * @param name The settings name
	 * @param value The string value
	 * @return Whether the value has changed
	 */
	bool setStringValue(QString name, QString value);
	/**
	 * @brief Get a string value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The string value
	 */
	QString stringValue(QString name, QString defaultValue = "");
	/**
	 * @brief Set a string list
	 * @param name The settings name
	 * @param list The string list
	 * @return Whether the value has changed
	 */
	bool setStringList(QString name, QStringList list);
	/**
	 * @brief Get a string list
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The string list
	 */
	QStringList stringList(QString name, QStringList defaultValue = QStringList());

	/**
	 * @brief Set a byte array
	 * @param name The settings name
	 * @param array The byte array
	 * @return Whether the value has changed
	 */
	bool setByteArray(QString name, QByteArray array);
	/**
	 * @brief Get a byte array
	 * @param name The settings name
	 * @return The byte array
	 */
	QByteArray byteArray(QString name);

	/**
	 * @brief Set a hash value for a key
	 * @param name Name of the hash
	 * @param key Key
	 * @param value Value
	 * @return Whether the value has changed
	 */
	bool setHash(QString name, QString key, QVariant value);

	/**
	 * @brief Get a hash value for a key
	 * @param name Name of the hash
	 * @param key Key
	 * @return A value
	 */
	QVariant hash(QString name, QString key);

	/**
	 * @brief Get a full hash (containing multiple values)
	 * @param name The settings name
	 * @return The hash
	 */
	QHash<QString, QVariant> fullHash(QString name);

protected:
	/**
	 * @brief The QSettings object
	 */
	QSettings _settings;

	// caches
	QMap<QString, int> _intValues;
	QMap<QString, qlonglong> _longLongValues;
	QMap<QString, float> _floatValues;
	QMap<QString, bool> _boolValues;
	QMap<QString, QString> _stringValues;
	QMap<QString, QByteArray> _byteArrayValues;
	QMap<QString, QStringList> _stringListValues;
	QMap<QString, QHash<QString, QVariant>> _hashValues;
};

#endif // PHGENERICSETTINGS_H
