#ifndef AUDIOTESTREADER_H
#define AUDIOTESTREADER_H

#include "PhAudio/PhAudioInput.h"

class AudioTestReader : public PhAudioInput
{
	Q_OBJECT
public:
	explicit AudioTestReader(QObject *parent = 0);

	int minLevel() {
		return _minLevel;
	}
	int maxLevel() {
		return _maxLevel;
	}

signals:
	void audioProcessed();

public slots:

protected:
	int processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer);

private:
	int _minLevel, _maxLevel;
};

#endif // AUDIOTESTREADER_H
