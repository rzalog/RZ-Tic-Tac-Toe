/////////////////
// Include guards
/////////////////

#ifndef __GAME_H__
#define __GAME_H__

///////////
// Includes
///////////

///////////////////////
// Forward Declarations
///////////////////////

class Graphics;
class Board;

/////////////////////
// Game class
// ---
// Runs the game
/////////////////////

class Game {
public:
	// initializes SDL and creates a window, and runs the game loop
	Game();
	// quits SDL, frees resources
	~Game();
private:
	// initializes SDL, creates window and renderer
	bool init();
	// frees resources, closes SDL
	void close(); 

	// runs the main game loop
	void gameLoop();
	// draws the game screen after every frame
	void draw();

	// graphics object
	Graphics* m_graphics;
	// board object
	Board* m_board;
};

#endif