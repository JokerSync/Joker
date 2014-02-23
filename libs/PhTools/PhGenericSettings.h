#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QByteArray>

/** Implement the integer setter and getter for a PhGenericSettings */
#define PH_SETTING_INT(setter, getter) \
	void setter(int getter) { setIntValue(#getter, getter); } \
	int getter() {return intValue(#getter); }

/** Implement the integer setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_INT2(setter, getter, defaultValue) \
	void setter(int getter) { setIntValue(#getter, getter); } \
	int getter() {return intValue(#getter, defaultValue); }

/** Implement the unsigned char setter and getter for a PhGenericSettings */
#define PH_SETTING_UCHAR(setter, getter) \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
	unsigned char getter() {return intValue(#getter); }

/** Implement the unsigned char setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_UCHAR2(setter, getter, defaultValue) \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
	unsigned char getter() {return intValue(#getter, defaultValue); }

/** Implement the bool setter and getter for a PhGenericSettings */
#define PH_SETTING_BOOL(setter, getter) \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
	bool getter() {return boolValue(#getter); }

/** Implement the bool setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_BOOL2(setter, getter, defaultValue) \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
	bool getter() {return boolValue(#getter, defaultValue); }

/** Implement the float setter and getter for a PhGenericSettings */
#define PH_SETTING_FLOAT(setter, getter) \
	void setter(float getter) { setFloatValue(#getter, getter); } \
	float getter() {return floatValue(#getter); }

/** Implement the float setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_FLOAT2(setter, getter, defaultValue) \
	void setter(float getter) { setFloatValue(#getter, getter); } \
	float getter() {return floatValue(#getter, defaultValue); }

/** Implement the string setter and getter for a PhGenericSettings */
#define PH_SETTING_STRING(setter, getter) \
	void setter(QString getter) { setStringValue(#getter, getter); } \
	QString getter() {return stringValue(#getter); }

/** Implement the string setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_STRING2(setter, getter, defaultValue) \
	void setter(QString getter) { setStringValue(#getter, getter); } \
	QString getter() {return stringValue(#getter, defaultValue); }

/** Implement the string list setter and getter for a PhGenericSettings */
#define PH_SETTING_STRINGLIST(setter, getter) \
	void setter(QStringList list) { setStringList(#getter, list); } \
	QStringList getter() {return stringList(#getter); }

/** Implement the byte array setter and getter for a PhGenericSettings */
#define PH_SETTING_BYTEARRAY(setter, getter) \
	void setter(QByteArray array) { setByteArray(#getter, array); } \
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
	 */
	PhGenericSettings();

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
	 * @return The string list
	 */
	QStringList stringList(QString name);

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

private:
	QSettings _settings;
};

#endif // PHGENERICSETTINGS_H
