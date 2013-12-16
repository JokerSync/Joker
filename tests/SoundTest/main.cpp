#include <QCoreApplication>
#include <QObject>
#include <QIODevice>

#include "PhTools/PhDebug.h"
#include "PhTools/ltc.h"

#include "generator.h"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 50;
const int DataSampleRateHz = 48000;
const int BufferSize      = 32768 * 2;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAudioDeviceInfo m_device = QAudioDeviceInfo::defaultOutputDevice();


    QAudioFormat m_format;
    m_format.setSampleRate(DataSampleRateHz);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);


    QAudioOutput *m_audioOutput = new QAudioOutput(m_device, m_format);
    Generator * m_generator = new Generator(m_format, DurationSeconds*1000000, ToneSampleRateHz, NULL);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int high = 10000;
    int low = 1000;

    m_generator->start();
    m_audioOutput->start(m_generator);
    while (true)
    {
        int freq = qrand() % ((high + 1) - low) + low;;
        m_audioOutput->stop();
        m_generator->generateData(m_format, DurationSeconds * 1000000, freq);
        m_audioOutput->start(m_generator);
        sleep(1);

    }
    //    m_generator->stop();





    return a.exec();
    //return 0;
}
