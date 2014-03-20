/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine() :  QObject(NULL),
	_settings(NULL),
	_fileName(""),
	_clock(PhTimeCodeType25),
	_decoder(NULL)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
}

bool PhVideoEngine::open(QString fileName)
{
	PHDEBUG << fileName;

	_clock.setFrame(0);

	_decoder = new PhAVDecoder();
	if(!_decoder->open(fileName))
		return false;

	_fileName = fileName;

	_firstFrame = _decoder->firstFrame();
	_clock.setTimeCodeType(_decoder->timeCodeType());

	QThread *thread = new QThread;

	_decoder->moveToThread(thread);
	connect(_decoder, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	connect(thread, SIGNAL(started()), _decoder, SLOT(process()));
	connect(_decoder, SIGNAL(finished()), thread, SLOT(quit()));
	connect(_decoder, SIGNAL(finished()), _decoder, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();

	return true;
}

void PhVideoEngine::close()
{
	if(_decoder) {
		_decoder->close();
		_decoder = NULL;
	}
	_fileName = "";
}

void PhVideoEngine::setSettings(PhVideoSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	if(_decoder) {
		PhFrame frame = _clock.frame() + _settings->screenDelay() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;
		if(frame != _oldFrame) {
			uint8_t *buffer = _decoder->getBuffer(frame);
			if(buffer) {
				_videoRect.createTextureFromARGBBuffer(buffer, _decoder->width(), _decoder->height());
				_oldFrame = frame;
#warning /// @todo delete buffer
				delete buffer;
				_frameCounter.tick();
			}
		}
		_videoRect.setRect(x, y, w, h);
		_videoRect.setZ(-10);
		_videoRect.draw();
	}
}

void PhVideoEngine::errorString(QString msg)
{
	PHDEBUG << msg;
}

void PhVideoEngine::setFirstFrame(PhFrame frame)
{
	_firstFrame = frame;
}

PhVideoEngine::~PhVideoEngine()
{
	close();
}

PhFrame PhVideoEngine::lastFrame()
{
	if(_decoder)
		return _firstFrame + _decoder->length() - 1;
	return 0;
}

PhFrame PhVideoEngine::length()
{
	if(_decoder)
		return _decoder->length();
	return 0;
}

int PhVideoEngine::width()
{
	if(_decoder)
		return _decoder->width();
	return 0;
}

int PhVideoEngine::height()
{
	if(_decoder)
		return _decoder->height();
	return 0;
}

QString PhVideoEngine::codecName()
{
	if(_decoder)
		return _decoder->codecName();
	return "";
}
