#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QByteArray>

/** Implement the integer setter and getter for a PhGenericSettings */
#define PH_SETTING_INT(setter, getter) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValue(#getter); }

/** Implement the integer setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_INT2(setter, getter, defaultValue) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValue(#getter, defaultValue); }

/** Implement the integer setter, getter and alias for a PhGenericSettings */
#define PH_SETTING_INT3(setter, getter, alias) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValueWithAlias(#getter, #alias); }

/** Implement the unsigned char setter and getter for a PhGenericSettings */
#define PH_SETTING_UCHAR(setter, getter) \
public slots: \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
public: \
	unsigned char getter() {return intValue(#getter); }

/** Implement the unsigned char setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_UCHAR2(setter, getter, defaultValue) \
public slots: \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
public: \
	unsigned char getter() {return intValue(#getter, defaultValue); }

/** Implement the bool setter and getter for a PhGenericSettings */
#define PH_SETTING_BOOL(setter, getter) \
public slots: \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
public: \
	bool getter() {return boolValue(#getter); }

/** Implement the bool setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_BOOL2(setter, getter, defaultValue) \
public slots: \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
public: \
	bool getter() {return boolValue(#getter, defaultValue); }

/** Implement the float setter and getter for a PhGenericSettings */
#define PH_SETTING_FLOAT(setter, getter) \
public slots: \
	void setter(float getter) { setFloatValue(#getter, getter); } \
public: \
	float getter() {return floatValue(#getter); }

/** Implement the float setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_FLOAT2(setter, getter, defaultValue) \
public slots: \
	void setter(float getter) { setFloatValue(#getter, getter); } \
public: \
	float getter() {return floatValue(#getter, defaultValue); }

/** Implement the string setter and getter for a PhGenericSettings */
#define PH_SETTING_STRING(setter, getter) \
public slots: \
	void setter(QString getter) { setStringValue(#getter, getter); } \
public: \
	QString getter() {return stringValue(#getter); }

/** Implement the string setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_STRING2(setter, getter, defaultValue) \
public slots: \
	void setter(QString getter) { setStringValue(#getter, getter); } \
public: \
	QString getter() {return stringValue(#getter, defaultValue); }

/** Implement the string list setter and getter for a PhGenericSettings */
#define PH_SETTING_STRINGLIST(setter, getter) \
public slots: \
	void setter(QStringList list) { setStringList(#getter, list); } \
public: \
	QStringList getter() {return stringList(#getter); }

/** Implement the string list setter and getter qnd default value for a PhGenericSettings */
#define PH_SETTING_STRINGLIST2(setter, getter, defaultValue) \
public slots: \
	void setter(QStringList list) { setStringList(#getter, list); } \
public: \
	QStringList getter() {return stringList(#getter, defaultValue); }

/** Implement the byte array setter and getter for a PhGenericSettings */
#define PH_SETTING_BYTEARRAY(setter, getter) \
public slots: \
	void setter(QByteArray array) { setByteArray(#getter, array); } \
public: \
	QByteArray getter() {return byteArray(#getter); }

/**
 * @brief A generic implementation of the module settings
 *
 * PhGenericSettings and its macro provide an easyier way to managed
 * the application settings by supplying an overlay of the QSettings
 * behaviour.
 * The main interest is to centralize the default value of each settings
 * and to insure settings name unicity and homogeneity.
 */
class PhGenericSettings
{
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
	 */
	void setIntValue(QString name, int value);
	/**
	 * @brief Get an integer value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The integer value
	 */
	int intValue(QString name, int defaultValue = 0);
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
	 */
	void setBoolValue(QString name, bool value);
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
	 */
	void setFloatValue(QString name, float value);
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
	 */
	void setStringValue(QString name, QString value);
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
	 */
	void setStringList(QString name, QStringList list);
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
	 */
	void setByteArray(QString name, QByteArray array);
	/**
	 * @brief Get a byte array
	 * @param name The settings name
	 * @return The byte array
	 */
	QByteArray byteArray(QString name);

protected:
	/**
	 * @brief The QSettings object
	 */
	QSettings _settings;
};

#endif // PHGENERICSETTINGS_H
