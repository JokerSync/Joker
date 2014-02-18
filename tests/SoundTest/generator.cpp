/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "generator.h"

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <qmath.h>
#include <qendian.h>

#include "PhTools/PhDebug.h"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 16500;
const int DataSampleRateHz = 44100;
const int BufferSize      = 32768;


Generator::Generator(const QAudioFormat &format,
                     qint64 durationUs,
                     int sampleRate,
                     QObject *parent)
	:   QIODevice(parent)
	,   m_pos(0)
{
	generateData(format, durationUs, sampleRate);
}

Generator::~Generator()
{

}

void Generator::start()
{
	open(QIODevice::ReadOnly);
}

void Generator::stop()
{
	m_pos = 0;
	close();
}

void Generator::generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate)
{
	const int channelBytes = format.sampleSize() / 8;

	qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
	                * durationUs / 1000000;

	m_buffer.resize(length);
	unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
	int sampleIndex = 0;

	PHDBG() << "Type :" << format.sampleType() << " Size : " << format.sampleSize() << " Channel Count : " << format.channelCount();

	while (length) {
		const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
		for (int i = 0; i < format.channelCount(); ++i) {
			qint16 value = static_cast<qint16>(x * 32767);
			qToLittleEndian<qint16>(value, ptr);

			ptr += channelBytes;
			length -= channelBytes;
		}
		++sampleIndex;
	}
}

qint64 Generator::readData(char *data, qint64 len)
{
	qint64 total = 0;
	while (len - total > 0) {
		const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
		memcpy(data + total, m_buffer.constData() + m_pos, chunk);
		m_pos = (m_pos + chunk) % m_buffer.size();
		total += chunk;
	}
	return total;
}

qint64 Generator::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);

	return 0;
}

qint64 Generator::bytesAvailable() const
{
	return m_buffer.size() + QIODevice::bytesAvailable();
}
