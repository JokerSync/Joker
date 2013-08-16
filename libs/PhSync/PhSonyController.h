#ifndef PHSONYCONTROLLER_H
#define PHSONYCONTROLLER_H

#include <QObject>
#include <QSerialPort>

/**
 * PhSonyController is an abstract class handle sony 9 pin communication
 * through the serial port.
 * It can be connected to a clock to update it accordingly.
 */
class PhSonyController : public QObject
{
	Q_OBJECT
public:
	explicit PhSonyController(QString comSuffix, QObject *parent);
	~PhSonyController();

	bool open();
	void close();

	void test();
signals:

public slots:

private:
	QString _comSuffix;
	QSerialPort _serial;

private slots:
	void handleError(QSerialPort::SerialPortError error);
};

#endif // PHSONYCONTROLLER_H
