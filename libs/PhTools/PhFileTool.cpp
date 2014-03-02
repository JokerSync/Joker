#include "PhDebug.h"
#include "PhFileTool.h"

unsigned char PhFileTool::readChar(QFile &f)
{
	return (unsigned char)f.read(1).data()[0];
}

unsigned short PhFileTool::readShort(QFile &f, int logLevel, QString name)
{
	int offset = f.pos();
	unsigned short * p = (unsigned short*)f.read(2).data();
	unsigned short result = *p;

	PHDBG(logLevel) << QString::number(offset, 16).toStdString().c_str() << name.toStdString().c_str() << QString::number(result, 16).toStdString().c_str();

	return result;
}

int PhFileTool::readInt(QFile &f, int logLevel, QString name)
{
	int offset = f.pos();
	int * p = (int*)f.read(4).data();
	int result = *p;

	PHDBG(logLevel) << QString::number(offset, 16).toStdString().c_str() << name.toStdString().c_str() << result;

	return result;
}

QString PhFileTool::readString(QFile &f, int logLevel, QString name)
{
	int offset = f.pos();
	ushort size = readChar(f);

	QString result = "";
	if (size == 0xff) {
		size = readChar(f);
		if(size == 0xff)
			size = readShort(f, 4);
		ushort * tab = (ushort*)f.read(size * 2).data();
		result = QString::fromUtf16(tab, size);
	}
	else {
		size += readChar(f) << 8;
		QByteArray array = f.read(size);
		char * tab = array.data();
		for(int i = 0; i < size; i++)
			result += tab[i];
	}

	PHDBG(logLevel) << QString::number(offset, 16).toStdString().c_str() << name.toStdString().c_str() << result;

	return result;
}
