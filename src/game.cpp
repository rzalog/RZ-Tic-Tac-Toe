////////////////
// Includes
////////////////

#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "globals.h"
#include "graphics.h"
#include "board.h"

///////////////////
// Constructor and destructor
///////////////////

Game::Game()
{
	if (!init()) {
		printf("Error initializing. Terminating program.\n");
	} else {
		gameLoop();
	}
}

Game::~Game()
{
	close();

	delete m_graphics;
	delete m_board;
}

//////////////////////////////
// Public member functions
//////////////////////////////



//////////////////////////////
// Private member functions
//////////////////////////////

bool Game::init()
{
	// success flag
	bool success = true;

	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized. SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// initialize SDL_image
		if (!IMG_Init(globals::SDL_IMAGE_FLAGS)) {
			printf("SDL_image could not be initialized. SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}
		else {
			// create Graphics object
			m_graphics = new Graphics(globals::GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				globals::SCREEN_HEIGHT, globals::SCREEN_WIDTH);
			if (!m_graphics->wasCreationSuccessful()) {
				printf("Error in creating Grahpics object.\n");
				success = false;
			}
			else {
				// create Board object
				m_board = new Board(m_graphics);
				if (!m_board->wasCreationSuccessful()) {
					printf("Error in creating Board object.\n");
					success = false;
				}
			}
		}
	}

	return success;
}

void Game::close()
{
	IMG_Quit();
	SDL_Quit();
}

void Game::gameLoop()
{
	// main loop flag
	bool quit = false;
	// event handler
	SDL_Event event;

	// main loop
	while (!quit)
	{
		// event handling loop
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
				quit = true;
			}
			else {
				m_board->handleEvent(event);
			}
		}

		draw();
	}
}

void Game::draw()
{
	m_board->drawBoard();
}