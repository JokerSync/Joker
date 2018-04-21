/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <qmath.h>
#include <QTimer>
#include <QScreen>
#include <QWindow>
#include <QFileInfo>

#include "PhTools/PhDebug.h"

#include "PhGraphicText.h"

#include "PhGraphicView.h"

PhGraphicView::PhGraphicView(QWidget *parent, PhGraphicView *shareWidget)
	: QGLWidget(parent, shareWidget),
	_settings(NULL),
	_dropDetected(0),
	_maxRefreshRate(0),
	_maxPaintDuration(0),
	_lastUpdateDuration(0),
	_maxUpdateDuration(0),
	_previousNsecsElapsed(0)
{
	_refreshTimer = new QTimer(this);
	connect(_refreshTimer, SIGNAL(timeout()), this, SLOT(onRefresh()));

	//set the screen frequency to the most common value (60hz);
	_screenFrequency = QGuiApplication::primaryScreen()->refreshRate();

	int timerInterval = static_cast<int>(500.0 / _screenFrequency);
	_refreshTimer->start( timerInterval);
	//PHDEBUG << "Refresh rate set to " << _screenFrequency << "hz, timer restart every" << timerInterval << "ms";
	_dropTimer.start();

	_timer.start();
}

PhGraphicView::PhGraphicView(int width, int height, QWidget *parent)
	: PhGraphicView(parent)
{
	int ratio = this->windowHandle()->devicePixelRatio();
	this->setGeometry(0, 0, width / ratio, height / ratio);
}

PhGraphicView::~PhGraphicView()
{
	_refreshTimer->stop();
}

void PhGraphicView::setGraphicSettings(PhGraphicSettings *settings)
{
	_settings = settings;
}

void PhGraphicView::resizeGL(int width, int height)
{
	if(height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLoadIdentity();
}

int PhGraphicView::refreshRate()
{
	return _frameTickCounter.frequency();
}

void PhGraphicView::addInfo(QString info)
{
	_infos.append(info);
}

int64_t PhGraphicView::compare(QString imageFile, int threshold, int width, int height)
{
	int ratio = this->windowHandle()->devicePixelRatio();
	if(width == 0)
		width = this->width() * ratio;
	if(height == 0)
		height = this->height() * ratio;
	int64_t totalDiff = 0;

	QImage result = this->renderPixmap(width, height).toImage();
	QImage expected(imageFile);
	if((expected.width() == 0) || (expected.height() == 0)) {
		PHDBG(9) << QString("Bad expected file: %1").arg(imageFile);
		totalDiff = -10;
	}
	else if(expected.size() != QSize(width, height)) {
		PHDBG(9) << QString("Bad size for %1: %2x%3 / %4x%5")
		    .arg(imageFile)
		    .arg(expected.width())
		    .arg(expected.height())
		    .arg(width)
		    .arg(height);
		totalDiff = -20;
	}
	else {
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				QRgb a = result.pixel(i, j);
				QRgb b = expected.pixel(i, j);
				long diff = qPow(qRed(a) - qRed(b), 2) + qPow(qGreen(a) - qGreen(b), 2) + qPow(qBlue(a) - qBlue(b), 2);
				totalDiff += diff;
			}
			if(totalDiff < 0) {
				// type overflow - stop with the max
				totalDiff = std::numeric_limits<int64_t>::max();
				break;
			}
		}
	}

	if(totalDiff > threshold || totalDiff < 0) {
		QFileInfo info(imageFile);
		QString resultFile = info.completeBaseName() + ".result.bmp";
		PHDBG(9) << "saving to " << resultFile;
		result.save(resultFile);
	}

	return totalDiff;
}

void PhGraphicView::onRefresh()
{
	if(this->refreshRate() > _maxRefreshRate)
		_maxRefreshRate = this->refreshRate();
	addInfo(QString("refresh: %1x%2, %3 / %4")
	        .arg(this->width())
	        .arg(this->height())
	        .arg(_maxRefreshRate)
	        .arg(this->refreshRate()));
	addInfo(QString("Update : %1 %2").arg(_maxUpdateDuration).arg(_lastUpdateDuration));
	addInfo(QString("drop: %1 %2").arg(_dropDetected).arg(_dropTimer.elapsed() / 1000));

	QTime t;
	t.start();
	updateGL();
	_lastUpdateDuration = t.elapsed();
	if(_lastUpdateDuration > _maxUpdateDuration)
		_maxUpdateDuration = _lastUpdateDuration;
	if(_lastUpdateDuration > static_cast<int>(1500.0 / _screenFrequency)) {
		_dropTimer.restart();
		_dropDetected++;
		PHDBG(8) << "Drop detected:" << _dropDetected;
	}
}

void PhGraphicView::paintGL()
{
	//PHDEBUG << "PhGraphicView::paintGL" ;

	// Update the clock time taking into account the time elapsed since the last paint event
	// In the particular case of V-sync enabled and no dropped frames, this is equivalent
	// to using the screen refresh rate to compute the elapsed time.
	// If V-sync is not enabled, using the screen refresh rate is plain wrong
	// (the refreshTimer period would be ok though).
	// If frames are dropped, it is mandatory to refer to a timer to be correct.
	// We rasterized the elapsed value to the theorical value
	// (based on the screen frequency) in order to keep a
	// constant value in most of the case.
	// Ex: given a 60Hz screen refresh rate
	// - 400 ticks / 16.6ms in normal case
	// - 0 ticks / 0ms if VSYNC is not enabled and the timer
	// - a multiple of 400 ticks / 16.6ms if frame were dropped
	qint64 nsecsElapsed = _timer.nsecsElapsed();
	PhTime trueElapsedTime = (nsecsElapsed - _previousNsecsElapsed) * 24000 / 1000000000;
	PhTime expectedElapsedTime = 24000 / _screenFrequency;
	int numberOfScreenRefresh = (trueElapsedTime + expectedElapsedTime / 2) / expectedElapsedTime;
	PhTime rasterizedElapsedTime =  numberOfScreenRefresh * expectedElapsedTime;

	if(rasterizedElapsedTime > 0)
		_previousNsecsElapsed = nsecsElapsed;

	emit beforePaint(rasterizedElapsedTime);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QTime timer;
	timer.start();

	int ratio = this->windowHandle()->devicePixelRatio();
	emit paint(this->width() * ratio, this->height() * ratio);

	if(timer.elapsed() > _maxPaintDuration)
		_maxPaintDuration = timer.elapsed();
	addInfo(QString("draw: %1 %2").arg(_maxPaintDuration).arg(timer.elapsed()));
	if(_settings) {
		if(_settings->resetInfo()) {
			_dropDetected = 0;
			_maxRefreshRate = 0;
			_maxPaintDuration = 0;
			_maxUpdateDuration = 0;
		}
		if(_settings->displayInfo()) {
			_infoFont.setFamily(_settings->infoFontFamily());
			int y = 0;
			foreach(QString info, _infos) {
				PhGraphicText gInfo(&_infoFont, info, 0, y);
				gInfo.setSize(_infoFont.getNominalWidth(info) / 2, 50);
				gInfo.setZ(10);
				gInfo.setColor(Qt::red);
				gInfo.draw();
				y += gInfo.height();
			}
		}
	}
	// Once the informations have been displayed
	// clear it
	_infos.clear();

	_frameTickCounter.tick();
}
