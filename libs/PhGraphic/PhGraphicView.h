/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICVIEW_H
#define PHGRAPHICVIEW_H

#include <QQuickWidget>
//#include <QQuickView>
//#include <QQmlApplicationEngine>
#include "PhSync/PhTime.h"
#include "PhTools/PhTickCounter.h"

#include "PhGraphicSettings.h"
#include "PhFont.h"

/**
 * @brief The PhGraphicView class is a canvas to create your custom graphic view.
 *
 * Create your PhGraphicView child, initialize your graphic object with init(),
 * paint them with paint(), and clear them with dispose().
 * These methods are called automatically after the view creation and during all
 * its lifetime.
 */
class PhGraphicView : public QQuickWidget
//class PhGraphicView : public QQuickView
//class PhGraphicView : public QQmlApplicationEngine
{
	Q_OBJECT
public:
	/**
	 * PhGraphicView constructor.
	 * @param parent Parent object.
	 */
	explicit PhGraphicView(QWidget *parent = 0);

	/**
	 * @brief PhGraphicView constructor specifying the client area size.
	 * @param width The client area width
	 * @param height The client area height
	 * @param parent Parent object
	 *
	 * The size is expressed in true pixel size. In the case of the retina screen,
	 * the size will correspond to the opengl viewport: 2 times the widget size.
	 */
	PhGraphicView (int width, int height, QWidget *parent = 0);

	~PhGraphicView();

	/**
	 * @brief Get the refresh rate of the view
	 * @return The rate (in fps)
	 */
	int refreshRate() {
		return _frameTickCounter.frequency();
	}

	/**
	 * @brief Pass the settings to the graphic view
	 * @param settings The settings
	 */
	void setGraphicSettings(PhGraphicSettings *settings);
	/**
	 * @brief measured max refresh rate
	 * @return measured max refresh rate
	 */
	int maxRefreshRate();
	/**
	 * @brief measured max update duration
	 * @return measured max update duration
	 */
	int maxUpdateDuration();
	/**
	 * @brief measured last update duration
	 * @return measured last update duration
	 */
	int lastUpdateDuration();
	/**
	 * @brief number of drops detected since the start of the application
	 * @return number of drops detected
	 */
	int dropDetected();
	/**
	 * @brief number of seconds elapsed since last drop detected
	 * @return number of seconds
	 */
	int secondsSinceLastDrop();
signals:
	/**
	 * @brief emit a signal just before the paint
	 * @param elapsedTime the time elapsed since the last paint event
	 */
	void beforePaint(PhTime elapsedTime);

	/**
	 * @brief paint event, every class have to re-implement it.
	 * @param width Width of the paint area
	 * @param height Height of the paint area
	 */
	void paint(int width, int height);
protected:

	/**
	 * @brief The screen frequency
	 */
	qreal _screenFrequency;

private slots:
	void onRefresh();

private:
	PhGraphicSettings *_settings;
	/**
	 * @brief t_Timer
	 * used to draw
	 */
	QTimer *_refreshTimer;
	PhTickCounter _frameTickCounter;
	QTime _dropTimer;
	int _dropDetected, _maxRefreshRate, _maxPaintDuration, _lastUpdateDuration, _maxUpdateDuration;
	QElapsedTimer _timer;
	qint64 _previousNsecsElapsed;
};

#endif // PHGRAPHICVIEW
