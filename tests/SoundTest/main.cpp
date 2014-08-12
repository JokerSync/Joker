#include <QApplication>
#include <QObject>
#include <QIODevice>

#include "PhTools/PhDebug.h"

#include "generator.h"
#include "audioinput.h"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 50;
const int DataSampleRateHz = 48000;
const int BufferSize      = 32768 * 2;


/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QAudioFormat m_format;

#if 1 /* This part is for output testing*/
	QAudioDeviceInfo m_device = QAudioDeviceInfo::defaultOutputDevice();


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
	int low = 100;

	m_generator->start();
	m_audioOutput->start(m_generator);
	while (true) {
		int freq = qrand() % ((high + 1) - low) + low;;
		m_audioOutput->stop();
		m_generator->generateData(m_format, DurationSeconds * 1000000, freq);
		m_audioOutput->start(m_generator);
		//sleep(1);

	}
	//    m_generator->stop();

#else

	AudioInfo *m_audioInfo = new AudioInfo(m_format, NULL);
	QAudioInput *m_audioInput;
	QAudioDeviceInfo m_device = QAudioDeviceInfo::defaultInputDevice();


	QIODevice *m_input = 0;
	QByteArray m_buffer(BufferSize, 0);


	m_format.setSampleRate(48000);
	m_format.setChannelCount(1);
	m_format.setSampleSize(8);
	m_format.setSampleType(QAudioFormat::SignedInt);
	m_format.setByteOrder(QAudioFormat::LittleEndian);
	m_format.setCodec("audio/pcm");

	QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

	QAudioDeviceInfo info;
	foreach(QAudioDeviceInfo input, list) {
		if(input.deviceName() == "Built-in Input")
			info = input;
	}

	if(info.isNull()) {
		PHDEBUG << "Error with input";
		return 1;
	}

	if (!info.isFormatSupported(m_format)) {
		qWarning() << "Default format not supported - trying to use nearest";
		m_format = info.nearestFormat(m_format);
	}

	m_audioInfo  = new AudioInfo(m_format, NULL);
	m_audioInput = new QAudioInput(m_device, m_format, NULL);

	PHDEBUG << m_audioInfo->level();

	m_audioInfo->start();
	m_input = m_audioInput->start();

	qint64 procUSecs = 0;
	// Stop after 10 seconds
	while(m_audioInput->elapsedUSecs() / 1000000 < 10) {
		qint64 len = m_audioInput->bytesReady();
		if (len > BufferSize)
			len = BufferSize;
		qint64 l = m_input->read(m_buffer.data(), len);
		if (l > 0)
			m_audioInfo->write(m_buffer.constData(), l);
		//        if (procUSecs != m_audioInput->processedUSecs())
//        {
//            qWarning() << "bytesReady = " << m_audioInput->bytesReady()
//                       << ", " << "elapsedUSecs = " <<m_audioInput->elapsedUSecs()
//                       << ", " << "processedUSecs = "<<m_audioInput->processedUSecs();
//            procUSecs = m_audioInput->processedUSecs();
//        }
	}

#endif

	return 0;
}
