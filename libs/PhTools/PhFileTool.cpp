/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhData.h"

#include "PhDebug.h"
#include "PhFileTool.h"

unsigned char PhFileTool::readChar(QFile &f, int level, QString name)
{
	int offset = f.pos();
	unsigned char result = (unsigned char)f.read(1).data()[0];
	PHDBG(level) << PHNQ(QString::number(offset, 16)) << PHNQ(name) << PHNQ(QString::number(result, 16));
	return result;
}

unsigned short PhFileTool::readShort(QFile &f, int level, QString name)
{
	int offset = f.pos();
	unsigned short * p = (unsigned short*)f.read(2).data();
	unsigned short result = *p;

	PHDBG(level) << PHNQ(QString::number(offset, 16)) << PHNQ(name) << PHNQ(QString::number(result, 16));

	return result;
}

int PhFileTool::readInt(QFile &f, int level, QString name)
{
	int offset = f.pos();
	int * p = (int*)f.read(4).data();
	int result = *p;

	PHDBG(level) << PHNQ(QString::number(offset, 16)) << PHNQ(name) << result;

	return result;
}

QString PhFileTool::readString(QFile &f, int level, QString name)
{
	int internLevel = 4;
	int offset = f.pos();
	int size = readShort(f, internLevel);

	QString result = "";
	switch(size) {
	case 0xfeff:
		readChar(f, internLevel);
		size = readChar(f, internLevel);
		if(size == 0xff) {
			size = readShort(f, internLevel);
			if(size == 0xffff)
				size = readInt(f, internLevel);
		}
		result = QString::fromUtf16((ushort*)f.read(size * 2).data(), size);
		break;
	case 0xffff:
		size = readShort(f, 4);
		result = QString::fromUtf16((ushort*)f.read(size * 2).data(), size);
		break;
	default:
		{
			QByteArray array = f.read(size);
			char * tab = array.data();
			for(int i = 0; i < size; i++)
				result += tab[i];
		}
	}

	QString displayResult = result;
	QStringList resultSplit = result.split("\r\n");
	if(resultSplit.count() > 3) {
		result = resultSplit.first() + "\r\n...\r\n" + resultSplit.last();
	}

	PHDBG(level) << PHNQ(QString::number(offset, 16)) << PHNQ(name) << result << "(" << PHNQ(QString::number(size, 16)) << ")";

	return result;
}
