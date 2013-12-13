#include <QCoreApplication>
#include <QObject>


#include "PhTools/PhDebug.h"

#include "generator.h"

const int DurationSeconds = 10;
const int ToneSampleRateHz = 1000;
const int DataSampleRateHz = 44100;
const int BufferSize      = 32768;


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

    m_generator->start();
    m_audioOutput->start(m_generator);
    sleep(3);



    //return a.exec();
    return 0;
}
