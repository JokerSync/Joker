#include "PhLtcWriter.h"

PhLtcWriter::PhLtcWriter(PhTimeCodeType tcType, QObject *parent) :
	QObject(parent),
	_clock(tcType),
	_encoder(NULL)
{
}

bool PhLtcWriter::init(QString output)
{

}

void PhLtcWriter::close()
{

}

PhClock *PhLtcWriter::clock()
{
	return &_clock;
}
