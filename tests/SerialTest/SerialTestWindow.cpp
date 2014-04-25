#include <QSerialPortInfo>
#include <QMessageBox>

#include "SerialTestWindow.h"
#include "ui_SerialTestWindow.h"
#include "PhTools/PhDebug.h"

SerialTestWindow::SerialTestWindow(SerialTestSettings *settings) :
	_settings(settings),
	ui(new Ui::SerialTestWindow),
	_serialA(this),
	_serialB(this)
{
	ui->setupUi(this);

	connect(ui->sendButton1, SIGNAL(clicked()), this, SLOT(sendTextA()));
	connect(&_serialA, SIGNAL(readyRead()), this, SLOT(readTextA()));

	connect(&_ctsTimer, SIGNAL(timeout()), this, SLOT(checkCTS()));

	connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendTextB()));
	connect(&_serialB, SIGNAL(readyRead()), this, SLOT(readTextB()));

	ui->checkA->setChecked(_settings->activatePortA());
	on_checkA_toggled(_settings->activatePortA());
	ui->checkB->setChecked(_settings->activatePortB());
	on_checkB_toggled(_settings->activatePortB());
}

SerialTestWindow::~SerialTestWindow()
{
	_serialA.close();
	_serialB.close();
	delete ui;
}

void SerialTestWindow::sendTextA()
{
	_serialA.write(ui->inputA->text().toUtf8().constData());
}

void SerialTestWindow::sendTextB()
{
	_serialB.write(ui->inputB->text().toUtf8().constData());
}

void SerialTestWindow::readTextA()
{
	char buffer[256];
	qint64 n = _serialA.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	PHDEBUG << s;
	ui->receiveA->setText(ui->receiveA->toPlainText() + s);
}

void SerialTestWindow::readTextB()
{
	char buffer[256];
	qint64 n = _serialB.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
//	PHDEBUG << s;
	ui->receiveB->setText(ui->receiveB->toPlainText() + s);
}

void SerialTestWindow::on_checkA_toggled(bool checked)
{
	_settings->setActivatePortA(checked);
	if(checked) {
		if(open(&_serialA, _settings->portAName())) {
			_serialA.write("Hello from serial A");
			_ctsTimer.start(10);
		}
		else
			QMessageBox::critical(this, "Serial Test", QString("Unable to connect to %1").arg(_settings->portAName()));
	}
	else {
		_ctsTimer.stop();
		_serialA.close();
	}
}

void SerialTestWindow::on_checkB_toggled(bool checked)
{
	_settings->setActivatePortB(checked);
	if(checked) {
		if(open(&_serialB, _settings->portBName()))
			_serialB.write("Hello from serial B");
		else
			QMessageBox::critical(this, "Serial Test", QString("Unable to connect to %1").arg(_settings->portBName()));
	}
	else
		_serialB.close();
}

bool SerialTestWindow::open(QSerialPort * serial, QString suffix)
{
	PHDEBUG << "open" << suffix;

	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		PHDEBUG << name << "available";
		if(name.endsWith(suffix)) {
			serial->setPort(info);
			serial->setBaudRate(QSerialPort::Baud38400);
			serial->setDataBits(QSerialPort::Data8);
			serial->setStopBits(QSerialPort::OneStop);
			serial->setParity(QSerialPort::OddParity);

			PHDEBUG << "Opening " << name;
			serial->open(QSerialPort::ReadWrite);

			return true;
		}
	}
	PHDEBUG << "not found";
	return false;
}

void SerialTestWindow::checkCTS()
{
	bool cts = _serialA.pinoutSignals() & QSerialPort::ClearToSendSignal;
	float frequency = _ctsCounter.frequency();
	if(cts != _lastCTS) {
		_ctsCounter.tick();
		ui->ctsLabel->setText("CTS : " + QString::number(frequency));
		_lastCTS = cts;
	}

	_serialA.write("checkCTS");

	_timerCounter.tick();
	PHDEBUG << _timerCounter.frequency() << frequency;
}
