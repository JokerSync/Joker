/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QDate>
#include <QDir>

#include <iostream>

#include "PhDebug.h"

PhDebug* PhDebug::_d = NULL;

void PhDebug::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	Q_UNUSED(type); Q_UNUSED(context);
	if(instance()->_logMask & (1 << instance()->_currentLogLevel)) {
		instance()->_mutex.lock();
		QString logMessage = "";

		// Display the date
		if(instance()->_displayDate) {
			logMessage += QDate::currentDate().toString("dd/MM/yyyy ");
		}

		// Display timestamp
		if (instance()->_displayTime) {
			logMessage += QTime::currentTime().toString("hh:mm:ss.zzz ");
		}

		// Display filename
		if (instance()->_displayFileName) {

			logMessage += QString(context.file).split("/").last() + "\t";
		}

		// Display function name
		if (instance()->_displayFunctionName) {
			logMessage += QString(context.function) + "\t";
		}

		// Display line number
		if (instance()->_displayLine) {
			logMessage += QString("@") + QString::number(context.line) + "\t";
		}

		logMessage += msg;

		if(instance()->_showConsole) {
			switch(type) {
			case QtWarningMsg:
			case QtCriticalMsg:
			case QtFatalMsg:
				std::cerr << logMessage.toStdString() << std::endl;
				break;
			default:
				std::cout << logMessage.toStdString() << std::endl;
				break;
			}
		}

		if(instance()->_textLog)
			*instance()->_textLog << logMessage << endl;

		instance()->_mutex.unlock();
	}
}

QString PhDebug::logLocation()
{
	return instance()->_logFileName;
}

void PhDebug::showConsole(bool show)
{
	instance()->_showConsole = show;
}

void PhDebug::setDisplay(bool date, bool time, bool fileName, bool functionName, bool line)
{
	instance()->_displayDate = date;
	instance()->_displayTime = time;
	instance()->_displayFileName = fileName;
	instance()->_displayFunctionName = functionName;
	instance()->_displayLine = line;
}

// Called if init() was forget
PhDebug* PhDebug::instance()
{
	if (!_d)   // Only allow one instance of class to be generated.
		_d = new PhDebug();
	return _d;
}

/**
 * @brief Message handler that filter all output
 */
void noMessageOutput(QtMsgType, const QMessageLogContext &, const QString &)
{

}

void PhDebug::enable()
{
	qInstallMessageHandler(instance()->messageOutput);
}

void PhDebug::disable()
{
	qInstallMessageHandler(noMessageOutput);
}

QDebug PhDebug::debug(const char *fileName, int lineNumber, const char *functionName, int messageLogLevel)
{
	instance()->_currentLogLevel = messageLogLevel;
	return QMessageLogger(fileName, lineNumber, functionName).debug();
}

QDebug PhDebug::error(const char *fileName, int lineNumber, const char *functionName)
{
	instance()->_currentLogLevel = 0;
	return QMessageLogger(fileName, lineNumber, functionName).critical();
}

PhDebug::PhDebug() : _currentLogLevel(0), _textLog(NULL)
{
	qInstallMessageHandler(this->messageOutput);

	QString logDirPath = QString("%1/%2/")
#if defined(Q_OS_MAC)
	                     .arg(QDir::homePath() + "/Library/Logs")
#elif defined(Q_OS_WIN)
	                     .arg(QString(qgetenv("APPDATA")))
#elif defined(Q_OS_LINUX)
	                     .arg("/var/log")
#endif
	                     .arg(PH_ORG_NAME);

	QDir logDir(logDirPath);
	if(!logDir.exists()) {
		QDir().mkdir(logDirPath);
	}
	_logFileName = QDir(logDirPath).absoluteFilePath(PH_APP_NAME + QString(".log"));
	QFile *f = new QFile(_logFileName);
	if(f->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		f->write("\n\n");
		_textLog = new QTextStream(f);
	}
	else
		delete f;

	_displayDate = false;
	_displayTime = true;
	_displayFunctionName = true;
	_displayFileName = true;
	_displayLine = true;
	_showConsole = true;
	_logMask = 1;
}

void PhDebug::setLogMask(int mask)
{
	instance()->_logMask = mask;
}

int PhDebug::logMask()
{
	return instance()->_logMask;
}
