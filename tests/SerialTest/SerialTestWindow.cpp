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

	connect(&_ctsTimer, SIGNAL(timeout()), this, SLOT(checkCTS()));

	connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendTextB()));

	ui->checkA->setChecked(_settings->activatePortA());
	on_checkA_toggled(_settings->activatePortA());
	ui->checkB->setChecked(_settings->activatePortB());
	on_checkB_toggled(_settings->activatePortB());
}

SerialTestWindow::~SerialTestWindow()
{
	FT_Close(_serialA);
	FT_Close(_serialB);
	delete ui;
}

void SerialTestWindow::sendTextA()
{
	QString s = ui->inputA->text();
	DWORD byteWritten = 0;
	FT_Write(_serialA, (void*)s.toUtf8().constData(), s.length() + 1, &byteWritten);
	PHDEBUG << byteWritten << "bytes written";
}

void SerialTestWindow::sendTextB()
{
	QString s = ui->inputB->text();
	DWORD byteWritten = 0;
	FT_Write(_serialB, (void*)s.toUtf8().constData(), s.length() + 1, &byteWritten);
	PHDEBUG << byteWritten << "bytes written";
}

void SerialTestWindow::readTextA()
{
	DWORD byteReceived = 0;
	DWORD byteTransmitted = 0;
	DWORD event = 0;
	DWORD byteRead = 0;

	FT_GetStatus(_serialA, &byteReceived, &byteTransmitted, &event);
	if(byteReceived) {
		char *buffer = new char[byteReceived];
		FT_Read(_serialA, buffer, byteReceived, &byteRead);
		ui->receiveA->setText(ui->receiveA->toPlainText() + buffer);
		PHDEBUG << byteReceived << "bytes received /" << byteRead << "bytes read";
	}
}

void SerialTestWindow::readTextB()
{
	DWORD byteReceived = 0;
	DWORD byteTransmitted = 0;
	DWORD event = 0;
	DWORD byteRead = 0;

	FT_GetStatus(_serialB, &byteReceived, &byteTransmitted, &event);
	if(byteReceived) {
		char *buffer = new char[byteReceived];
		FT_Read(_serialB, buffer, byteReceived, &byteRead);
		ui->receiveB->setText(ui->receiveB->toPlainText() + buffer);
		PHDEBUG << byteReceived << "bytes received /" << byteRead << "bytes read";
	}
}

void SerialTestWindow::on_checkA_toggled(bool checked)
{
	_settings->setActivatePortA(checked);
	if(checked) {
		if(open(&_serialA, _settings->portAName())) {
			_ctsTimer.start(10);
		}
		else
			QMessageBox::critical(this, "Serial Test", QString("Unable to connect to %1").arg(_settings->portAName()));
	}
	else {
		_ctsTimer.stop();
		FT_Close(_serialA);
	}
}

void SerialTestWindow::on_checkB_toggled(bool checked)
{
	_settings->setActivatePortB(checked);
	if(checked) {
		if(!open(&_serialB, _settings->portBName()))
			QMessageBox::critical(this, "Serial Test", QString("Unable to connect to %1").arg(_settings->portBName()));
	}
	else
		FT_Close(_serialB);
}

bool SerialTestWindow::open(FT_HANDLE * serial, QString suffix)
{
	PHDEBUG << "open" << suffix;

	DWORD deviceCount = 0;
	if(FT_CreateDeviceInfoList(&deviceCount) == FT_OK) {
		FT_DEVICE_LIST_INFO_NODE *infos = new FT_DEVICE_LIST_INFO_NODE[deviceCount];
		FT_GetDeviceInfoList(infos, &deviceCount);
		for(int i = 0; i < deviceCount; i++) {
			if(QString(infos[i].Description).endsWith(suffix)) {
				if(FT_Open(i, serial) == FT_OK) {
					FT_SetBaudRate(*serial, FT_BAUD_38400);
					FT_SetDataCharacteristics(*serial, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_ODD);
					return true;
				}
			}
		}
	}

	return false;
}

void SerialTestWindow::checkCTS()
{
	ULONG status = 0;
	if(FT_GetModemStatus(_serialA, &status) == FT_OK) {

		bool cts = status & 0x10;
		float frequency = _ctsCounter.frequency();
		if(cts != _lastCTS) {
			_ctsCounter.tick();
			ui->ctsLabel->setText("CTS : " + QString::number(frequency));
			_lastCTS = cts;
		}
	}

	readTextA();
	readTextB();

	_timerCounter.tick();
}
