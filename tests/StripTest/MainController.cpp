/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "MainController.h"

#include "iostream"
using namespace std;


MainController::MainController(QObject *parent) :
    QObject(parent)
{
    _doc = new PhStripDoc();

#if defined(Q_OS_MAC)
       _settingsPath = QDir::homePath() + "/Library/Preferences/com.phonations.Joker.plist";
        _settingsFormat = QSettings::NativeFormat;
#elif defined(Q_OS_UNIX)
        _settingsPath = QDir::homePath() + "StripTest.ini"; //TODO : change to specific setting folder
        _settingsFormat = QSettings::IniFormat;
#elif defined(Q_OS_WIN)//TODO handle settings under Windows and trigger compilation error
#else
    //TODO trigger compilation error cf rtm
#endif
        loadSettings();
}
void MainController::loadSettings()
{
        // Try to load a settings file (temp)

        if(QFile( _settingsPath).exists())
        {
            _settings = new QSettings(_settingsPath, _settingsFormat);
            qDebug() << "Settings loaded fine";
            getLastFile();
    }
    else
    {
            qDebug() << "Prefs file is missing, creating new pref file with default values :";

            _settings = new QSettings(_settingsPath, _settingsFormat);

            _settings->setValue("last_file", "");
            _settings->setValue("natural_scroll", true);
    }

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
