#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <SDL2/SDL_image.h>

namespace globals {
	const char GAME_TITLE[] = "Tic-Tac-Toe";
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 640;
	const int SDL_IMAGE_FLAGS = IMG_INIT_PNG;

		// constants for drawing the board
	const int X_PADDING = 80;
	const int Y_PADDING = 20;
	const int MENU_Y_OFFSET = 120;

	const int ROWS = 3;
	const int COLS = 3;
}

#endif