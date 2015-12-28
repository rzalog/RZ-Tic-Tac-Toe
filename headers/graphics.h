#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

/*
 * Forward declarations
 */

/*
 * Includes
 */
#include <SDL2/SDL.h>

/*
 * Graphics class definition
 * ---
 * Handles all the graphics for the game
 */

class Graphics {
public:
	// constructor, creates a window with given dimensions
	Graphics(const char* title, const int init_xpos, const int init_ypos, const int width, const int height);
	// destructor
	~Graphics();

	// get the renderer
	SDL_Renderer* getRenderer() const;

	// to check that everything worked
	bool wasCreationSuccessful();

private:
	// if everything was created successfully
	bool m_creationSuccess;
	// window and renderer
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};


#endif