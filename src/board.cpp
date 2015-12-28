// board.cpp -- Contains the board class that handles game events

#include <SDL2/SDL.h>
#include "board.h"
#include "graphics.h"
#include "utilities.h"

/*
 * Public member functions
 */

Board::Board(Graphics* graphics)
{
		// initialize private data members
	m_creationSuccess = true;
	m_renderer = graphics->getRenderer();
	m_gameStatus = MENU_SCREEN;
	m_turn = O_TURN;

		// set board to all blank
	clearBoard();

		// get all the SDL_Rect objects we need
	generateLineRects();
	generateIconRects();

		// load media
	if (!loadMedia()) {
		printf("Unable to load media.\n");
		m_creationSuccess = false;
	}
}

Board::~Board()
{
		// destroy textures
	SDL_DestroyTexture(m_menuScreen);
	SDL_DestroyTexture(m_gameScreen);
	for (int i = 0; i < TOTAL_ICON_TEXTURES; i++)
		SDL_DestroyTexture(m_iconTextures[i]);
}

void Board::drawBoard()
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);

	if (m_gameStatus == MENU_SCREEN) {
		SDL_RenderCopy(m_renderer, m_menuScreen, nullptr, nullptr);
	}
	else if (m_gameStatus == GAME_SCREEN) {
		SDL_RenderCopy(m_renderer, m_gameScreen, nullptr, nullptr);

			// draw the lines on the board
		drawLines();
		drawIcons();
	}

	SDL_RenderPresent(m_renderer);
}

void Board::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.scancode)
		{
			case SDL_SCANCODE_T:
				if (m_gameStatus == MENU_SCREEN)
					m_gameStatus = GAME_SCREEN;
				break;
			default:
				break;
		}
	}
}

bool Board::wasCreationSuccessful()
{
	return m_creationSuccess;
}

/*
 * Private member functions
 */

void Board::drawLines()
{
	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
	for (int i = 0; i < TOTAL_LINE_RECTS; i++) {
		SDL_RenderFillRect(m_renderer, &m_lineRects[i]);
	}
}

void Board::drawIcons()
{
	for (int r = 0; r < globals::ROWS; r++) {
		for (int c = 0; c < globals::COLS; c++) {
			SDL_Texture* iconTexture = nullptr;
			if (m_iconStates[r][c] == BLANK_STATE)
				continue;
			else if (m_iconStates[r][c] == O_STATE)
				iconTexture = m_iconTextures[O_TEXTURE];
			else if (m_iconStates[r][c] == X_STATE)
				iconTexture = m_iconTextures[X_TEXTURE];
			// copy to render
			if (iconTexture != nullptr) {
				SDL_RenderCopy(m_renderer, iconTexture, nullptr, &m_iconRects[r][c]);
			}
		}
	}
}

void Board::fillSquare(int r, int c, IconState icon)
{
	m_iconStates[r][c] = icon;
}

void Board::clearBoard()
{
	for (int i = 0; i < globals::ROWS; i++) {
		for (int j = 0; j < globals::COLS; j++) {
			m_iconStates[i][j] = BLANK_STATE;
		}
	}
}

void Board::generateLineRects()
{
		// everything with reference to lines
	using namespace line;
		// vertical lines
	m_lineRects[FIRST_VERT] = { FIRST_VERT_X, VERT_Y, LINE_WIDTH, LINE_LENGTH };
	m_lineRects[SECOND_VERT] = { SECOND_VERT_X, VERT_Y, LINE_WIDTH, LINE_LENGTH };
		// horizontal lines (note: "width" of the *horizontal* lines is same as the length of the lines)
	m_lineRects[FIRST_HORIZ] = { HORIZ_X, FIRST_HORIZ_Y, LINE_LENGTH, LINE_WIDTH };
	m_lineRects[SECOND_HORIZ] = { HORIZ_X, SECOND_HORIZ_Y, LINE_LENGTH, LINE_WIDTH };
}

void Board::generateIconRects()
{
		// need some stuff from line namespace defined in board.h
	using namespace line;
	using namespace globals;

	int x, y;
	for (int r = 0; r < globals::ROWS; r++) {
		for (int c = 0; c < globals::ROWS; c++) {
			x = X_PADDING + BOX_LENGTH * c + LINE_WIDTH * c;
			y = MENU_Y_OFFSET + Y_PADDING + BOX_LENGTH * r + LINE_WIDTH * r;
			m_iconRects[r][c] = { x, y, BOX_LENGTH, BOX_LENGTH };
		}
	}
}

bool Board::loadMedia()
{
	// initialize textures to nullptr
	m_menuScreen = nullptr;
	m_gameScreen = nullptr;
	for (int i = 0; i < TOTAL_ICON_TEXTURES; i++) {
		m_iconTextures[i] = nullptr;
	}

	// menu and game screen
	m_menuScreen = loadImage("../content/menuscreen.png", m_renderer);
	m_gameScreen = loadImage("../content/gamescreen.png", m_renderer);
	// icons
	m_iconTextures[O_TEXTURE] = loadImage("../content/o.png", m_renderer);
	m_iconTextures[X_TEXTURE] = loadImage("../content/x.png", m_renderer);

	if (m_menuScreen == nullptr || m_gameScreen == nullptr ||
		m_iconTextures[O_TEXTURE] == nullptr || m_iconTextures[X_TEXTURE] == nullptr)
	{
		return false;
	}
	else {
		return true;
	}
}

