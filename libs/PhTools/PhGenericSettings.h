#ifndef PHGENERICSETTINGS_H
#define PHGENERICSETTINGS_H

#include "PhTools/PhData.h"

/** Implement the integer setter and getter for a PhGenericSettings */
#define PH_SETTING_INT(setter, getter) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValue(#getter); }

/** Implement the integer setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_INT2(setter, getter, defaultValue) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValue(#getter, defaultValue); }

/** Implement the integer setter, getter and alias for a PhGenericSettings */
#define PH_SETTING_INT3(setter, getter, alias) \
Q_PROPERTY(int getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(int getter) { setIntValue(#getter, getter); } \
public: \
	int getter() {return intValueWithAlias(#getter, #alias); }

/** Implement the unsigned char setter and getter for a PhGenericSettings */
#define PH_SETTING_UCHAR(setter, getter) \
Q_PROPERTY(unsigned char getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
public: \
	unsigned char getter() {return intValue(#getter); }

/** Implement the unsigned char setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_UCHAR2(setter, getter, defaultValue) \
Q_PROPERTY(unsigned char getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(unsigned char getter) { setIntValue(#getter, getter); } \
public: \
	unsigned char getter() {return intValue(#getter, defaultValue); }

/** Implement the bool setter and getter for a PhGenericSettings */
#define PH_SETTING_BOOL(setter, getter) \
Q_PROPERTY(bool getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
public: \
	bool getter() {return boolValue(#getter); }

/** Implement the bool setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_BOOL2(setter, getter, defaultValue) \
Q_PROPERTY(bool getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(bool getter) { setBoolValue(#getter, getter); } \
public: \
	bool getter() {return boolValue(#getter, defaultValue); }


/** Implement the float setter and getter for a PhGenericSettings */
#define PH_SETTING_FLOAT(setter, getter) \
Q_PROPERTY(float getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(float getter) { setFloatValue(#getter, getter); } \
public: \
	float getter() {return floatValue(#getter); }

/** Implement the float setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_FLOAT2(setter, getter, defaultValue) \
Q_PROPERTY(float getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(float getter) { setFloatValue(#getter, getter); } \
public: \
	float getter() {return floatValue(#getter, defaultValue); }

/** Implement the string setter and getter for a PhGenericSettings */
#define PH_SETTING_STRING(setter, getter) \
Q_PROPERTY(QString getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(QString getter) { setStringValue(#getter, getter); } \
public: \
	QString getter() {return stringValue(#getter); }

/** Implement the string setter, getter and default value for a PhGenericSettings */
#define PH_SETTING_STRING2(setter, getter, defaultValue) \
Q_PROPERTY(QString getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(QString getter) { setStringValue(#getter, getter); } \
public: \
	QString getter() {return stringValue(#getter, defaultValue); }

/** Implement the string list setter and getter for a PhGenericSettings */
#define PH_SETTING_STRINGLIST(setter, getter) \
Q_PROPERTY(QStringList getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(QStringList list) { setStringList(#getter, list); } \
public: \
	QStringList getter() {return stringList(#getter); }

/** Implement the string list setter and getter qnd default value for a PhGenericSettings */
#define PH_SETTING_STRINGLIST2(setter, getter, defaultValue) \
Q_PROPERTY(QStringList getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(QStringList list) { setStringList(#getter, list); } \
public: \
	QStringList getter() {return stringList(#getter, defaultValue); }

/** Implement the byte array setter and getter for a PhGenericSettings */
#define PH_SETTING_BYTEARRAY(setter, getter) \
Q_PROPERTY(QByteArray getter READ getter WRITE setter NOTIFY changed) \
public slots: \
	void setter(QByteArray array) { setByteArray(#getter, array); } \
public: \
	QByteArray getter() {return byteArray(#getter); }

/**
 * @brief A generic implementation of the module settings
 *
 * PhGenericSettings macros provide an easyier way to managed
 * the application settings by supplying an overlay of the QSettings
 * behaviour.
 * The main interest is to centralize the default value of each settings
 * and to insure settings name unicity and homogeneity.
 */

#endif // PHGENERICSETTINGS_H
