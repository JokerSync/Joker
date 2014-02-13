/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICVIEW_H
#define PHGRAPHICVIEW_H

#include <QGLWidget>
#include <QTimer>
#include <QSettings>

#include "PhTools/PhTickCounter.h"

/**
 * The PhGraphicView class is a canvas to create your custom graphic view.
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
	~PhGraphicView();

	/**
	 * Handle the resizing of the view.
	 *
	 * @param width
	 * @param height
	 */
	void resizeGL(int width, int height);

	/*!
	 * \brief Get the refresh rate of the view
	 * \return The rate (in fps)
	 */
	int refreshRate() { return _frameTickCounter.frequency();}

signals:
	/*!
	 * \brief emit a signal just before the paint
	 * \param frequency the effective frequency
	 */
	void beforePaint(int frequency);

protected:

	/*!
	 * \brief Set the settings
	 * \param settings the desired settings
	 */
	void setSettings(QSettings *settings);
	/*!
	 * \brief The member containing the settings
	 */
	QSettings * _settings;

	/*!
	 * \brief initialization, every class have to re-implement it.
	 * \return true if everything went well, false otherwise.
	 */
	virtual bool init() = 0;
	/*!
	 * \brief paint event, every class have to re-implement it.
	 * \return true if everything went well, false otherwise.
	 */
	virtual void paint() = 0;

	/*!
	 * \brief initializeGL
	 * This virtual function is called once before the first call to paintGL() or resizeGL(),
	 * and then once whenever the widget has been assigned a new QGLContext.
	 * Reimplement it in a subclass.
	 * This function should set up any required OpenGL context rendering flags,
	 * defining display lists, etc.
	 *
	 * It calls init()
	 */
	void initializeGL();
	/*!
	 * \brief paintGL
	 * This virtual function is called whenever the widget needs to be painted.
	 * Reimplement it in a subclass.
	 */
	void paintGL();

private slots:
	void onRefresh();

private:

    /**
     * @brief t_Timer
     * used to draw
     */
    QTimer *t_Timer;
	PhTickCounter _frameTickCounter;
};

#endif // PHGRAPHICVIEW
