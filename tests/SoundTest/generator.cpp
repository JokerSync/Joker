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
        for (int i=0; i<format.channelCount(); ++i)
        {
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
