#include <QtGlobal>

#include <SDL2/SDL.h>
#if defined(Q_OS_MAC)
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <string>
#include <cstring>
#include "string.h"
#include "iostream"

#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QTime>

using namespace std;

#undef main

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	int numdrivers = SDL_GetNumRenderDrivers();
	printf("Drivers count: %i\n", numdrivers);
	for (int i = 0; i < numdrivers; i++) {
		SDL_RendererInfo drinfo;
		SDL_GetRenderDriverInfo(i, &drinfo);

		printf("Driver name: %s\n", drinfo.name);
	}

	qDebug() << "Initialize all SDL subsystems";
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
		qDebug() << "Error:" << SDL_GetError();
		return false;
	}

	qDebug() << "Set up the window";
	int screenWidth = 1280;
	int screenHeight = 600;
	SDL_Window * window = SDL_CreateWindow("SDLTest", 100, 100, screenWidth, screenHeight, SDL_SWSURFACE);

	SDL_Surface * screen = SDL_GetWindowSurface(window);

	SDL_Rect rect = {600, 20, 200, 300};
	SDL_FillRect(screen, &rect, 0xff0000ff);

	QString ressourcesPath = QApplication::applicationDirPath();
	// Create a surface from picture:
	QString lookPath = ressourcesPath + "/look.png";

	QFile file(lookPath);
	qDebug() << "exists" << file.exists();
	SDL_Surface *image = IMG_Load( lookPath.toStdString().c_str());

	if ( image == NULL ) {
		qDebug() << "failed to load " << lookPath;
		qDebug() << SDL_GetError();
		return 1;
	}

	// Display the picture:
	SDL_Rect imageRect = {0, 0, image->w, image->h};

	SDL_BlitSurface( image, NULL, screen, &imageRect );

	// Initialize TTF :
	if( TTF_Init() == -1 ) {
		qDebug() << "TTF error.";
		return 2;
	}

	//Create a font
	QString fontPath = ressourcesPath + "/SWENSON.TTF";
	TTF_Font *font = TTF_OpenFont(fontPath.toStdString().c_str(), 100 );
	qDebug() << "Outline :" << TTF_GetFontOutline(font);

	if (font == NULL) {
		qDebug() << "Error opening " << fontPath;
		qDebug() << SDL_GetError();
		return 3;
	}

	//Font's color (black)
	SDL_Color color = {0, 0, 0, 255};
	Uint16 ch;
#define TEST  2

#if TEST == 0
	// Displaying a simple string:
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "ABCDEFGHIJKL", color );
	SDL_Rect textRect = {0, 100, surface->w, surface->h};
	SDL_BlitSurface( surface, NULL, screen, &textRect);

#elif TEST == 2
	// used to set the base surface
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;



	// Creation of the glyph surface
	SDL_Surface * glyphMatrix = SDL_CreateRGBSurface(0, 2048, 2048, 32, rmask, gmask, bmask, amask);


	// THIS IS ONLY FOR VISUAL PURPOSE, TOTALY USELESS ON A REAL SITUATION
	Uint32 bgColor;
	bgColor = 0xffffffff;
	qDebug() << SDL_FillRect(screen, NULL, bgColor);

	// store the width of each glyph
	int glyphWidth[256];
	int glyphAdvance[256];

	// Space between glyph
	int space = 128;
	int glyphHeight = 0;

	// Nb pass for bold setting
	int nbPass = 2;
	for(int i = 0; i <= nbPass; i++) {
		TTF_SetFontOutline(font, i);
		for(ch = 32; ch < 256; ++ch) {
			if(TTF_GlyphIsProvided(font, ch)) {
				int minx, maxx, miny, maxy, advance;
				TTF_GlyphMetrics(font, ch, &minx, &maxx, &miny, &maxy, &advance);
				//qDebug() << ch << (char) ch << minx << maxx << miny << maxy << advance;
				if(advance != 0) {
					// Temporary surface of the character
					SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(font, ch, color);
					if (!glyphSurface) {

						qDebug() << SDL_GetError();
						qDebug() << TTF_GetError();
					}
					SDL_Rect glyphRect;
					glyphRect.x = (ch % 16) * space;
					glyphRect.y = (ch / 16) * space;
					glyphRect.w = glyphSurface->w;
					glyphRect.h = glyphSurface->h;
					if(glyphRect.h > glyphHeight)
						glyphHeight = glyphRect.h;
					if(SDL_BlitSurface( glyphSurface, NULL, glyphMatrix, &glyphRect ))
						qDebug() << SDL_GetError();

					// Store information about the glyph
					glyphAdvance[ch] = advance;
					glyphWidth[ch] = maxx - minx;
				}
				else
					qDebug() <<" Error with : " << ch << (char) ch << minx << maxx << miny << maxy << advance;
			}
			else {
				glyphAdvance[ch] = 0;
				glyphWidth[ch] = 0;
			}
		}
	}

	//This draw the entire glyph
	SDL_Rect glyphMatrixRect;
	glyphMatrixRect.x = 0;
	glyphMatrixRect.y = -1000;
	glyphMatrixRect.w = 400;
	glyphMatrixRect.h = 400;
	SDL_BlitSurface(glyphMatrix, NULL, screen, &glyphMatrixRect);

	int x = 50;
	int y = 100;
	QString s = "Martin et ses chaussettes propres";

	// Display a string
	for(int i = 0; i < s.length(); i++) {
		int ch = (int)s.at(i).toLatin1();
		if(glyphWidth[ch] > 0) {
			// Compute glyph rect in the matrix
			SDL_Rect glyphRect;
			glyphRect.x = (ch % 16) * space;
			glyphRect.y = (ch / 16) * space;
			//glyphRect.w = glyphWidth[ch];
			glyphRect.w = space;
			glyphRect.h = glyphHeight;


			// Display the glyph on the screen
			SDL_Rect draw = {x, y, 500, 500};
			SDL_BlitSurface(glyphMatrix, &glyphRect, screen, &draw);

		}
		// Shift the offset
		x += glyphAdvance[ch];
	}



	//qDebug() << t.elapsed();

#endif

	SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(font, 339, color);
	SDL_BlitSurface(glyphSurface, NULL, screen, NULL);


	//Update the screen
	if( SDL_UpdateWindowSurface(window)) {
		qDebug() << "failed to update";
		return -1;
	}

	bool quit = false;

	//While the user hasn't quit
	while(quit == false) {
		SDL_Event event;
		//While there's an event to handle
		while( SDL_PollEvent(&event) ) {
			//If the user has Xed out the window
			if( event.type == SDL_QUIT ) { //Quit the program
				quit = true;
			}
		}
	}

//	//Free the surface and quit SDL
	SDL_FreeSurface( image );
//	SDL_FreeSurface(message);
	SDL_FreeSurface(screen);

//	//Quit SDL
	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();

	return 0;
}


