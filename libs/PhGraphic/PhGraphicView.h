/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICVIEW_H
#define PHGRAPHICVIEW_H

#include <QGLWidget>
#include <QTimer>

#include "PhTools/PhTime.h"
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
class PhGraphicView : public QGLWidget
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
	 * Handle the resizing of the view.
	 *
	 * @param width
	 * @param height
	 */
	void resizeGL(int width, int height);

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
	 * @brief Add a line to the debug info
	 * @param info A string
	 */
	void addInfo(QString info);
signals:
	/**
	 * @brief emit a signal just before the paint
	 * @param frequency the effective frequency
	 */
	void beforePaint(PhTimeScale frequency);

	/**
	 * @brief paint event, every class have to re-implement it.
	 * @param width Width of the paint area
	 * @param height Height of the paint area
	 */
	void paint(int width, int height);
protected:
	/**
	 * @brief paintGL
	 * This virtual function is called whenever the widget needs to be painted.
	 * Reimplement it in a subclass.
	 */
	void paintGL();

	/**
	 * @brief The screen frequency
	 */
	int _screenFrequency;

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
	QStringList _infos;
	PhFont _infoFont;
	QTime _dropTimer;
	int _dropDetected, _maxRefreshRate, _maxPaintDuration, _lastUpdateDuration, _maxUpdateDuration;
};

#endif // PHGRAPHICVIEW
