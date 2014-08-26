/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhQmlView.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLContext>

#include "PhTools/PhDebug.h"

PhQmlView::PhQmlView()
	: _openGLFunctions(NULL)
{
	connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void PhQmlView::onRefresh()
{
	if (window())
		window()->update();
}

void PhQmlView::handleWindowChanged(QQuickWindow *win)
{
	if (win) {
		connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
		connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);

		// If we allow QML to do the clearing, it would clear what we paint
		// and nothing would show.
		win->setClearBeforeRendering(false);

		connect(win, SIGNAL(beforeRendering()), this, SLOT(onPaint()), Qt::DirectConnection);
	}
}

void PhQmlView::cleanup()
{
}

void PhQmlView::sync()
{
	if (window())
		window()->update();
}

void PhQmlView::onPaint()
{
//	if (_openGLFunctions == NULL) {
//		if (QOpenGLContext::currentContext()) {
//			_openGLFunctions = new QOpenGLFunctions(QOpenGLContext::currentContext());
//			_openGLFunctions->initializeOpenGLFunctions();
//		}
//	}

//	if (_openGLFunctions != NULL) {
//		// Enable the fixed-pipeline functionnality in OpenGL.
//		// By default it is disabled because Qt priority is to target OpenGL _ES_ where fixed pipeline does not exist.
//		_openGLFunctions->glUseProgram(0);
//	} else {
//		return;
//	}

//	glViewport(0, 0, window()->width(), window()->height());
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(0, window()->width(), window()->height(), 0, -10, 10);
//	glMatrixMode(GL_MODELVIEW);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glLoadIdentity();

	// http://stackoverflow.com/questions/21870004/strange-bugs-while-rendering-with-opengl-under-qml-scene
	//glDepthMask(GL_TRUE);

//	glClearColor(0, 0, 0, 1);
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int ratio = window()->devicePixelRatio();
	emit paint(window()->width() * ratio, window()->height() * ratio);

	// try to restore the state to where it was before we touched it
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
}
