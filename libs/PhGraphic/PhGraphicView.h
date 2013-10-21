#ifndef PHGRAPHICVIEW_H
#define PHGRAPHICVIEW_H

#include <QGLWidget>
#include <QTimer>

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

protected:


	virtual bool init() = 0;

	virtual void paint() = 0;

	void initializeGL();

	void paintGL();

private slots:
	void onRefresh();

private:

    /**
     * @brief t_Timer
     * used to draw
     */
    QTimer *t_Timer;
};

#endif // PHGRAPHICVIEW
