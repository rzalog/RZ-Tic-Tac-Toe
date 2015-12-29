// board.cpp -- Contains the board class that handles game events

#include <SDL2/SDL.h>
#include <cstring>
#include "board.h"
#include "graphics.h"
#include "utilities.h"

/*
 * Public member functions
 */

Board::Board(Graphics* graphics)
{
	m_renderer = graphics->getRenderer();
		// initEmptyBoardialize data members for an empty board
	initEmptyBoard();
	m_gameStatus = MENU;

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
	for (int i = 0; i < TOTAL_SCREENS; i++)
		SDL_DestroyTexture(m_screenTextures[i]);
	for (int i = 0; i < TOTAL_ICON_TEXTURES; i++)
		SDL_DestroyTexture(m_iconTextures[i]);
}

void Board::drawBoard()
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);

	if (m_gameStatus == MENU) {
		SDL_RenderCopy(m_renderer, m_screenTextures[MENU_SCREEN], nullptr, nullptr);
	}
	else if (m_gameStatus == BOARD) {
		SDL_RenderCopy(m_renderer, m_screenTextures[GAME_SCREEN], nullptr, nullptr);

			// draw the lines on the board
		drawLines();
		drawIcons();

		if (isGameFinished()) {
			m_gameStatus = GAME_FINISHED;
		}
	}
	
	else if (m_gameStatus == GAME_FINISHED) {
			// flash the screen
		bool flash = false;
		for (int i = 0; i < 10; i++) {
				// change the render draw color
			if (flash) {
					// teal-ish color
				SDL_SetRenderDrawColor(m_renderer, 0x4E, 0xEA, 0xF2, 0xFF);
				SDL_RenderClear(m_renderer);
				flash = false;
			}
			else {
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(m_renderer);
				flash = true;
			}
				
				// draw the rest of the board
			drawLines();
			drawIcons();
				// push the renderer to the screen
			SDL_RenderPresent(m_renderer);
				// wait half a second
			SDL_Delay(500);
		}

			// find out who won, render the respective screen
		if (m_winner == O_STATE) {
			SDL_RenderCopy(m_renderer, m_screenTextures[O_WIN_SCREEN], nullptr, nullptr);
		}
		else if (m_winner == X_STATE) {
			SDL_RenderCopy(m_renderer, m_screenTextures[X_WIN_SCREEN], nullptr, nullptr);
		}
			// if neither side won
		else if (m_winner == BLANK_STATE) {
				// note: BLANK_WIN_SCREEN is when neither player wins
			SDL_RenderCopy(m_renderer, m_screenTextures[BLANK_WIN_SCREEN], nullptr, nullptr);
		}
			// update the renderer
		SDL_RenderPresent(m_renderer);
			// let the user look at the win screen
		SDL_Delay(1000);

			// initialize empty board
		initEmptyBoard();
		m_gameStatus = BOARD;
	}

	SDL_RenderPresent(m_renderer);
}

void Board::handleEvent(const SDL_Event& event)
{
	// key was pressed
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.scancode)
		{
			case SDL_SCANCODE_T:
				if (m_gameStatus == MENU)
					m_gameStatus = BOARD;
				break;
			case SDL_SCANCODE_R:
				if (m_gameStatus == BOARD)
					initEmptyBoard();

			default:
				break;
		}
	}

	// mouse was clicked
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
			// if user is on the game board, then we want to check if they clicked an icon
		if (m_gameStatus == BOARD) {
				// check if an icon was clicked
			int mousex = event.button.x;
			int mousey = event.button.y;

			for (int r = 0; r < globals::ROWS; r++) {
				for (int c = 0; c < globals::COLS; c++) {
					if (m_iconStates[r][c] == BLANK_STATE && isPointInRect(mousex, mousey, m_iconRects[r][c])) {
						m_iconStates[r][c] = m_turn;
						m_turn = (m_turn == O_STATE) ? X_STATE : O_STATE;
					}
				}
			}
		}
	}
}

bool Board::wasCreationSuccessful()
{
	return m_creationSuccess;
}

///////////////////////////////////////////////////////////
// Private member functions
///////////////////////////////////////////////////////////

void Board::initEmptyBoard()
{
		// initialize private data members
	m_creationSuccess = true;
	m_turn = O_STATE;
	m_winner = BLANK_STATE;

		// set board to all blank
	clearBoard();
}

bool Board::isGameFinished()
{
	bool finished = false;
	
		// if one player got 3 in a row, the game is finished.
	if (hasPlayerWon()) {
		finished = true;
	}

		// if no blank space, game is finished.
	if (!containsBlankSpace()) {
		finished = true;
	}

	return finished;
}

bool Board::hasPlayerWon()
{
	// FIX THIS SHIT

	int r;
	int c;

		// check vertical matches, row is locked
	r = 2;
	for (c = 0; c < globals::COLS; c++) {
		if ( (m_iconStates[r][c] == m_iconStates[r-1][c]) && (m_iconStates[r][c] == m_iconStates[r-2][c]) ) {
			m_winner = m_iconStates[r][c];
			if (m_winner != BLANK_STATE) break;
			break;
		}
	}

	if (m_winner == BLANK_STATE) {
			// check horizontal matches, column is locked
		c = 2;
		for (r = 0; r < globals::ROWS; r++) {
			if ( (m_iconStates[r][c] == m_iconStates[r][c-1]) && (m_iconStates[r][c] == m_iconStates[r][c-2]) ){
				m_winner = m_iconStates[r][c];
				if (m_winner != BLANK_STATE) break;
			}
		}
	}

	if (m_winner == BLANK_STATE) {
			// check diagonol matches
		if ( ( m_iconStates[0][0] == m_iconStates[1][1] && m_iconStates[0][0] == m_iconStates[2][2] ) ||
			( m_iconStates[0][2] == m_iconStates[1][1] && m_iconStates[0][2] == m_iconStates[2][0] ) )
		{
				// if a diagonol one, has to be the middle icon
			m_winner = m_iconStates[1][1];
		}
	}

		// if winner is O or X (aka not BLANK_STATE), then the user has won
	return (m_winner != BLANK_STATE);
}

bool Board::containsBlankSpace()
{
	bool success = false;

	for (int r = 0; r < globals::ROWS; r++)
		for (int c = 0; c < globals::COLS; c++)
			if (m_iconStates[r][c] == BLANK_STATE)
				success = true;

	return success;
}

/*
 * Graphics functions
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
	for (int i = 0; i < TOTAL_SCREENS; i++)
		m_screenTextures[i] = nullptr;
	for (int i = 0; i < TOTAL_ICON_TEXTURES; i++) {
		m_iconTextures[i] = nullptr;
	}

		// for CONTENT_PATH
	using namespace globals;
		// game screens
	m_screenTextures[MENU_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "menu_screen.png");
	m_screenTextures[GAME_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "game_screen.png");
	m_screenTextures[O_WIN_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "o_win.png");
	m_screenTextures[X_WIN_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "x_win.png");
		// for when neither player wins
	m_screenTextures[BLANK_WIN_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "blank_win.png");
	m_screenTextures[RESTART_SCREEN] = loadImage(m_renderer, CONTENT_PATH + "restart.png");
		// icons
	m_iconTextures[O_TEXTURE] = loadImage(m_renderer, CONTENT_PATH + "o_icon.png");
	m_iconTextures[X_TEXTURE] = loadImage(m_renderer, CONTENT_PATH + "x_icon.png");

	bool success = true;
	for (int i = 0; i < TOTAL_SCREENS; i++)
		if (m_screenTextures[i] == nullptr)
			success = false;
	for (int i = 0; i < TOTAL_ICON_TEXTURES; i++)
		if (m_iconTextures[i] == nullptr)
			success = false;
	return success;
}

