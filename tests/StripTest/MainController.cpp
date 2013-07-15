/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "MainController.h"


MainController::MainController(QObject *parent) :
    QObject(parent)
{

    _doc = new PhStripDoc();
    loadSettings();
}
void MainController::loadSettings()
{

    if (QFile(QDir::homePath() + "/Library/Preferences/com.phonations.Joker.plist").exists()){
        // Try to create a settings file (temp)
#if defined(Q_OS_MAC)
        _settings = new QSettings(QDir::homePath() + "/Library/Preferences/com.phonations.Joker.plist", QSettings::NativeFormat);
#elif defined(Q_OS_UNIX)
#elif defined(Q_OS_WIN)
#else
        qDebug() << "unknown OS please report informations";
#endif
        qDebug() << "Settings loaded fine";
        getLastFile();
    }
    else
    {
        newSettings();
    }
}

void MainController::newSettings()
{
    qDebug() << "Prefs file is missing, creating new pref file with default values :";
#if defined(Q_OS_MAC)
    _settings = new QSettings(QDir::homePath() + "/Library/Preferences/com.phonations.Joker.plist", QSettings::NativeFormat);
#endif
    _settings->setValue("last_file", "");
    _settings->setValue("natural_scroll", true);

    QStringList values = _settings->allKeys();

    foreach (PhString value, values)
    {
        qDebug() << value << ":" <<  _settings->value(value).toString();
    }

}

PhString MainController::getLastFile()
{
    qDebug() << "lastfile :" << _settings->value("last_file", "").toString();
    return _settings->value("last_file").toString();
}

bool MainController::openDoc(PhString fileName)
{
    bool succeed = _doc->openDetX(fileName);
    //if (succeed)
        emit docChanged();
    return succeed;
}

PhStripDoc MainController::getDoc()
{
    return *_doc;
}

bool MainController::getNaturalScrollPref()
{
    return _settings->value("natural_scroll", "true").toBool();
}

void MainController::setLastFile(PhString filename)
{
    _settings->setValue("last_file", filename);
}


void MainController::onRateChanged2(float rate)
{
}

void MainController::onPositionChanged2(int move)
{
}
