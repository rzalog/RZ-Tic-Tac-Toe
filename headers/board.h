//////////////////////////////
// Inlcude guards
//////////////////////////////

#ifndef __BOARD_H__
#define __BOARD_H__

//////////////////////////////
// Forward declarations
//////////////////////////////

class Graphics;

//////////////////////////////
// Includes
//////////////////////////////

#include <SDL2/SDL.h>
#include <string>
#include "globals.h"

// constants for creating the lines on the board
namespace line {
	const int LINE_WIDTH = 8;
	const int LINE_LENGTH = globals::SCREEN_HEIGHT - 2 * globals::Y_PADDING - globals::MENU_Y_OFFSET;
	const int VERT_Y = globals::MENU_Y_OFFSET + globals::Y_PADDING;	// for vertical lines, y-coord is const
	const int HORIZ_X = globals::X_PADDING;					// for horizontal lines, x-coord is const

	const int BOX_LENGTH = (LINE_LENGTH - 2 * LINE_WIDTH) / 3;

	const int FIRST_VERT_X = globals::X_PADDING + BOX_LENGTH;
	const int SECOND_VERT_X = FIRST_VERT_X + BOX_LENGTH + LINE_WIDTH;
		// same as the other two lines, just rotated
	const int FIRST_HORIZ_Y = globals::MENU_Y_OFFSET + globals::Y_PADDING + BOX_LENGTH;
	const int SECOND_HORIZ_Y = FIRST_HORIZ_Y + BOX_LENGTH + LINE_WIDTH;
}

enum LineRects {
	FIRST_VERT,
	SECOND_VERT,
	FIRST_HORIZ,
	SECOND_HORIZ,
	TOTAL_LINE_RECTS
};

enum IconRects {
	TOP_LEFT, TOP_MID, TOP_RIGHT,
	MID_LEFT, MID_MID, MID_RIGHT,
	BOT_LEFT, BOT_MID, BOT_RIGHT,
	TOTAL_ICON_RECTS
};

// to keep track of which icons contain what, and keep track of the turn
enum IconState {
	O_STATE,
	X_STATE,
	BLANK_STATE
};

// for our icon array
enum IconTextureIndex {
	O_TEXTURE,
	X_TEXTURE,
	TOTAL_ICON_TEXTURES
};

// keep track of the game state
enum GameStatus {
	MENU,
	BOARD,
	GAME_FINISHED,
	ASK_RESTART,
	TOTAL_STATES
};

// keep track of screen textures
enum ScreenTextureIndex {
	MENU_SCREEN,
	GAME_SCREEN,
	O_WIN_SCREEN,
	X_WIN_SCREEN,
	BLANK_WIN_SCREEN,	// "win screen" for when neither player wins
	RESTART_SCREEN,
	TOTAL_SCREENS
};

// keep track of who wins
enum Player {
	PLAYER_O,
	PLAYER_X
};


class Board {
public:
		// constructor, initializes member variables
	Board(Graphics* graphics);
		// Destructor, frees resources
	~Board();

		// draw the board, which starts out blank
	void drawBoard();

		// handle an SDL_Event
	void handleEvent(const SDL_Event& event);

		// if creation was successful
	bool wasCreationSuccessful();

private:
		// initializes an empty board
	void initEmptyBoard();
		// does things when the game is over
	void gameOver();
		// checks if game is finished
	bool isGameFinished();
		// checks if 3 in a row on the board
			// if there is, updates the winner, returns true
			// otherwise, returns false
	bool hasPlayerWon();
		// checks if board contains blank space
	bool containsBlankSpace();

	/* Graphics functions */

		// draws the lines on the board
	void drawLines();
	void drawIcons();

		// clears board by setting each icon to BLANK_STATE
	void clearBoard();

		// fills m_iconRects with appropriate SDL_Rect's to put icons into
	void generateLineRects();
	void generateIconRects();

		// loads in media
	bool loadMedia();

	/* Data members */
		// SDL textures
	SDL_Texture* m_screenTextures[TOTAL_SCREENS];
	SDL_Texture* m_iconTextures[TOTAL_ICON_TEXTURES];

		// all the SDL_Rect's on the board
	SDL_Rect m_iconRects[globals::ROWS][globals::COLS];
		// the lines to draw the board
	SDL_Rect m_lineRects[TOTAL_LINE_RECTS];
		// which icons contain which symbols
	IconState m_iconStates[globals::ROWS][globals::COLS];
		// whose turn it is, circle or square
	IconState m_turn;
		// game status, menu or actually playing
	GameStatus m_gameStatus;
		// which player has won
	IconState m_winner;

		// successful creation flag
	bool m_creationSuccess;

		// the window's renderer
	SDL_Renderer* m_renderer;
};

#endif