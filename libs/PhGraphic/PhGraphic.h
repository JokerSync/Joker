/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QtGlobal>
#include <QColor>
#include <QSize>
#include <QGLWidget>

/* QtGui provides OpenGL definitions */
#include <QtGui>

#include <SDL2/SDL.h>

#if defined(Q_OS_MAC)
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#if defined(Q_OS_WIN)
#include <GL/glu.h>
#else
#include <glu.h>
#endif

