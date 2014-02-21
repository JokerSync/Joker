#ifndef PHAUDIO_H
#define PHAUDIO_H

#include <QObject>

#include <portaudio.h>

class PhAudio : public QObject
{
	Q_OBJECT
public:
	explicit PhAudio(QObject *parent = 0);

	~PhAudio();

	virtual bool init(QString deviceName);

	/**
	 * @brief Close the audio stream
	 */
	void close();

signals:

public slots:

protected:
	virtual int processAudio(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) = 0;

	PaStream *_stream;

	static int audioCallback(const void *inputBuffer, void *outputBuffer,
	                          unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
	                          void *userData );

private:
	bool _paInitOk;
};

#endif // PHAUDIO_H
