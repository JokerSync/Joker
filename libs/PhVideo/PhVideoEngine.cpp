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
	_oldFrame(PHFRAMEMIN),
	_decoder(NULL)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();
}

bool PhVideoEngine::open(QString fileName)
{
	PHDEBUG << fileName;

	this->close();

	_clock.setFrame(0);
	_oldFrame = PHFRAMEMIN;

	_decoder = new PhAVDecoder(_settings->videoBufferSize());
	if(!_decoder->open(fileName))
		return false;

	_fileName = fileName;

	_clock.setTimeCodeType(_decoder->timeCodeType());

	QThread *thread = new QThread;

	// Setup the thread worker according to
	// http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
	_decoder->moveToThread(thread);
	connect(_decoder, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	connect(thread, SIGNAL(started()), _decoder, SLOT(process()));
	connect(_decoder, SIGNAL(finished()), thread, SLOT(quit()));
	connect(_decoder, SIGNAL(finished()), _decoder, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(&_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), _decoder, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), _decoder, SLOT(onRateChanged(PhRate)));

	thread->start(QThread::LowestPriority);

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

void PhVideoEngine::setDeinterlace(bool deinterlace)
{
	_oldFrame = PHFRAMEMIN;
	if(_decoder)
		_decoder->setDeinterlace(deinterlace);
}

void PhVideoEngine::setSettings(PhVideoSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	if(_decoder) {
		PhFrame frame = _clock.frame();
		if(_settings)
			frame += _settings->screenDelay() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;

		if(frame != _oldFrame) {
			uint8_t *buffer = _decoder->getBuffer(frame);
			if(buffer) {
				int height = _decoder->height();
				if(_decoder->deinterlace())
					height = height / 2;
				_videoRect.createTextureFromRGBBuffer(buffer, _decoder->width(), height);
				_oldFrame = frame;
				_frameCounter.tick();
			}
		}
		_videoRect.setRect(x, y, w, h);
		_videoRect.setZ(-10);
		_videoRect.draw();
	}
}

int PhVideoEngine::bufferSize()
{
	return _settings->videoBufferSize();
}

int PhVideoEngine::bufferOccupation()
{
	if(_decoder)
		return _decoder->bufferOccupation();
	return 0;
}

void PhVideoEngine::errorString(QString msg)
{
	PHDEBUG << msg;
}

void PhVideoEngine::setFirstFrame(PhFrame frame)
{
	if(_decoder)
		_decoder->setFirstFrame(frame);
}

PhVideoEngine::~PhVideoEngine()
{
	close();
}

PhFrame PhVideoEngine::firstFrame()
{
	if(_decoder)
		return _decoder->firstFrame();
	return 0;
}

PhFrame PhVideoEngine::lastFrame()
{
	if(_decoder)
		return _decoder->firstFrame() + _decoder->length() - 1;
	return 0;
}

PhFrame PhVideoEngine::length()
{
	if(_decoder)
		return _decoder->length();
	return 0;
}

float PhVideoEngine::framePerSecond()
{
	if(_decoder)
		return _decoder->length();
	return 0.0f;
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
