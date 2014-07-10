/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHAUDIO_H
#define PHAUDIO_H

#include <QObject>

#include <portaudio.h>

/**
 * @brief A generic audio device
 *
 * It connects the audio callback to the child audio device processAudio() method.
 */
class PhAudio : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhAudio constructor
	 * @param parent The parent object
	 */
	explicit PhAudio(QObject *parent = 0);

	~PhAudio();

	/**
	 * @brief Initialize the device
	 *
	 * The method only check if portaudio is initialized.
	 *
	 * @param deviceName The device name
	 * @return True if success, false otherwise
	 */
	virtual bool init(QString deviceName);

	/**
	 * @brief Close the audio device
	 */
	void close();

signals:

public slots:

protected:
	/**
	 * @brief Send/request audio buffer for processing
	 *
	 * This method depends if it is an output or an input device:
	 * - output : ask for the next buffer audio data to be filled.
	 * - input : provide the latest read data.
	 *
	 * @param inputBuffer The input data buffer
	 * @param outputBuffer The output data buffer
	 * @param framesPerBuffer The number of frame in the buffer
	 * @return A PaStreamCallbackResult value
	 */
	virtual int processAudio(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer) = 0;

	/** @brief The stream */
	PaStream *_stream;

	/**
	 * @brief The audio callback
	 *
	 * This callback only redirect to the processAudio() method.
	 *
	 * @param inputBuffer The input data buffer
	 * @param outputBuffer The output data buffer
	 * @param framesPerBuffer The number of frame in the buffer
	 * @param statusFlags Flags indicating underflow or overflow conditions
	 * @param userData A pointer to the PhAudio device
	 * @return A PaStreamCallbackResult value
	 */
	static int audioCallback(const void *inputBuffer, void *outputBuffer,
	                         unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags statusFlags,
	                         void *userData );

private:
	bool _paInitOk;
};

#endif // PHAUDIO_H
