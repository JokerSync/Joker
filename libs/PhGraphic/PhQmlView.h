/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHQMLGRAPHICVIEW_H
#define PHQMLGRAPHICVIEW_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLFunctions>


class PhQmlView : public QQuickItem
{
	Q_OBJECT

public:
	PhQmlView();

signals:
	/**
	 * @brief paint event, every class have to re-implement it.
	 * @param width Width of the paint area
	 * @param height Height of the paint area
	 */
	void paint(int width, int height);

public slots:
	void sync();
	void cleanup();
	void onPaint();
	void onRefresh();

private slots:
	void handleWindowChanged(QQuickWindow *win);

private:
	QOpenGLFunctions *_openGLFunctions;
};

#endif // PHQMLGRAPHICVIEW_H
